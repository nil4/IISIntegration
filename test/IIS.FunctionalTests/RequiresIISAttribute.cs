// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

using System;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Xml.Linq;
using Microsoft.AspNetCore.Testing.xunit;
using Microsoft.Win32;

namespace Microsoft.AspNetCore.Server.IISIntegration.FunctionalTests
{
    [AttributeUsage(AttributeTargets.Assembly | AttributeTargets.Class | AttributeTargets.Method)]
    public sealed class RequiresIISAttribute : Attribute, ITestCondition
    {
        private static readonly bool _isMetStatic;
        private static readonly string _skipReasonStatic;

        private readonly bool _isMet;
        private readonly string _skipReason;

        private static readonly bool _websocketsAvailable;
        private static readonly bool _windowsAuthAvailable;
        private static readonly bool _poolEnvironmentVariablesAvailable;

        static RequiresIISAttribute()
        {
            if (!RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                _skipReasonStatic = "IIS tests can only be run on Windows.";
                return;
            }

            var identity = WindowsIdentity.GetCurrent();
            var principal = new WindowsPrincipal(identity);
            if (!principal.IsInRole(WindowsBuiltInRole.Administrator))
            {
                _skipReasonStatic += "The current console is not running as admin.";
                return;
            }

            if (!File.Exists(Path.Combine(Environment.SystemDirectory, "inetsrv", "w3wp.exe")))
            {
                _skipReasonStatic += "The machine does not have IIS installed.";
                return;
            }

         
            var iisRegistryKey = Registry.LocalMachine.OpenSubKey(@"Software\Microsoft\InetStp", writable: false);
            if (iisRegistryKey == null)
            {
                _skipReasonStatic = "IIS is not installed on the machine.";
                _poolEnvironmentVariablesAvailable = false;
                return;
            }
            else
            {
                var majorVersion = (int)iisRegistryKey.GetValue("MajorVersion", -1);
                var minorVersion = (int)iisRegistryKey.GetValue("MinorVersion", -1);
                var version = new Version(majorVersion, minorVersion);
                if (version < new Version(7, 0))
                {
                    _skipReasonStatic = "IIS version 7.0 or higher must be installed on the test machine.";
                    return;
                }
                _poolEnvironmentVariablesAvailable = version >= new Version(10, 0);
            }

            var type = Type.GetTypeFromCLSID(new Guid("2B72133B-3F5B-4602-8952-803546CE3344"), throwOnError: false);

            try
            {
                var _ = Activator.CreateInstance(type);
            }
            catch (COMException comException)
            {
                _skipReasonStatic = $"IIS installtion doesn't have the correct features. ComException: {comException.Message}.";
                return;
            }
            var ancmConfigPath = Path.Combine(Environment.SystemDirectory, "inetsrv", "config", "schema", "aspnetcore_schema_v2.xml");

            if (!File.Exists(ancmConfigPath))
            {
                _skipReasonStatic = "ANCM Schema is not installed.";
                return;
            }

            XDocument ancmConfig;

            try
            {
                ancmConfig = XDocument.Load(ancmConfigPath);
            }
            catch
            {
                _skipReasonStatic = "Could not read ANCM schema configuration.";
                return;
            }

            _isMetStatic = ancmConfig
                .Root
                .Descendants("attribute")
                .Any(n => "hostingModel".Equals(n.Attribute("name")?.Value, StringComparison.Ordinal));

            _skipReasonStatic = _isMetStatic ? null : "IIS schema needs to be upgraded to support ANCM.";

            _websocketsAvailable = File.Exists(Path.Combine(Environment.SystemDirectory, "inetsrv", "iiswsock.dll"));

            _windowsAuthAvailable = File.Exists(Path.Combine(Environment.SystemDirectory, "inetsrv", "authsspi.dll"));
        }

        public RequiresIISAttribute() { }

        public RequiresIISAttribute(IISCapability capabilities)
        {
            _isMet = _isMetStatic;
            _skipReason = _skipReasonStatic;
            if (capabilities.HasFlag(IISCapability.Websockets))
            {
                _isMet &= _websocketsAvailable;
                if (!_websocketsAvailable)
                {
                    _skipReason += "The machine does not have IIS websockets installed.";
                }
            }
            if (capabilities.HasFlag(IISCapability.WindowsAuthentication))
            {
                _isMet &= _windowsAuthAvailable;

                if (!_windowsAuthAvailable)
                {
                    _skipReason += "The machine does not have IIS windows authentication installed.";
                }
            }
            if (capabilities.HasFlag(IISCapability.PoolEnvironmentVariables))
            {
                _isMet &= _poolEnvironmentVariablesAvailable;
                if (!_poolEnvironmentVariablesAvailable)
                {
                    _skipReason += "The machine does allow for setting environment variables on application pools.";
                }
            }
        }

        public bool IsMet => _isMet;
        public string SkipReason => _skipReason;
    }
}

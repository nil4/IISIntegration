using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace IISIntegration.FunctionalTests.Inprocess
{
    class HWCInterop
    {
        [DllImport(@"%windir%\system32\inetsrv\hwebcore.dll")]
        public static extern int WebCoreActivate(
            [In, MarshalAs(UnmanagedType.LPWStr)]
            string appHostConfigPath,     // Required
            [In, MarshalAs(UnmanagedType.LPWStr)]
            string rootWebConfigPath,    // Optional
            [In, MarshalAs(UnmanagedType.LPWStr)]
            string instanceName);             // Required

        [DllImport(@"%windir%\system32\inetsrv\hwebcore.dll")]
        public static extern int WebCoreShutdown(
            bool immediate);
    }

    class InprocessServer
    {
        public void Start()
        {
            var tempFolder = Path.GetTempPath();
            var exe = Path.Combine(tempFolder, "app.exe");
            var runtimeConfig = Path.Combine(tempFolder, "app.runtimeconfig.json");
            File.WriteAllText(exe, string.Empty);
            File.WriteAllText(runtimeConfig, string.Empty);
            string hostFxrPath = "";
            string apphostConfigTemplatePath = "";
            string weconfigTemplatePath = "";

            File.Copy(hostFxrPath, tempFolder);


        }
    }
}

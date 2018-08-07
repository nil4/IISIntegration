// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

using System;
using System.Xml.Linq;

namespace Microsoft.AspNetCore.Server.IntegrationTesting.IIS
{
    public static class WebConfigHelpers
    {
        public static Action<XElement, string> AddOrModifyAspNetCoreSection(string key, string value)
        {
            return (element, _) => {
                element
                    .AddOrUpdate("system.webServer")
                    .AddOrUpdate("aspNetCore")
                    .SetAttributeValue(key, value);
            };
        }

        public static Action<XElement, string> AddOrModifyHandlerSection(string key, string value)
        {
            return (element, _) =>
            {
                element
                    .AddOrUpdate("system.webServer")
                    .AddOrUpdate("handlers")
                    .AddOrUpdate("add")
                    .SetAttributeValue(key, value);
            };
        }
    }
}

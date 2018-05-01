// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

using System;
using System.Threading;

namespace Microsoft.AspNetCore.Server.IISIntegration.FunctionalTests
{
    public static class TestConstants
    {
        public static TimeSpan DefaultTimeOut { get; } = TimeSpan.FromSeconds(15);

        public static CancellationToken CancellationTokenWithDefaultTimeout()
        {
            return new CancellationTokenSource(DefaultTimeOut).Token;
        }
    }
}

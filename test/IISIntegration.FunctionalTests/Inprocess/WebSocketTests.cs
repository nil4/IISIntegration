// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

using System;
using System.Net.WebSockets;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Testing.xunit;
using Xunit;

namespace Microsoft.AspNetCore.Server.IISIntegration.FunctionalTests
{
    [Collection(IISTestSiteCollection.Name)]
    public class WebSocketsTests
    {
        private readonly IISTestSiteFixture _fixture;

        public WebSocketsTests(IISTestSiteFixture fixture)
        {
            _fixture = fixture;
        }

#if NETCOREAPP2_1
        [ConditionalFact]
        public async Task WebSocket_Echo()
        {
            var bytes = Encoding.ASCII.GetBytes("Hello World");
            var result = await _fixture.Client.GetAsync("/WebSocketEcho");
            var fromStream = WebSocket.CreateFromStream(await result.Content.ReadAsStreamAsync(), false, null, TestConstants.DefaultTimeOut);

            await fromStream.SendAsync(bytes, WebSocketMessageType.Text, true, TestConstants.CancellationTokenWithDefaultTimeout());
            await fromStream.ReceiveAsync(bytes, TestConstants.CancellationTokenWithDefaultTimeout());

            Assert.Equal("Hello World", Encoding.ASCII.GetString(bytes));
        }
#endif
    }
}

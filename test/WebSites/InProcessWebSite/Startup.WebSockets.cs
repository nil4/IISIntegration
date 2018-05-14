// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

using System;
using System.Collections.Generic;
using System.IO;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using IISIntegration.FunctionalTests;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Http.Features;
using Microsoft.Extensions.DependencyInjection;
using Xunit;

namespace IISTestSite
{
    public partial class Startup
    {

        private void WebsocketRequest(IApplicationBuilder app)
        {
            app.Run(async context =>
            {
                await context.Response.WriteAsync("test");
            });
        }

        private void WebReadBeforeUpgrade(IApplicationBuilder app)
        {
            app.Run(async context => {

                var singleByteArray = new byte[1];
                Assert.Equal(0, await context.Request.Body.ReadAsync(singleByteArray, 0, 1));

                var ws = await Upgrade(context);
                await SendMessages(ws, "Yay");
            });
        }

        private void WebSocketEcho(IApplicationBuilder app)
        {
            app.Run(async context =>
            {
                var ws = await Upgrade(context);

                var appLifetime = app.ApplicationServices.GetRequiredService<IApplicationLifetime>();

                await Echo(ws, appLifetime.ApplicationStopping);
            });
        }

        private void WebSocketLifetimeEvents(IApplicationBuilder app)
        {
            app.Run(async context => {

                var messages = new List<string>();

                context.Response.OnStarting(() => {
                    context.Response.Headers["custom-header"] = "value";
                    messages.Add("OnStarting");
                    return Task.CompletedTask;
                });

                var ws = await Upgrade(context);
                messages.Add("Upgraded");

                await SendMessages(ws, messages.ToArray());
            });
        }

        private static async Task SendMessages(WebSocket webSocket, params string[] messages)
        {
            foreach (var message in messages)
            {
                await webSocket.SendAsync(new ArraySegment<byte>(Encoding.ASCII.GetBytes(message)), WebSocketMessageType.Text, true, CancellationToken.None);
            }
        }

        private static async Task<WebSocket> Upgrade(HttpContext context)
        {
            var upgradeFeature = context.Features.Get<IHttpUpgradeFeature>();

            // Generate WebSocket response headers
            string key = context.Request.Headers[Constants.Headers.SecWebSocketKey].ToString();
            var responseHeaders = HandshakeHelpers.GenerateResponseHeaders(key);
            foreach (var headerPair in responseHeaders)
            {
                context.Response.Headers[headerPair.Key] = headerPair.Value;
            }

            // Upgrade the connection
            Stream opaqueTransport = await upgradeFeature.UpgradeAsync();

            // Get the WebSocket object
            var ws = WebSocketProtocol.CreateFromStream(opaqueTransport, isServer: true, subProtocol: null, keepAliveInterval: TimeSpan.FromMinutes(2));
            return ws;
        }

        private async Task Echo(WebSocket webSocket, CancellationToken token)
        {
            var buffer = new byte[1024 * 4];
            var result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), token);
            bool closeFromServer = false;
            string closeFromServerCmd = "CloseFromServer";
            int closeFromServerLength = closeFromServerCmd.Length;

            while (!result.CloseStatus.HasValue && !token.IsCancellationRequested && !closeFromServer)
            {
                if (result.Count == closeFromServerLength &&
                    Encoding.ASCII.GetString(buffer).Substring(0, result.Count) == closeFromServerCmd)
                {
                    // The client sent "CloseFromServer" text message to request the server to close (a test scenario).
                    closeFromServer = true;
                }
                else
                {
                    await webSocket.SendAsync(new ArraySegment<byte>(buffer, 0, result.Count), result.MessageType, result.EndOfMessage, token);
                    result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), token);
                }
            }

            if (result.CloseStatus.HasValue)
            {
                // Client-initiated close handshake
                await webSocket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription, CancellationToken.None);
            }
            else
            {
                // Server-initiated close handshake due to either of the two conditions:
                // (1) The applicaton host is performing a graceful shutdown.
                // (2) The client sent "CloseFromServer" text message to request the server to close (a test scenario).
                await webSocket.CloseOutputAsync(WebSocketCloseStatus.NormalClosure, closeFromServerCmd, CancellationToken.None);

                // The server has sent the Close frame.
                // Stop sending but keep receiving until we get the Close frame from the client.
                while (!result.CloseStatus.HasValue)
                {
                    result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
                }
            }
        }
    }
}

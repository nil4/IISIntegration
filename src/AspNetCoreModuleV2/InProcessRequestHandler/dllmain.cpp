// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

// dllmain.cpp : Defines the entry point for the DLL application.

#include <VersionHelpers.h>

#include "inprocessapplication.h"
#include "StartupExceptionApplication.h"
#include "inprocesshandler.h"
#include "requesthandler_config.h"
#include "debugutil.h"
#include "resources.h"
#include "exceptions.h"
#include "ShuttingDownApplication.h"

DECLARE_DEBUG_PRINT_OBJECT("aspnetcorev2_inprocess.dll");

BOOL                g_fGlobalInitialize = FALSE;
BOOL                g_fProcessDetach = FALSE;
SRWLOCK             g_srwLockRH;
IHttpServer *       g_pHttpServer = NULL;
HINSTANCE           g_hWinHttpModule;
HINSTANCE           g_hAspNetCoreModule;
HANDLE              g_hEventLog = NULL;
bool                g_fInProcessApplicationCreated = false;
PCWSTR              g_moduleName = L"inprocess";

HRESULT
InitializeGlobalConfiguration(
    IHttpServer * pServer,
    IHttpApplication* pHttpApplication
)
{
    if (!g_fGlobalInitialize)
    {
        SRWExclusiveLock lock(g_srwLockRH);

        if (!g_fGlobalInitialize)
        {
            g_pHttpServer = pServer;
            RETURN_IF_FAILED(ALLOC_CACHE_HANDLER::StaticInitialize());
            RETURN_IF_FAILED(IN_PROCESS_HANDLER::StaticInitialize());

            if (pServer->IsCommandLineLaunch())
            {
                g_hEventLog = RegisterEventSource(NULL, ASPNETCORE_IISEXPRESS_EVENT_PROVIDER);
            }
            else
            {
                g_hEventLog = RegisterEventSource(NULL, ASPNETCORE_EVENT_PROVIDER);
            }

            DebugInitializeFromConfig(*pServer, *pHttpApplication);

            g_fGlobalInitialize = TRUE;
        }
    }

    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    UNREFERENCED_PARAMETER(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        InitializeSRWLock(&g_srwLockRH);
        DebugInitialize(hModule);
        break;
    case DLL_PROCESS_DETACH:
        g_fProcessDetach = TRUE;
        DebugStop();
    default:
        break;
    }
    return TRUE;
}

HRESULT
__stdcall
CreateApplication(
    _In_  IHttpServer           *pServer,
    _In_  IHttpApplication      *pHttpApplication,
    _In_  APPLICATION_PARAMETER *pParameters,
    _In_  DWORD                  nParameters,
    _Out_ IAPPLICATION          **ppApplication
)
{
    try
    {
        RETURN_IF_FAILED(InitializeGlobalConfiguration(pServer, pHttpApplication));

        // In process application was already created so another call to CreateApplication
        // means that server is shutting does and request arrived in the meantime
        if (g_fInProcessApplicationCreated)
        {
            *ppApplication = new ShuttingDownApplication(*pServer, *pHttpApplication);
            return S_OK;
        }

        REQUESTHANDLER_CONFIG *pConfig = nullptr;
        RETURN_IF_FAILED(REQUESTHANDLER_CONFIG::CreateRequestHandlerConfig(pServer, pHttpApplication, &pConfig));
        std::unique_ptr<REQUESTHANDLER_CONFIG> pRequestHandlerConfig(pConfig);

        BOOL disableStartupPage = pConfig->QueryDisableStartUpErrorPage();

        auto pApplication = std::make_unique<IN_PROCESS_APPLICATION>(*pServer, *pHttpApplication, std::move(pRequestHandlerConfig), pParameters, nParameters);

        // never create two inprocess applications in one process
        g_fInProcessApplicationCreated = true;
        if (FAILED_LOG(pApplication->LoadManagedApplication()))
        {
            // Set the currently running application to a fake application that returns startup exceptions.
            auto pErrorApplication = std::make_unique<StartupExceptionApplication>(*pServer, *pHttpApplication, disableStartupPage);

            RETURN_IF_FAILED(pErrorApplication->StartMonitoringAppOffline());
            *ppApplication = pErrorApplication.release();
        }
        else
        {
            RETURN_IF_FAILED(pApplication->StartMonitoringAppOffline());
            *ppApplication = pApplication.release();
        }
    }
    CATCH_RETURN();

    return S_OK;
}

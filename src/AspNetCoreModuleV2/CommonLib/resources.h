// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "aspnetcore_msg.h"

#define IDS_INVALID_PROPERTY        1000
#define IDS_SERVER_ERROR            1001

#define ASPNETCORE_EVENT_PROVIDER L"IIS AspNetCore Module V2"
#define ASPNETCORE_IISEXPRESS_EVENT_PROVIDER L"IIS Express AspNetCore Module V2"

#define ASPNETCORE_EVENT_MSG_BUFFER_SIZE 256
#define ASPNETCORE_EVENT_PROCESS_START_SUCCESS_MSG           L"Application '%s' started process '%d' successfully and process '%d' is listening on port '%d'."
#define ASPNETCORE_EVENT_RAPID_FAIL_COUNT_EXCEEDED_MSG       L"Maximum rapid fail count per minute of '%d' exceeded."
#define ASPNETCORE_EVENT_PROCESS_START_ERROR_MSG             L"Application '%s' with physical root '%s' failed to start process with commandline '%s' at stage '%s', ErrorCode = '0x%x', assigned port %d, retryCounter '%d'."
#define ASPNETCORE_EVENT_PROCESS_START_FAILURE_MSG           L"Application '%s' with physical root '%s' failed to start process with commandline '%s' with multiple retries. The last try of listening port is '%d'. See previous warnings for details."
#define ASPNETCORE_EVENT_PROCESS_START_STATUS_ERROR_MSG      L"Application '%s' with physical root '%s' failed to start process with commandline '%s' , ErrorCode = '0x%x', processId '%d', processStatus '%d'."
#define ASPNETCORE_EVENT_PROCESS_START_PORTSETUP_ERROR_MSG   L"Application '%s' with physical root '%s' failed to choose listen port '%d' given port rang '%d - %d', EorrorCode = '0x%x'. If environment variable 'ASPNETCORE_PORT' was set, try removing it such that a random port is selected instead."
#define ASPNETCORE_EVENT_PROCESS_START_WRONGPORT_ERROR_MSG   L"Application '%s' with physical root '%s' created process with commandline '%s' but failed to listen on the given port '%d'"
#define ASPNETCORE_EVENT_PROCESS_START_NOTREADY_ERROR_MSG    L"Application '%s' with physical root '%s' created process with commandline '%s' but either crashed or did not respond or did not listen on the given port '%d', ErrorCode = '0x%x'"
#define ASPNETCORE_EVENT_PROCESS_SHUTDOWN_MSG                L"Application '%s' with physical root '%s' shut down process with Id '%d' listening on port '%d'"
#define ASPNETCORE_EVENT_INVALID_STDOUT_LOG_FILE_MSG         L"Warning: Could not create stdoutLogFile %s, ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_GRACEFUL_SHUTDOWN_FAILURE_MSG       L"Failed to gracefully shutdown process '%d'."
#define ASPNETCORE_EVENT_SENT_SHUTDOWN_HTTP_REQUEST_MSG      L"Sent shutdown HTTP message to process '%d' and received http status '%d'."
#define ASPNETCORE_EVENT_APP_SHUTDOWN_FAILURE_MSG            L"Failed to gracefully shutdown application '%s'."
#define ASPNETCORE_EVENT_LOAD_CLR_FALIURE_MSG                L"Application '%s' with physical root '%s' failed to load clr and managed application, ErrorCode = '0x%x."
#define ASPNETCORE_EVENT_APP_SHUTDOWN_SUCCESSFUL_MSG         L"Application '%s' has shutdown."
#define ASPNETCORE_EVENT_DUPLICATED_INPROCESS_APP_MSG        L"Only one inprocess application is allowed per IIS application pool. Please assign the application '%s' to a different IIS application pool."
#define ASPNETCORE_EVENT_MIXED_HOSTING_MODEL_ERROR_MSG       L"Mixed hosting model is not supported. Application '%s' configured with different hostingModel value '%d' other than the one of running application(s)."
#define ASPNETCORE_EVENT_UNKNOWN_HOSTING_MODEL_ERROR_MSG     L"Unknown hosting model '%s'. Please specify either hostingModel=\"inprocess\" or hostingModel=\"outofprocess\" in the web.config file."
#define ASPNETCORE_EVENT_UNKNOWN_HOSTING_MODEL_ERROR_LEVEL   EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_ADD_APPLICATION_ERROR_MSG           L"Failed to start application '%s', ErrorCode '0x%x'."
#define ASPNETCORE_EVENT_INPROCESS_THREAD_EXIT_STDOUT_MSG    L"Application '%s' with physical root '%s' hit unexpected managed background thread exit, ErrorCode = '0x%x. Last 4KB characters of captured stdout and stderr logs:\r\n%s"
#define ASPNETCORE_EVENT_INPROCESS_THREAD_EXIT_MSG           L"Application '%s' with physical root '%s' hit unexpected managed background thread exit, ErrorCode = '0x%x. Please check the stderr logs for more information."
#define ASPNETCORE_EVENT_APP_IN_SHUTDOWN_MSG                 L"Application shutting down."
#define ASPNETCORE_EVENT_RECYCLE_APPOFFLINE_MSG              L"Application '%s' was recycled after detecting the app_offline file."
#define ASPNETCORE_EVENT_MONITOR_APPOFFLINE_ERROR_MSG        L"Monitoring app_offline.htm failed for application '%s', ErrorCode '0x%x'. "
#define ASPNETCORE_EVENT_RECYCLE_CONFIGURATION_MSG           L"Application '%s' was recycled due to configuration change"
#define ASPNETCORE_EVENT_RECYCLE_FAILURE_CONFIGURATION_MSG   L"Failed to recycle application due to a configuration change at '%s'. Recycling worker process."
#define ASPNETCORE_EVENT_MODULE_DISABLED_MSG                 L"AspNetCore Module is disabled"
#define ASPNETCORE_EVENT_INPROCESS_FULL_FRAMEWORK_APP_LEVEL  EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_INPROCESS_FULL_FRAMEWORK_APP_MSG    L"Application '%s' was compiled for .NET Framework. Please compile for .NET core to run the inprocess application or change the process mode to out of process. ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_PORTABLE_APP_DOTNET_MISSING_MSG     L"Could not find dotnet.exe on the system PATH environment variable for portable application '%s'. Check that a valid path to dotnet is on the PATH and the bitness of dotnet matches the bitness of the IIS worker process. ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_HOSTFXR_DIRECTORY_NOT_FOUND_LEVEL   EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_HOSTFXR_DIRECTORY_NOT_FOUND_MSG     L"Could not find the hostfxr directory '%s' in the dotnet directory. ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_HOSTFXR_DLL_NOT_FOUND_LEVEL         EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_HOSTFXR_DLL_NOT_FOUND_MSG           L"Could not find hostfxr.dll in '%s'. ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_APPLICATION_EXE_NOT_FOUND_LEVEL     EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_APPLICATION_EXE_NOT_FOUND_MSG       L"Could not find application executable in '%s'. ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_INPROCESS_THREAD_EXCEPTION_MSG      L"Application '%s' with physical root '%s' hit unexpected managed exception, ErrorCode = '0x%x. Please check the stderr logs for more information."
#define ASPNETCORE_EVENT_INVALID_PROCESS_PATH_LEVEL          EVENTLOG_ERROR_TYPE
#define ASPNETCORE_EVENT_INVALID_PROCESS_PATH_MSG            L"Invalid or unknown processPath provided in web.config: processPath = '%s', ErrorCode = '0x%x'."
#define ASPNETCORE_EVENT_INPROCESS_RH_ERROR_MSG              L"Could not find the assembly '%s' for in-process application. Please confirm the Microsoft.AspNetCore.Server.IIS package is referenced in your application. Captured output: %s"
#define ASPNETCORE_EVENT_OUT_OF_PROCESS_RH_MISSING_MSG       L"Could not find the assembly '%s' for out-of-process application. Please confirm the assembly is installed correctly for IIS or IISExpress."
#define ASPNETCORE_EVENT_INPROCESS_START_SUCCESS_MSG         L"Application '%s' started the coreclr in-process successfully."

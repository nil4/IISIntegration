// TestHostfxr.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

typedef BOOL(WINAPI * hostfxr_main_fn) (CONST DWORD argc, CONST PCWSTR argv[]);

static CONST PCWSTR * ancmLocation;
static hostfxr_main_fn mainCallback;

EXTERN_C __MIDL_DECLSPEC_DLLEXPORT
INT
get_native_search_directories(
    CONST INT argc, CONST PCWSTR* argv, PWSTR buffer, DWORD buffer_size, DWORD* required_buffer_size
)
{
    StrCpyNW(buffer, *ancmLocation, buffer_size);
    return 0;
}

EXTERN_C __MIDL_DECLSPEC_DLLEXPORT
INT
hostfxr_main(
    CONST DWORD argc, CONST PCWSTR argv[]
)
{
    mainCallback(argc, argv);
    return 0;
}

EXTERN_C __MIDL_DECLSPEC_DLLEXPORT
INT
set_hostfxr_main_callback(
    CONST PCWSTR* location,
    hostfxr_main_fn callback
)
{
    ancmLocation = location;
    mainCallback = callback;

    return 0;
}

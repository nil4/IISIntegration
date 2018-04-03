#pragma once

struct KernelHandle
{
    typedef HANDLE HandleType;
    static const HANDLE InvalidValue = INVALID_HANDLE_VALUE;

    static void Close(HANDLE value);
};

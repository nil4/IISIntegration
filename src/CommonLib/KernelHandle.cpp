#include "stdafx.h"
#include "KernelHandle.h"

void KernelHandleTraits::Close(HANDLE value)
{
    CloseHandle(value);
}

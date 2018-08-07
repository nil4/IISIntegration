// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "sttimer.h"
#include "IOutputManager.h"
#include "HandleWrapper.h"

class FileOutputManager : public IOutputManager
{
    #define FILE_FLUSH_TIMEOUT 3000
public:
    FileOutputManager();
    ~FileOutputManager();

    HRESULT
    Initialize(PCWSTR pwzStdOutLogFileName, PCWSTR pwzApplciationpath);

    virtual bool GetStdOutContent(STRA* struStdOutput) override;
    virtual HRESULT Start() override;
    virtual HRESULT Stop() override;

private:
    HANDLE m_hLogFileHandle;
    STTIMER m_Timer;
    STRU m_wsStdOutLogFileName;
    STRU m_wsApplicationPath;
    STRU m_struLogFilePath;
    int m_fdPreviousStdOut;
    int m_fdPreviousStdErr;
    BOOL m_disposed;
    SRWLOCK m_srwLock;
};


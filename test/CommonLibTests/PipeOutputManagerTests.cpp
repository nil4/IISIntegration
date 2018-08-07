// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "stdafx.h"
#include "gtest/internal/gtest-port.h"
#include "PipeOutputManager.h"

class FileManagerWrapper
{
public:
    PipeOutputManager * manager;
    FileManagerWrapper(PipeOutputManager* m)
        : manager(m)
    {
        manager->Start();
    }

    ~FileManagerWrapper()
    {
        delete manager;
    }
};

namespace PipeOutputManagerTests
{
    TEST(PipeManagerOutputTest, BasicFunctionalityCheck)
    {
        PCWSTR expected = L"test";
        STRA output;

        PipeOutputManager* pManager = new PipeOutputManager();

        ASSERT_EQ(S_OK, pManager->Start());
        wprintf(expected);
        ASSERT_EQ(S_OK, pManager->Stop());

        pManager->GetStdOutContent(&output);
        ASSERT_STREQ(output.QueryStr(), "test");
        delete pManager;
    }

    TEST(PipeManagerOutputTest, BasicFunctionalityCheck)
    {
        PCWSTR expected = L"test";
        STRA output;

        PipeOutputManager* pManager = new PipeOutputManager();

        ASSERT_EQ(S_OK, pManager->Start());
        wprintf(expected, stderr);
        ASSERT_EQ(S_OK, pManager->Stop());

        pManager->GetStdOutContent(&output);
        ASSERT_STREQ(output.QueryStr(), "test");
        delete pManager;
    }

    TEST(PipeManagerOutputTest, CheckMaxPipeSize)
    {
        std::wstring test;
        STRA output;
        for (int i = 0; i < 2000; i++)
        {
            test.append(L"hello world");
        }

        PipeOutputManager* pManager = new PipeOutputManager();

        ASSERT_EQ(S_OK, pManager->Start());
        wprintf(test.c_str());
        ASSERT_EQ(S_OK, pManager->Stop());

        pManager->GetStdOutContent(&output);
        ASSERT_EQ(output.QueryCCH(), (DWORD)4096);
        delete pManager;
    }

    TEST(PipeManagerOutputTest, CheckMaxPipeSize)
    {
        std::wstring test;
        STRA output;
        for (int i = 0; i < 2000; i++)
        {
            test.append(L"hello world");
        }

        PipeOutputManager* pManager = new PipeOutputManager();

        ASSERT_EQ(S_OK, pManager->Start());
        wprintf(test.c_str(), stderr);
        ASSERT_EQ(S_OK, pManager->Stop());

        pManager->GetStdOutContent(&output);
        ASSERT_EQ(output.QueryCCH(), (DWORD)4096);
        delete pManager;
    }

    TEST(PipeManagerOutputTest, SetInvalidHandlesForErrAndOut)
    {
        auto m_fdPreviousStdOut = _dup(_fileno(stdout));
        auto m_fdPreviousStdErr = _dup(_fileno(stderr));

        SetStdHandle(STD_ERROR_HANDLE, INVALID_HANDLE_VALUE);
        SetStdHandle(STD_OUTPUT_HANDLE, INVALID_HANDLE_VALUE);

        PCWSTR expected = L"test";

        PipeOutputManager* pManager = new PipeOutputManager();
        ASSERT_EQ(S_OK, pManager->Start());

        _dup2(m_fdPreviousStdOut, _fileno(stdout));
        _dup2(m_fdPreviousStdErr, _fileno(stderr));

        // Test will fail if we didn't redirect stdout back to a file descriptor.
        // This is because gtest relies on console output to know if a test succeeded or failed.
        // If the output still points to a file/pipe, the test (and all other tests after it) will fail.
        delete pManager;
    }
}


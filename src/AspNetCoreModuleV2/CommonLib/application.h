// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "stdafx.h"

class APPLICATION : public IAPPLICATION
{

public:

    APPLICATION_STATUS
    QueryStatus() override
    {
        return m_status;
    }

    APPLICATION()
        : m_cRefs(1)
    {
    }

    VOID
    Release() override
    {
        DereferenceApplication();
    }

    VOID
    ReferenceApplication()
    {
        InterlockedIncrement(&m_cRefs);
    }

    VOID
    DereferenceApplication()
    {
        DBG_ASSERT(m_cRefs != 0);

        if (InterlockedDecrement(&m_cRefs) == 0)
        {
            delete this;
        }
    }

protected:
    volatile APPLICATION_STATUS     m_status = APPLICATION_STATUS::UNKNOWN;

private:
    mutable LONG                    m_cRefs;
};

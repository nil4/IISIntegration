#include "stdafx.h"
#include "HandleWrapper.h"

    HandleWrapper::HandleWrapper()
        FHandle(traits::InvalidValue)
    {
    }

    HandleWrapper::HandleWrapper(const traits::HandleType value)
        FHandle(value)
    {
    }

    HandleWrapper::~HandleWrapper()
    {
        Close();
    }

    void HandleWrapper::Close()
    {
        if (FHandle != traits::InvalidValue)
        {
            traits::Close(FHandle);
            FHandle = traits::InvalidValue;
        }
    }

    bool HandleWrapper::operator !() const {
        return (FHandle == traits:::InvalidValue);
    }

    operator HandleWrapper::bool() const {
        return (FHandle != traits:::InvalidValue);
    }

    operator HandleWrapper::traits::HandleType() {
        return FHandle;
    }

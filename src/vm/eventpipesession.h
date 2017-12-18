// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#ifndef __EVENTPIPE_SESSION_H__
#define __EVENTPIPE_SESSION_H__

#ifdef FEATURE_PERFTRACING

class EventPipeSession
{
private:
    // The set of configurations for each enabled provider.
    EventPipeEnabledProviderList *m_pEnabledProviderList;

    // The configured size of the circular buffer.
    size_t m_circularBufferSizeInBytes;
    
    // True if rundown is enabled.
    Volatile<bool> m_rundownEnabled;

public:

    // TODO: This needs to be exposed via EventPipe::CreateSession() and EventPipe::DeleteSession() to avoid memory ownership issues.
    EventPipeSession(
        unsigned int circularBufferSizeInMB,
        EventPipeProviderConfiguration *pProviders,
        unsigned int numProviders)
    {
        LIMITED_METHOD_CONTRACT;
        m_circularBufferSizeInBytes = circularBufferSizeInMB * 1024; // 1MB;
        m_rundownEnabled = false;
        m_pEnabledProviderList = new EventPipeEnabledProviderList(
            pProviders,
            numProviders);
    }

    ~EventPipeSession()
    {
        LIMITED_METHOD_CONTRACT;
        if(m_pEnabledProviderList != NULL)
        {
            delete m_pEnabledProviderList;
            m_pEnabledProviderList = NULL;
        }
    }

    // Get the configured size of the circular buffer.
    size_t GetCircularBufferSize() const
    {
        LIMITED_METHOD_CONTRACT;
        return m_circularBufferSizeInBytes;
    }

    // Determine if rundown is enabled.
    bool RundownEnabled() const
    {
        LIMITED_METHOD_CONTRACT;
        return m_rundownEnabled;
    }

    // Set the rundown enabled flag.
    void SetRundownEnabled(bool value)
    {
        LIMITED_METHOD_CONTRACT;
        m_rundownEnabled = value;
    }

    // Get the enabled provider for the specified provider if present.
    EventPipeEnabledProvider* GetEnabledProvider(EventPipeProvider *pProvider)
    {
        LIMITED_METHOD_CONTRACT;
        return m_pEnabledProviderList->GetEnabledProvider(pProvider);
    }
};

#endif // FEATURE_PERFTRACING

#endif // __EVENTPIPE_SESSION_H__

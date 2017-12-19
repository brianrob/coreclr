// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#ifndef __EVENTPIPE_SESSION_H__
#define __EVENTPIPE_SESSION_H__

#ifdef FEATURE_PERFTRACING

enum class EventPipeEventLevel;
struct EventPipeProviderConfiguration;
class EventPipeSessionProviderList;
class EventPipeSessionProvider;

class EventPipeSession
{
private:
    // The set of configurations for each provider in the session.
    EventPipeSessionProviderList *m_pProviderList;

    // The configured size of the circular buffer.
    size_t m_circularBufferSizeInBytes;
    
    // True if rundown is enabled.
    Volatile<bool> m_rundownEnabled;

public:

    // TODO: This needs to be exposed via EventPipe::CreateSession() and EventPipe::DeleteSession() to avoid memory ownership issues.
    EventPipeSession(
        unsigned int circularBufferSizeInMB,
        EventPipeProviderConfiguration *pProviders,
        unsigned int numProviders);

    ~EventPipeSession();

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

    // Get the session provider for the specified provider if present.
    EventPipeSessionProvider* GetSessionProvider(EventPipeProvider *pProvider);
};

class EventPipeSessionProviderList
{

private:

    // The number of providers in the list.
    unsigned int m_numProviders;

    // The list of providers.
    EventPipeSessionProvider *m_pProviders;

    // A catch-all provider used when tracing is enabled at start-up
    // under (COMPlus_PerformanceTracing & 1) == 1.
    EventPipeSessionProvider *m_pCatchAllProvider;

public:

    // Create a new list based on the input.
    EventPipeSessionProviderList(EventPipeProviderConfiguration *pConfigs, unsigned int numConfigs);
    ~EventPipeSessionProviderList();

    // Get the session provider for the specified provider.
    // Return NULL if one doesn't exist.
    EventPipeSessionProvider* GetSessionProvider(EventPipeProvider *pProvider);
};

class EventPipeSessionProvider
{
private:

    // The provider name.
    WCHAR *m_pProviderName;

    // The enabled keywords.
    UINT64 m_keywords;

    // The loging level.
    EventPipeEventLevel m_loggingLevel;

public:

    EventPipeSessionProvider();
    ~EventPipeSessionProvider();

    void Set(LPCWSTR providerName, UINT64 keywords, EventPipeEventLevel loggingLevel);

    LPCWSTR GetProviderName() const;

    UINT64 GetKeywords() const;

    EventPipeEventLevel GetLevel() const;
};

#endif // FEATURE_PERFTRACING

#endif // __EVENTPIPE_SESSION_H__

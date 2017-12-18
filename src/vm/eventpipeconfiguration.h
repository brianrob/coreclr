// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#ifndef __EVENTPIPE_CONFIGURATION_H__
#define __EVENTPIPE_CONFIGURATION_H__

#ifdef FEATURE_PERFTRACING

#include "slist.h"

class EventPipeEnabledProvider;
class EventPipeEnabledProviderList;
class EventPipeEvent;
class EventPipeEventInstance;
class EventPipeProvider;
struct EventPipeProviderConfiguration;
class EventPipeSession;

enum class EventPipeEventLevel
{
    LogAlways,
    Critical,
    Error,
    Warning,
    Informational,
    Verbose
};

class EventPipeConfiguration
{
public:

    EventPipeConfiguration();
    ~EventPipeConfiguration();

    // Perform initialization that cannot be performed in the constructor.
    void Initialize();

    // Create a new provider.
    EventPipeProvider* CreateProvider(const SString &providerName, EventPipeCallback pCallbackFunction, void *pCallbackData);

    // Delete a provider.
    void DeleteProvider(EventPipeProvider *pProvider);

    // Register a provider.
    bool RegisterProvider(EventPipeProvider &provider);

    // Unregister a provider.
    bool UnregisterProvider(EventPipeProvider &provider);

    // Get the provider with the specified provider ID if it exists.
    EventPipeProvider* GetProvider(const SString &providerID);

    // Create a new session.
    EventPipeSession* CreateSession(unsigned int circularBufferSizeInMB, EventPipeProviderConfiguration *pProviders, unsigned int numProviders);

    // Delete a session.
    void DeleteSession(EventPipeSession *pSession);

    // Get the configured size of the circular buffer.
    size_t GetCircularBufferSize() const;

    // Enable a session in the event pipe.
    void Enable(EventPipeSession *pSession);

    // Disable a session in the event pipe.
    void Disable(EventPipeSession *pSession);

    // Get the status of the event pipe.
    bool Enabled() const;

    // Determine if rundown is enabled.
    bool RundownEnabled() const;

    // Enable rundown using the specified configuration.
    void EnableRundown(EventPipeSession *pSession);

    // Get the event used to write metadata to the event stream.
    EventPipeEventInstance* BuildEventMetadataEvent(EventPipeEventInstance &sourceInstance);

    // Delete deferred providers.
    void DeleteDeferredProviders();

private:

    // Get the provider without taking the lock.
    EventPipeProvider* GetProviderNoLock(const SString &providerID);

    // Get the enabled provider.
    EventPipeEnabledProvider* GetEnabledProviderForSession(EventPipeSession *pSession, EventPipeProvider *pProvider);

    // The one and only EventPipe session.
    EventPipeSession *m_pSession;

    // Determines whether or not the event pipe is enabled.
    Volatile<bool> m_enabled;

    // The list of event pipe providers.
    SList<SListElem<EventPipeProvider*>> *m_pProviderList;

    // The provider used to write configuration events to the event stream.
    EventPipeProvider *m_pConfigProvider;

    // The event used to write event information to the event stream.
    EventPipeEvent *m_pMetadataEvent;

    // The provider name for the configuration event pipe provider.
    // This provider is used to emit configuration events.
    const static WCHAR* s_configurationProviderName;

    // True if rundown is enabled.
    Volatile<bool> m_rundownEnabled;
};

class EventPipeEnabledProviderList
{

private:

    // The number of providers in the list.
    unsigned int m_numProviders;

    // The list of providers.
    EventPipeEnabledProvider *m_pProviders;

    // A catch-all provider used when tracing is enabled at start-up
    // under (COMPlus_PerformanceTracing & 1) == 1.
    EventPipeEnabledProvider *m_pCatchAllProvider;

public:

    // Create a new list based on the input.
    EventPipeEnabledProviderList(EventPipeProviderConfiguration *pConfigs, unsigned int numConfigs);
    ~EventPipeEnabledProviderList();

    // Get the enabled provider for the specified provider.
    // Return NULL if one doesn't exist.
    EventPipeEnabledProvider* GetEnabledProvider(EventPipeProvider *pProvider);
};

class EventPipeEnabledProvider
{
private:

    // The provider name.
    WCHAR *m_pProviderName;

    // The enabled keywords.
    UINT64 m_keywords;

    // The loging level.
    EventPipeEventLevel m_loggingLevel;

public:

    EventPipeEnabledProvider();
    ~EventPipeEnabledProvider();

    void Set(LPCWSTR providerName, UINT64 keywords, EventPipeEventLevel loggingLevel);

    LPCWSTR GetProviderName() const;

    UINT64 GetKeywords() const;

    EventPipeEventLevel GetLevel() const;
};

#endif // FEATURE_PERFTRACING

#endif // __EVENTPIPE_CONFIGURATION_H__

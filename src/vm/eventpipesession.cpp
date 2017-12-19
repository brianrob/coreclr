// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "common.h"
#include "eventpipe.h"
#include "eventpipeprovider.h"
#include "eventpipesession.h"

#ifdef FEATURE_PERFTRACING

EventPipeSession::EventPipeSession(
    unsigned int circularBufferSizeInMB,
    EventPipeProviderConfiguration *pProviders,
    unsigned int numProviders)
{
    LIMITED_METHOD_CONTRACT;
    m_circularBufferSizeInBytes = circularBufferSizeInMB * 1024; // 1MB;
    m_rundownEnabled = false;
    m_pProviderList = new EventPipeSessionProviderList(
        pProviders,
        numProviders);
}

EventPipeSession::~EventPipeSession()
{
    LIMITED_METHOD_CONTRACT;
    if(m_pProviderList != NULL)
    {
        delete m_pProviderList;
        m_pProviderList = NULL;
    }
}

EventPipeSessionProvider* EventPipeSession::GetSessionProvider(EventPipeProvider *pProvider)
{
    LIMITED_METHOD_CONTRACT;
    return m_pProviderList->GetSessionProvider(pProvider);
}

EventPipeSessionProviderList::EventPipeSessionProviderList(
    EventPipeProviderConfiguration *pConfigs,
    unsigned int numConfigs)
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    m_pProviders = NULL;
    m_pCatchAllProvider = NULL;
    m_numProviders = 0;

    // Test COMPLUS variable to enable tracing at start-up.
    // If tracing is enabled at start-up create the catch-all provider and always return it.
    if((CLRConfig::GetConfigValue(CLRConfig::INTERNAL_EnableEventPipe) & 1) == 1)
    {
        m_pCatchAllProvider = new EventPipeSessionProvider();
        m_pCatchAllProvider->Set(NULL, 0xFFFFFFFFFFFFFFFF, EventPipeEventLevel::Verbose);
        return;
    }

    m_pCatchAllProvider = NULL;
    m_numProviders = numConfigs;
    if(m_numProviders == 0)
    {
        return;
    }

    m_pProviders = new EventPipeSessionProvider[m_numProviders];
    for(int i=0; i<m_numProviders; i++)
    {
        m_pProviders[i].Set(
            pConfigs[i].GetProviderName(),
            pConfigs[i].GetKeywords(),
            (EventPipeEventLevel)pConfigs[i].GetLevel());
    }
}

EventPipeSessionProviderList::~EventPipeSessionProviderList()
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    if(m_pProviders != NULL)
    {
        delete[] m_pProviders;
        m_pProviders = NULL;
    }
    if(m_pCatchAllProvider != NULL)
    {
        delete(m_pCatchAllProvider);
        m_pCatchAllProvider = NULL;
    }
}

EventPipeSessionProvider* EventPipeSessionProviderList::GetSessionProvider(
    EventPipeProvider *pProvider)
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    // If tracing was enabled on start-up, all events should be on (this is a diagnostic config).
    if(m_pCatchAllProvider != NULL)
    {
        return m_pCatchAllProvider;
    }

    if(m_pProviders == NULL)
    {
        return NULL;
    }

    SString providerNameStr = pProvider->GetProviderName();
    LPCWSTR providerName = providerNameStr.GetUnicode();

    EventPipeSessionProvider *pSessionProvider = NULL;
    for(int i=0; i<m_numProviders; i++)
    {
        EventPipeSessionProvider *pCandidate = &m_pProviders[i];
        if(pCandidate != NULL)
        {
            if(wcscmp(providerName, pCandidate->GetProviderName()) == 0)
            {
                pSessionProvider = pCandidate;
                break;
            }
        }
    }

    return pSessionProvider;
}

EventPipeSessionProvider::EventPipeSessionProvider()
{
    LIMITED_METHOD_CONTRACT;
    m_pProviderName = NULL;
    m_keywords = 0;
}

EventPipeSessionProvider::~EventPipeSessionProvider()
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    if(m_pProviderName != NULL)
    {
        delete[] m_pProviderName;
        m_pProviderName = NULL;
    }
}

void EventPipeSessionProvider::Set(LPCWSTR providerName, UINT64 keywords, EventPipeEventLevel loggingLevel)
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    if(m_pProviderName != NULL)
    {
        delete(m_pProviderName);
        m_pProviderName = NULL;
    }

    if(providerName != NULL)
    {
        unsigned int bufSize = wcslen(providerName) + 1;
        m_pProviderName = new WCHAR[bufSize];
        wcscpy_s(m_pProviderName, bufSize, providerName);
    }
    m_keywords = keywords;
    m_loggingLevel = loggingLevel;
}

LPCWSTR EventPipeSessionProvider::GetProviderName() const
{
    LIMITED_METHOD_CONTRACT;
    return m_pProviderName;
}

UINT64 EventPipeSessionProvider::GetKeywords() const
{
    LIMITED_METHOD_CONTRACT;
    return m_keywords;
}

EventPipeEventLevel EventPipeSessionProvider::GetLevel() const
{
    LIMITED_METHOD_CONTRACT;
    return m_loggingLevel;
}

#endif // FEATURE_PERFTRACING

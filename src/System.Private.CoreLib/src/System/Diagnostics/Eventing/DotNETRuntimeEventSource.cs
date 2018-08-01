﻿// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
namespace System.Diagnostics.Tracing
{
#if FEATURE_PERFTRACING
    /// <summary>
    /// RuntimeEventSource is an EventSource that represents the ETW/EventPipe events emitted by the native runtime.
    /// Most of RuntimeEventSource is auto-generated by scripts/genRuntimeEventSources.py based on the contents of the Microsoft-Windows-DotNETRuntime provider.
    /// </summary>
    internal sealed partial class RuntimeEventSource : EventSource
    {
        /// <summary>
        /// Dispatch a single event with the specified event ID and payload.
        /// </summary>
        /// <param name="eventID">The eventID corresponding to the event as defined in the auto-generated portion of the RuntimeEventSource class.</param>
        /// <param name="payload">A span pointing to the data payload for the event.</param>
        [NonEvent]
        internal unsafe void ProcessEvent(uint eventID, uint osThreadID, DateTimeOffset timeStamp, Guid activityId, Guid childActivityId, ReadOnlySpan<Byte> payload)
        {
            // Make sure the eventID is valid.
            if (eventID >= m_eventData.Length)
            {
                return;
            }

            // Decode the payload.
            object[] decodedPayloadFields = EventPipePayloadDecoder.DecodePayload(ref m_eventData[eventID], payload);
            WriteToAllListeners(
                eventId: (int)eventID,
                osThreadId: &osThreadID,
                timeStamp: &timeStamp,
                activityID: &activityId,
                childActivityID: &childActivityId,
                args: decodedPayloadFields);
        }
   }
#endif // FEATURE_PERFTRACING
}

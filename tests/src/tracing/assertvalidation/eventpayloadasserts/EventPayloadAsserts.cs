using System;
using System.Diagnostics.Tracing;
using System.IO;
using Tracing.Tests.Common;

namespace Tracing.Tests
{
    // Validate that no asserts are fired for events with and without arguments.
    public static class EventPayloadAsserts
    {
        private const string EventSourceName = "EventPayloadAssertsEventSource";

        public static int Main(string[] args)
        {
            bool pass = true;
            bool keepOutput = false;

            // Use the first arg as an output filename if there is one.
            string outputFilename = null;
            if (args.Length >= 1)
            {
                outputFilename = args[0];
                keepOutput = true;
            }
            else
            {
                outputFilename = System.IO.Path.GetTempPath() + Guid.NewGuid().ToString() + ".netperf";
            }

            try
            {
                Console.WriteLine("\tStart: Enable tracing.");
                TraceConfiguration config = new TraceConfiguration(
                    "out.netperf",
                    1024*1024*1024); // 1 GB circular buffer
                config.EnableProvider(EventSourceName, 0xFFFFFFFFFFFFFFFF, 5);
                TraceControl.Enable(config);
                Console.WriteLine("\tEnd: Enable tracing.\n");

                Console.WriteLine("\tStart: Generate some events.");
                // TODO: Add 3rd argument (name)
                Assert.Equal<bool>(EventPayloadAssertsEventSource.Log.IsEnabled(), true);
                EventPayloadAssertsEventSource.Log.EventWithNoArgs();
                EventPayloadAssertsEventSource.Log.EventWithArgs(1, "Hello World!");
                Console.WriteLine("\tEnd: Generate some events.\n");

                Console.WriteLine("\tStart: Disable tracing.");
                TraceControl.Disable();
                Console.WriteLine("\tEnd: Disable tracing.\n");
            }
            finally
            {
                if (keepOutput)
                {
                    Console.WriteLine("\n\tOutput file: {0}", outputFilename);
                }
                else
                {
                    System.IO.File.Delete(outputFilename);
                }
            }

            return 100;
        }

        [EventSource(Name=EventSourceName)]
        public sealed class EventPayloadAssertsEventSource : EventSource
        {
            public static EventPayloadAssertsEventSource Log = new EventPayloadAssertsEventSource();

            private EventPayloadAssertsEventSource()
            {
            }

            [Event(1)]
            public void EventWithNoArgs()
            {
                WriteEvent(1);
            }

            [Event(2)]
            public void EventWithArgs(int i, string s)
            {
                WriteEvent(2, i, s);
            }
        }
    }
}

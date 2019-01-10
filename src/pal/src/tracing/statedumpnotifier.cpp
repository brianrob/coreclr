// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "statedumpnotifier.h"

static void
StateDumpNotifierCallback(struct lttng_session *session, void *priv)
{
}

__attribute__((constructor (300)))
static void
InitializeTracingStateDumpNotifier()
{
    static struct lttng_ust_notifier notifier;

	lttng_ust_init_statedump_notifier(&notifier, StateDumpNotifierCallback, NULL);
	lttng_ust_register_statedump_notifier(&notifier);
}

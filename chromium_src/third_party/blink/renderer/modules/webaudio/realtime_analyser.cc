/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define BRAVE_REALTIMEANALYSER_CONVERTFLOATTODB \
  destination[i] = audio_farbling_callback_.Run(destination[i], i);

#define BRAVE_REALTIMEANALYSER_CONVERTTOBYTEDATA \
  scaled_value = audio_farbling_callback_.Run(scaled_value, i);

#define BRAVE_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA \
  destination[i] = audio_farbling_callback_.Run(value, i);

#define BRAVE_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA \
  value = audio_farbling_callback_.Run(value, i);

#include "../../../../../../third_party/blink/renderer/modules/webaudio/realtime_analyser.cc"

#undef BRAVE_REALTIMEANALYSER_CONVERTFLOATTODB
#undef BRAVE_REALTIMEANALYSER_CONVERTTOBYTEDATA
#undef BRAVE_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA
#undef BRAVE_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA

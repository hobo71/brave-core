/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <random>

#include "brave/third_party/blink/renderer/brave_farbling_constants.h"
#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/page/plugin_data.h"
#include "third_party/blink/renderer/modules/plugins/dom_plugin.h"
#include "third_party/blink/renderer/modules/plugins/dom_plugin_array.h"
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

using blink::DOMPlugin;
using blink::DOMPluginArray;
using blink::HeapVector;
using blink::LocalFrame;
using blink::MakeGarbageCollected;
using blink::Member;
using blink::PluginInfo;

namespace brave {

void FarblePlugins(DOMPluginArray* owner,
                   HeapVector<Member<DOMPlugin>>* dom_plugins) {
  LocalFrame* frame = owner->GetFrame();
  if (!frame || !frame->GetContentSettingsClient())
    return;
  switch (frame->GetContentSettingsClient()->GetBraveFarblingLevel()) {
    case BraveFarblingLevel::OFF: {
      break;
    }
    case BraveFarblingLevel::MAXIMUM: {
      dom_plugins->clear();
      // "Maximum" behavior is clear existing plugins + "balanced" behavior,
      // so fall through here.
      U_FALLTHROUGH;
    }
    case BraveFarblingLevel::BALANCED: {
      // The item() method will populate plugin info if any item of
      // |dom_plugins_| is null, but when it tries, it assumes the
      // length of |dom_plugins_| == the length of the underlying
      // GetPluginData()->Plugins(). Once we add our fake plugins, that
      // assumption will break and the item() method will crash with an
      // out-of-bounds array access. Rather than patch the item() method, we
      // ensure that the cache is fully populated now while the assumptions
      // still hold, so the problematic code is never executed later.
      for (unsigned index = 0; index < dom_plugins->size(); index++) {
        (*dom_plugins)[index] = owner->item(index);
      }
      // Add fake plugin #1.
      auto* fake_plugin_info_1 = MakeGarbageCollected<PluginInfo>(
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_1_NAME", 8),
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_1_FILENAME", 16),
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_1_DESCRIPTION", 32),
          0, false);
      auto* fake_dom_plugin_1 =
          MakeGarbageCollected<DOMPlugin>(frame, *fake_plugin_info_1);
      dom_plugins->push_back(fake_dom_plugin_1);
      // Add fake plugin #2.
      auto* fake_plugin_info_2 = MakeGarbageCollected<PluginInfo>(
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_2_NAME", 7),
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_2_FILENAME", 15),
          BraveSessionCache::From(*(frame->GetDocument()))
              .GenerateRandomString("PLUGIN_2_DESCRIPTION", 31),
          0, false);
      auto* fake_dom_plugin_2 =
          MakeGarbageCollected<DOMPlugin>(frame, *fake_plugin_info_2);
      dom_plugins->push_back(fake_dom_plugin_2);
      // Shuffle the list of plugins pseudo-randomly, based on the domain key.
      std::mt19937_64 prng = BraveSessionCache::From(*(frame->GetDocument()))
                                 .MakePseudoRandomGenerator();
      std::shuffle(dom_plugins->begin(), dom_plugins->end(), prng);
      break;
    }
    default:
      NOTREACHED();
  }
}

}  // namespace brave

#define BRAVE_DOM_PLUGINS_UPDATE_PLUGIN_DATA \
  brave::FarblePlugins(this, &dom_plugins_);

#include "../../../../../../third_party/blink/renderer/modules/plugins/dom_plugin_array.cc"

#undef BRAVE_DOM_PLUGIN_ARRAY_GET_PLUGIN_DATA

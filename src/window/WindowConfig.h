#ifndef XENGINE_WINDOW_WINDOWCONFIG_H
#define XENGINE_WINDOW_WINDOWCONFIG_H

#include "WindowDefine.h"

#include "core/Types.h"

#include "resource/ResourceIdentity.h"

#include <EASTL/string.h>

namespace xEngine {

struct WindowConfig {
  static WindowConfig ForWindow(int32 width, int32 height, const eastl::string &title);

  static WindowConfig ForFullScreen(const eastl::string &title);

  ResourceIdentity identity{ResourceIdentity::Unique()};

  int32 width{1024};

  int32 height{768};

  int32 frame_buffer_width{1024};

  int32 frame_buffer_height{768};

  bool is_full_screen{false};

  eastl::string title{"xEngine"};

};

inline WindowConfig WindowConfig::ForWindow(int32 width, int32 height, const eastl::string &title) {
  WindowConfig config;
  config.width = width;
  config.height = height;
  config.is_full_screen = false;
  config.title = title;
  return config;
}

inline WindowConfig WindowConfig::ForFullScreen(const eastl::string &title) {
  WindowConfig config;
  config.is_full_screen = true;
  config.title = title;
  return config;
}

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOWCONFIG_H
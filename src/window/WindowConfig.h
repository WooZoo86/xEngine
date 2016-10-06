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

  PixelFormat color_format{PixelFormat::RGBA8};

  PixelFormat depth_format{PixelFormat::D24S8};

  int32 sample_count{1};

  bool is_full_screen{false};

  int32 swap_interval{1};

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
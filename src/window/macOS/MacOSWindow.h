#ifndef XENGINE_WINDOW_MACOS_MACOSWINDOW_H
#define XENGINE_WINDOW_MACOS_MACOSWINDOW_H

#if X_MACOS

#include "window/WindowInterface.h"

namespace xEngine {

class MacOSWindow: public WindowInterface {
 public:
  static void Initialize();

  static void Finalize();

  static void Tick();

 public:
  virtual void Create(const WindowConfig &config) override;

  virtual void Destroy() override;

  virtual void SetTitle(const eastl::string &name) override;

  virtual void *GetNativeHandle() override { return window_; }

 private:
  void *window_{nullptr};

  friend void SetShouldClose(MacOSWindow *window) { window->should_close_ = true; }

  friend WindowConfig &GetConfig(MacOSWindow *window) { return window->config_; }

  friend void SetMousePosition(MacOSWindow *window, float32 x, float32 y) {
    window->mouse_position_.x = x;
    window->mouse_position_.y = y;
  }

  friend void SetMouseButtonDown(MacOSWindow *window, MouseButtonType type) {
    window->mouse_button_down_status_ |= static_cast<uint8>(type);
    window->mouse_button_status_cache_ |= static_cast<uint8>(type);
  }

  friend void SetMouseButtonUp(MacOSWindow *window, MouseButtonType type) {
    window->mouse_button_up_status_ |= static_cast<uint8>(type);
    window->mouse_button_status_cache_ ^= static_cast<uint8>(type);
  }

  friend void SetMouseScroll(MacOSWindow *window, float32 x, float32 y) {
    window->mouse_scroll_.x = x;
    window->mouse_scroll_.y = y;
  }
};

} // namespace xEngine

#endif // X_MACOS

#endif // XENGINE_WINDOW_MACOS_MACOSWINDOW_H
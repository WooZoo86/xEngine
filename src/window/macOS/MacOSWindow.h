#ifndef XENGINE_WINDOW_MACOS_MACOSWINDOW_H
#define XENGINE_WINDOW_MACOS_MACOSWINDOW_H

#if X_MACOS

#include "window/WindowInterface.h"

namespace xEngine {

class MacOSWindow: public WindowInterface {
 public:
  static void Initialize();

  static void Finalize();

  static void PollEvent();

 public:
  virtual void Create(const WindowConfig &config) override;

  virtual void Destroy() override;

  virtual bool Available() const override;

  virtual bool ShouldClose() override;

  virtual void SetTitle(const eastl::string &name) override;

  virtual void *GetNativeHandle() override { return window_; }

 private:
  bool should_close_{false};
  void *window_{nullptr};

  friend void SetShouldClose(MacOSWindow *window) { window->should_close_ = true; }

  friend WindowConfig &GetConfig(MacOSWindow *window) { return window->config_; }
};

} // namespace xEngine

#endif // X_MACOS

#endif // XENGINE_WINDOW_MACOS_MACOSWINDOW_H
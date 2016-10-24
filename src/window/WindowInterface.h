#ifndef XENGINE_WINDOW_WINDOWINTERFACE_H
#define XENGINE_WINDOW_WINDOWINTERFACE_H

#include "WindowConfig.h"

#include "graphics/Graphics.h"

#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

namespace xEngine {

class WindowInterface {
 public:
  virtual ~WindowInterface() {}

  virtual void Create(const WindowConfig &config) = 0;

  virtual void Destroy() = 0;

  virtual void SetTitle(const eastl::string &name) = 0;

  virtual void *GetNativeHandle() = 0;

  bool ShouldClose() { return should_close_; }

  const glm::vec2 &GetMousePosition() { return mouse_position_; }

  bool IsMouseButtonDown(MouseButtonType type) { return mouse_button_down_status_ & static_cast<uint8>(type); }

  bool IsMouseButtonUp(MouseButtonType type) { return mouse_button_up_status_ & static_cast<uint8>(type); }

  bool IsMouseButtonPressed(MouseButtonType type) { return mouse_button_status_cache_ & static_cast<uint8>(type); }

  const WindowConfig &config() const { return config_; }

  const eastl::unique_ptr<Graphics> &graphics() const { return graphics_; }

 private:
  void ResetMouseStatus() { mouse_button_down_status_ = 0; mouse_button_up_status_ = 0; }

 protected:
  WindowConfig config_;

  bool should_close_{false};

  glm::vec2 mouse_position_;

  uint8 mouse_button_down_status_{0};
  uint8 mouse_button_up_status_{0};
  uint8 mouse_button_status_cache_{0};

  eastl::unique_ptr<Graphics> graphics_{new Graphics};

  friend class Window;
};

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOWINTERFACE_H
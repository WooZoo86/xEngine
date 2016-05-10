#ifndef XENGINE_WINDOW_GLFW_GLFWWINDOW_H
#define XENGINE_WINDOW_GLFW_GLFWWINDOW_H

#if X_WINDOWS || X_OSX || X_LINUX

#include "window/WindowInterface.h"

#include <GLFW/glfw3.h>

namespace xEngine {

class GLFWWindow : public WindowInterface {
 public:
  virtual void Initialize(const WindowConfig &config) override;

  virtual void Finalize() override;

  virtual bool Available() override;

  virtual bool ShouldClose() override;

  virtual void PollEvent() override;

  virtual void Present() override;

  virtual void SetTitle(const eastl::string &name) override;

  virtual const WindowConfig &GetConfig() const override;

 private:
  static void ErrorCallback(int error, const char *desc);

 private:
  static GLFWWindow *self_;
  WindowConfig config_;
  GLFWwindow *window_{nullptr};
};

} // namespace xEngine

#endif // X_WINDOWS || X_OSX || X_LINUX

#endif // XENGINE_WINDOW_GLFW_GLFWWINDOW_H
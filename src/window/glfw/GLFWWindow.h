#ifndef XENGINE_WINDOW_GLFW_GLFWWINDOW_H
#define XENGINE_WINDOW_GLFW_GLFWWINDOW_H

#if X_USE_GLFW

#include "window/WindowInterface.h"

#include <GLFW/glfw3.h>

namespace xEngine {

class GLFWWindow: public WindowInterface {
 public:
  static void Initialize();

  static void Finalize();

  static void PollEvent();

 public:
  virtual void Create(const WindowConfig &config) override;

  virtual void Destroy() override;

  virtual bool Available() const override;

  virtual void MakeCurrent() override;

  virtual bool ShouldClose() override;

  virtual void Present() override;

  virtual void SetTitle(const eastl::string &name) override;

  virtual void *GetNativeHandle() override { return window_; }

 private:
  static void ErrorCallback(int error, const char *desc);

 private:
  GLFWwindow *window_{nullptr};
};

} // namespace xEngine

#endif // X_USE_GLFW

#endif // XENGINE_WINDOW_GLFW_GLFWWINDOW_H
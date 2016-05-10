#if X_WINDOWS || X_OSX || X_LINUX

#include "GLFWWindow.h"

#include "core/Core.h"

namespace xEngine {

GLFWWindow *GLFWWindow::self_ = nullptr;

void GLFWWindow::Initialize(const WindowConfig &config) {
  self_ = this;
  x_assert(!Available());
  config_ = config;
  if (glfwInit() != GL_TRUE) {
    x_error("GLFW init error!\n");
  }
  glfwSetErrorCallback(ErrorCallback);
  glfwWindowHint(GLFW_RED_BITS, ChannelBitOfPixelFormat(config_.color_format, PixelChannel::RED));
  glfwWindowHint(GLFW_GREEN_BITS, ChannelBitOfPixelFormat(config_.color_format, PixelChannel::GREEN));
  glfwWindowHint(GLFW_BLUE_BITS, ChannelBitOfPixelFormat(config_.color_format, PixelChannel::BLUE));
  glfwWindowHint(GLFW_ALPHA_BITS, ChannelBitOfPixelFormat(config_.color_format, PixelChannel::ALPHA));
  glfwWindowHint(GLFW_DEPTH_BITS, ChannelBitOfPixelFormat(config_.depth_format, PixelChannel::DEPTH));
  glfwWindowHint(GLFW_STENCIL_BITS, ChannelBitOfPixelFormat(config_.depth_format, PixelChannel::STENCIL));
  glfwWindowHint(GLFW_SAMPLES, config_.sample_count > 1 ? config_.sample_count : 0);
#if X_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // X_DEBUG
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window_ = glfwCreateWindow(config_.width, config_.height, config_.title.c_str(), nullptr, nullptr);
  x_assert(Available());
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(config_.swap_interval);
}

void GLFWWindow::Finalize() {
  x_assert(Available());
  glfwDestroyWindow(window_);
  window_ = nullptr;
  glfwTerminate();
  self_ = nullptr;
}

bool GLFWWindow::Available() {
  return window_ != nullptr;
}

bool GLFWWindow::ShouldClose() {
  x_assert(Available());
  return glfwWindowShouldClose(window_) == GL_TRUE;
}

void GLFWWindow::PollEvent() {
  x_assert(Available());
  glfwPollEvents();
}

void GLFWWindow::Present() {
  x_assert(Available());
  glfwSwapBuffers(window_);
}

void GLFWWindow::SetTitle(const eastl::string &name) {
  x_assert(Available());
  glfwSetWindowTitle(window_, name.c_str());
}

void GLFWWindow::ErrorCallback(int error, const char *desc) {
  Log::GetInstance().Error("glfw error[%d]: %s\n", error, desc);
}

const WindowConfig &GLFWWindow::GetConfig() const {
  return config_;
}

} // namespace xEngine

#endif // X_WINDOWS || X_OSX || X_LINUX
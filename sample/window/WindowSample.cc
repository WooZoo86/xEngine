#include "application/Application.h"
#include "core/Log.h"
#include "window/Window.h"

using namespace xEngine;

class WindowSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(1024, 768, "WindowSample"));
    Window::GetInstance().MakeCurrent(window_id_);
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    Window::GetInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    Window::GetInstance().PollEvent();
    Window::GetInstance().PresentAllWindow();
    if (Window::GetInstance().ShouldClose(window_id_)) {
      Window::GetInstance().Destroy(window_id_);
      window_id_ = kInvalidResourceID;
    }
    return Window::GetInstance().IsAllClosed() ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  ResourceID window_id_{kInvalidResourceID};
};

XENGINE_APPLICATION(WindowSample)
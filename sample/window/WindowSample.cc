#include "application/Application.h"
#include "core/Log.h"
#include "window/Window.h"

using namespace xEngine;

class WindowSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    window = new Window;
    window->Initialize(WindowConfig::ForWindow(1024, 768, "WindowSample"));
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    window->Finalize();
    delete window;
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    window->PollEvent();
    window->Present();
    return window->ShouldClose() ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  Window *window{nullptr};
};

XENGINE_APPLICATION(WindowSample)
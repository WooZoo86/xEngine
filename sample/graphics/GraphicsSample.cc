#include "application/Application.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"

using namespace xEngine;

class GraphicsSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(1024, 768, "GraphicsSample"));
    Window::GetInstance().MakeCurrent(window_id_);
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    Window::GetInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    if (window_id_ != kInvalidResourceID) {
      state_.clear_color += Color(0.01f, 0.005f, 0.0025f, 0.0f);
      state_.clear_color = glm::mod(state_.clear_color, 1.0f);
      Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyTarget();
      Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyClearState(state_);
			Window::GetInstance().GetGraphics(window_id_)->renderer()->Render();
    }
    Window::GetInstance().PollEvent();
    Window::GetInstance().PresentAllWindow();
    if (Window::GetInstance().ShouldClose(window_id_)) {
      Window::GetInstance().GetGraphics(window_id_)->Finalize();
      Window::GetInstance().Destroy(window_id_);
      window_id_ = kInvalidResourceID;
    }
    return Window::GetInstance().IsAllClosed() ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }
 private:
  ResourceID window_id_{kInvalidResourceID};
  ClearState state_;
};

XENGINE_WINDOW_APPLICATION(GraphicsSample)
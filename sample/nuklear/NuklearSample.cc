#include "application/Application.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "graphics/RendererInterface.h"
#include "graphics/GraphicsResourceManagerInterface.h"
#include "window/Window.h"
#include "NuklearGUI.h"

#define LEN(a) (sizeof(a)/sizeof(a)[0])
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#include <demo/overview.c>
#undef LEN
#undef MAX

using namespace xEngine;

class NuklearSample: public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(1024, 768, "NuklearSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();
    NuklearGUI::GetInstance().Initialize(NuklearConfig::ForWindow(window_id_));
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    Window::GetInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    if (window_id_ != kInvalidResourceID) {
      auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
      renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
      NuklearGUI::GetInstance().BeginFrame();
      overview(NuklearGUI::GetInstance().context());
      NuklearGUI::GetInstance().EndFrame();
      renderer->Render();
    }

    Window::GetInstance().PollEvent();
    if (Window::GetInstance().ShouldClose(window_id_)) {
      NuklearGUI::GetInstance().Finalize();
      Window::GetInstance().GetGraphics(window_id_)->Finalize();
      Window::GetInstance().Destroy(window_id_);
      window_id_ = kInvalidResourceID;
    }
    return Window::GetInstance().IsAllClosed() ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  ResourceID window_id_{kInvalidResourceID};
};

XENGINE_WINDOW_APPLICATION(NuklearSample)
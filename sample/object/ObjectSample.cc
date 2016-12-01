#include "application/ApplicationDelegate.h"
#include "window/Window.h"
#include "object/core/Scene.h"
#include "object/component/Camera.h"

using namespace xEngine;

class ObjectSample : public ApplicationDelegate, WindowDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, 1024, 768, "ObjectSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

    Scene::GetInstance().Initialize();
    auto camera = Scene::GetInstance().AddGameObject();
    component_ = eastl::static_pointer_cast<Camera>(camera->AttachComponent(ComponentType::kCamera));
    component_->set_render_target(window_id_);
    component_->set_clear_type(CameraClearType::kSolidColor);
  }

  virtual void Finalize() override {
    Scene::GetInstance().Finalize();
    Window::GetInstance().Finalize();
  }

  virtual void OnWindowClose() override {
    Window::GetInstance().GetGraphics(window_id_)->Finalize();
    Window::GetInstance().Destroy(window_id_);
    window_id_ = kInvalidResourceID;
#if X_WINDOWS
    Application::GetInstance().Quit();
#endif
  }

  virtual void OnWindowUpdate() override {
    component_->set_background_color(glm::mod(component_->background_color() + Color(0.01f, 0.005f, 0.0025f, 0.0f), 1.0f));
    Scene::GetInstance().Update();
  }

 private:
  ResourceID window_id_{kInvalidResourceID};
  CameraPtr component_;
};

XENGINE_WINDOW_APPLICATION(ObjectSample)
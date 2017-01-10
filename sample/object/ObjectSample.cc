#include "application/ApplicationDelegate.h"
#include "window/Window.h"
#include "object/core/Scene.h"
#include "object/component/Camera.h"
#include "object/component/MeshRenderer.h"

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
    auto camera_component = eastl::static_pointer_cast<Camera>(camera->AttachComponent(ComponentType::kCamera));
    camera_component->set_render_target(window_id_);
    camera_component->set_clear_type(CameraClearType::kSolidColor);

    auto cube = Scene::GetInstance().AddGameObject();
    auto cube_component = eastl::static_pointer_cast<MeshRenderer>(cube->AttachComponent(ComponentType::kMeshRenderer));
    cube_component->set_mesh(Mesh::Parse(window_id_, MeshUtil::Cube()));
    cube_component->SetMaterialCount(1);
    auto material = cube_component->GetMaterial(0);
    material.reset(new Material);
    material->set_shader(Shader::Parse(window_id_, ));
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
    Scene::GetInstance().Update();
  }

 private:
  ResourceID window_id_{kInvalidResourceID};
};

XENGINE_WINDOW_APPLICATION(ObjectSample)
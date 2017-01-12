#include "application/ApplicationDelegate.h"
#include "window/Window.h"
#include "io/IO.h"
#include "storage/Storage.h"
#include "object/core/Scene.h"
#include "object/component/Camera.h"
#include "object/component/MeshRenderer.h"
#include "asset/Shader.h"
#include "asset/Mesh.h"
#include "util/MeshUtil.h"

using namespace xEngine;

class ObjectSample : public ApplicationDelegate, WindowDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, 1024, 768, "ObjectSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

    IO::GetInstance().Initialize();
    IO::GetInstance().AddPlaceholder("texture", "storage://" +
      Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("texture").string() + Path::separator());
    IO::GetInstance().AddPlaceholder("shader", "storage://" +
      Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("shader").string() + Path::separator());
    IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    IO::GetInstance().Read("shader:Blinn-Phong.Texture.shader", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        shader_ = Shader::Parse(window_id_, data);
        shader_->pipeline_config().depth_stencil_state.depth_enable = true;
        shader_->pipeline_config().depth_stencil_state.depth_write_enable = true;
        shader_->pipeline_config().rasterizer_state.cull_face_enable = true;
        shader_->Initialize();

        auto cube = Scene::GetInstance().AddGameObject();
        auto cube_component = eastl::static_pointer_cast<MeshRenderer>(cube->AttachComponent(ComponentType::kMeshRenderer));
        cube_component->set_mesh(cube_mesh_);
        cube_component->SetMaterialCount(1);
        auto material = cube_component->GetMaterial(0);
        material.reset(new Material);
        material->set_shader(shader_);
      }
    });

    cube_mesh_ = Mesh::Parse(window_id_, MeshUtil::Cube());
    cube_mesh_->Initialize();

    Scene::GetInstance().Initialize();

    auto camera = Scene::GetInstance().AddGameObject();
    auto camera_component = eastl::static_pointer_cast<Camera>(camera->AttachComponent(ComponentType::kCamera));
    camera_component->set_render_target(window_id_);
    camera_component->set_clear_type(CameraClearType::kSolidColor);
  }

  virtual void Finalize() override {
    IO::GetInstance().Finalize();
    Window::GetInstance().Finalize();
  }

  virtual void OnWindowClose() override {
    cube_mesh_->Finalize();

    Scene::GetInstance().Finalize();

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
  MeshPtr cube_mesh_;
  ShaderPtr shader_;
};

XENGINE_WINDOW_APPLICATION(ObjectSample)
#include "application/ApplicationDelegate.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "asset/graphics/Shader.h"
#include "asset/graphics/Camera.h"
#include "asset/graphics/util/MeshUtil.h"
#include "window/Window.h"
#include "io/IO.h"
#include "storage/Storage.h"

#include <stb_image.h>

#include <gtc/matrix_transform.hpp>

using namespace xEngine;

class SkyBoxSample : public ApplicationDelegate, WindowDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, 1024, 768, "SkyBoxSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

    IO::GetInstance().Initialize();
    IO::GetInstance().AddPlaceholder("texture", "storage://" +
        Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("texture").string() + Path::separator());
    IO::GetInstance().AddPlaceholder("shader", "storage://" +
        Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("shader").string() + Path::separator());
    IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    load_camera();
    load_shader();
    load_sampler();
    load_texture();
  }

  virtual void Finalize() override {
    IO::GetInstance().Finalize();
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
    draw();
  }

  virtual void OnWindowMousePosition(const glm::vec2 &position) override {
    static glm::vec2 last_position;

    auto delta = position - last_position;
    last_position = position;

    if (Window::GetInstance().Get(window_id_)->IsMouseButtonPressed(MouseButtonType::kRight)) {
      delta /= 10.0f;
      camera_->Rotate(delta.x, delta.y);
    }

    if (Window::GetInstance().Get(window_id_)->IsMouseButtonPressed(MouseButtonType::kMiddle)) {
      delta /= 50.0f;
      camera_->Move(delta.x, delta.y);
    }
  }

  virtual void OnWindowMouseScroll(const glm::vec2 &offset) override {
//    camera_->set_zoom(camera_->zoom() + offset.y / 10.0f);
  }

 private:
  void load_shader() {
    IO::GetInstance().Read("shader:SkyBox.shader", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        shader_ = Shader::Parse(window_id_, data);
        shader_->Initialize();

        auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader_->UpdateResourceData("uView", Data::Create(glm::value_ptr(view), sizeof(view)));
        auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);
        shader_->UpdateResourceData("uProjection", Data::Create(glm::value_ptr(projection), sizeof(projection)));

        load_mesh();
      }
    });
  }

  void load_sampler() {
    SamplerConfig sampler_config;
    sampler_config.wrap_mod_s = TextureWrapMode::kClampToEdge;
    sampler_config.wrap_mod_t = TextureWrapMode::kClampToEdge;
    sampler_config.wrap_mod_r = TextureWrapMode::kClampToEdge;
    sampler_config.filter_mode_min = TextureFilterMode::kLinear;
    sampler_config.filter_mode_mag = TextureFilterMode::kLinear;
    sampler_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(sampler_config);
  }

  void load_texture() {
    for (auto index = 0; index < static_cast<uint8>(GraphicsMaxDefine::kMaxCubeTextureFaceCount); ++index) {
      char path[32] = {0};
      sprintf(path, "texture:skybox/%d.jpg", index);
      IO::GetInstance().Read(path, [=](Location location, IOStatus status, DataPtr data) {
        if (status == IOStatus::kSuccess) {
          int components;
          stbi_set_unpremultiply_on_load(1);
          stbi_convert_iphone_png_to_rgb(1);
          auto result = stbi_info_from_memory(reinterpret_cast<const stbi_uc *>(data->buffer()),
                                              static_cast<int>(data->size()),
                                              &texture_width, &texture_height, &components);
          if (result == 1 && texture_width > 0 && texture_height > 0) {
            auto buffer = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(data->buffer()),
                                                static_cast<int>(data->size()),
                                                &texture_width, &texture_height, &components, STBI_rgb);
            texture_data_[index] = Data::Create(reinterpret_cast<const char *>(buffer), static_cast<size_t>(texture_width * texture_height * 4));
            stbi_image_free(buffer);

            load_cube();
          }
        }
      });
    }
  }

  void load_cube() {
    TextureConfig config;
    config.width = texture_width;
    config.height = texture_height;
    config.type = TextureType::kTextureCube;
    config.color_format = PixelFormat::RGB8;
    for (auto index = 0; index < static_cast<uint8>(GraphicsMaxDefine::kMaxCubeTextureFaceCount); ++index) {
      if (texture_data_[index] == nullptr) return;
      config.data[index][0] = texture_data_[index];
    }
    cube_texture_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(config);
  }

  void load_mesh() {
    VertexLayout layout;
    layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);

    box_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(MeshUtil::Cube().config());

    auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_->resource_id(), layout);
    pipeline_config.depth_stencil_state.depth_enable = false;
    pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
  }

  void load_camera() {
    camera_ = Camera::CreateUnique();
    camera_->set_render_window(window_id_);
    camera_->set_position(glm::vec3(0.0f, 0.0f, 3.0f));
    camera_->set_target(glm::vec3(0.0f, 0.0f, 0.0f));
    camera_->set_up_direction(glm::vec3(0.0f, 1.0f, 0.0f));
  }

  void draw() {
    if (shader_ == nullptr) return;

    auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
    renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
    
    renderer->ApplyPipeline(pipeline_);

    shader_->UpdateResourceData("uProjection", Data::Create(glm::value_ptr(camera_->projection_matrix()), sizeof(glm::mat4)));
    shader_->UpdateResourceData("uView", Data::Create(glm::value_ptr(glm::mat4(glm::mat3(camera_->view_matrix()))), sizeof(glm::mat4)));
    
    shader_->UpdateResourceSampler("uTexture", sampler_);
    shader_->UpdateResourceTexture("uTexture", cube_texture_);

    renderer->ApplyMesh(box_);
    renderer->Draw(DrawCallState::Triangles(36));

    renderer->Render();
  }

 private:
  ShaderPtr shader_;
  CameraUniquePtr camera_;

  DataPtr texture_data_[static_cast<uint8>(GraphicsMaxDefine::kMaxCubeTextureFaceCount)];

  int32 texture_width, texture_height;

  ResourceID box_{kInvalidResourceID};

  ResourceID cube_texture_{kInvalidResourceID};
  ResourceID sampler_{kInvalidResourceID};

  ResourceID pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
};

XENGINE_WINDOW_APPLICATION(SkyBoxSample)
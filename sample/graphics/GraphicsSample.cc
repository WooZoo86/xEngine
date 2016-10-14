#include "application/Application.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"
#include "io/IO.h"
#include "storage/StorageFilesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace xEngine;

class GraphicsSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(1024, 768, "GraphicsSample"));
    Window::GetInstance().MakeCurrent(window_id_);
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    IO::GetInstance().Initialize();
#if X_MACOS
    IO::GetInstance().AddPlaceholder("local", "storage:///Users/leafnsand/Desktop/");
#elif X_WINDOWS
    IO::GetInstance().AddPlaceholder("local", "storage://C:\\Users\\leafnsand\\Desktop\\");
#endif
    IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    load_shader();
    load_texture();
    load_mesh();
    state_ = ClearState::ClearColor();

    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    Window::GetInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    IO::GetInstance().Tick();

    draw();

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
  void load_shader() {
    static const auto generate_shader = [&]() {
      if (shader_ == kInvalidResourceID && vertex_shader_ && fragment_shader_) {
        shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(ShaderConfig::FromData(vertex_shader_->buffer(), fragment_shader_->buffer()));
      }
    };
    IO::GetInstance().Read("local:vertex.shader", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        vertex_shader_ = data;
        generate_shader();
      }
    });
    IO::GetInstance().Read("local:fragment.shader", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        fragment_shader_ = data;
        generate_shader();
      }
    });
  }

  void load_texture() {
    IO::GetInstance().Read("local:test.jpg", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        int width, height, components;
        stbi_set_unpremultiply_on_load(1);
        stbi_convert_iphone_png_to_rgb(1);
        auto result = stbi_info_from_memory(reinterpret_cast<const stbi_uc *>(data->buffer()),
                                            static_cast<int>(data->size()),
                                            &width, &height, &components);
        if (result == 1 && width > 0 && height > 0)
        {
          auto buffer = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(data->buffer()),
                                              static_cast<int>(data->size()),
                                              &width, &height, &components, STBI_rgb_alpha);
          data->Copy(reinterpret_cast<const char *>(buffer), static_cast<size_t>(width * height * 4));
          stbi_image_free(buffer);
          TextureConfig config;
          config.width = width;
          config.height = height;
          config.color_format = PixelFormat::RGBA8;
          config.data[0][0] = data;
          texture_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(config);
        }
      }
    });
  }

  void load_mesh() {
    float32 vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };
    uint16 indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    MeshConfig mesh_config;
    mesh_config.index_type = IndexFormat::kUint16;
    mesh_config.index_count = sizeof(indices) / sizeof(uint16);
    mesh_config.index_data = indices;
    mesh_config.vertex_count = 4;
    mesh_config.vertex_data = vertices;
    mesh_config.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat2)
        .AddElement(VertexElementSemantic::kColor0, VertexElementFormat::kFloat3)
        .AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
    mesh_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh_config);
  }

  void draw() {
    static const auto texture_index = 0;
    if (window_id_ != kInvalidResourceID) {
      state_.clear_color += Color(0.01f, 0.005f, 0.0025f, 0.0f);
      state_.clear_color = glm::mod(state_.clear_color, 1.0f);
      auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
      renderer->Reset();
      renderer->ApplyTarget(kInvalidResourceID, state_);
      renderer->ApplyShader(shader_);
      renderer->ApplyTexture(texture_, texture_index);
      renderer->ApplyMesh(mesh_);
      renderer->UpdateShaderUniform(shader_, "tex", UniformFormat::kTexture, &texture_index);
      renderer->DrawTopology(VertexTopology::kTriangles, 0, 6);
      renderer->Render();
    }
  }

 private:
  ResourceID shader_{kInvalidResourceID};
  ResourceID texture_{kInvalidResourceID};
  ResourceID mesh_{kInvalidResourceID};
  DataPtr vertex_shader_{nullptr};
  DataPtr fragment_shader_{nullptr};
  ResourceID window_id_{kInvalidResourceID};
  ClearState state_;
};

XENGINE_WINDOW_APPLICATION(GraphicsSample)
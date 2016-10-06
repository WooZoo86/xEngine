#include "application/Application.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "graphics/RendererInterface.h"
#include "graphics/GraphicsResourceManagerInterface.h"
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
    load_vertex_data();
    load_texture();

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
        shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(ShaderConfig::FromData(vertex_shader_, fragment_shader_));
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

  void load_vertex_data() {
    float32 vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };
    uint16 indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    VertexDataConfig vertex_config;
    vertex_config.buffer_usage = BufferUsage::kStatic;
    vertex_config.data = Data::Create(reinterpret_cast<const char *>(vertices), sizeof(vertices));
    vertex_config.vertex_size = sizeof(vertices) / 4;
    VertexElementConfig position_element;
    position_element.type = VertexElementType::kPosition;
    position_element.format = VertexElementFormat::kFloat2;
    position_element.is_normalized = true;
    vertex_config.element[0] = position_element;
    VertexElementConfig color_element;
    color_element.type = VertexElementType::kColor;
    color_element.format = VertexElementFormat::kFloat3;
    color_element.is_normalized = true;
    color_element.offset = 2 * sizeof(float32);
    vertex_config.element[1] = color_element;
    vertex_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(vertex_config);
    IndexDataConfig index_config;
    index_config.buffer_usage = BufferUsage::kStatic;
    index_config.data = Data::Create(reinterpret_cast<const char *>(indices), sizeof(indices));
    index_config.type = IndexFormat::kUint16;
    index_config.count = sizeof(indices) / sizeof(uint16);
    index_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(index_config);
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
          config.type = TextureType::kTexture2d;
          config.color_format = PixelFormat::RGBA8;
          config.data_size[0][0] = data->size();
          config.data = data;
          texture_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(config);
        }
      }
    });
  }

  void draw() {
    if (window_id_ != kInvalidResourceID) {
      state_.clear_color += Color(0.01f, 0.005f, 0.0025f, 0.0f);
      state_.clear_color = glm::mod(state_.clear_color, 1.0f);
      auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
      renderer->ApplyTarget();
      renderer->ApplyClearState(state_);
      renderer->Render();
      renderer->ApplyShader(shader_);
      renderer->ApplyVertexData(vertex_);
      renderer->ApplyIndexData(index_);
      renderer->ApplyTexture(texture_);
      renderer->DrawTopology(VertexTopology::kTriangles, 0, 6);
    }
  }

 private:
  ResourceID shader_{kInvalidResourceID};
  ResourceID vertex_{kInvalidResourceID};
  ResourceID index_{kInvalidResourceID};
  ResourceID texture_{kInvalidResourceID};
  DataPtr vertex_shader_{nullptr};
  DataPtr fragment_shader_{nullptr};
  ResourceID window_id_{kInvalidResourceID};
  ClearState state_;
};

XENGINE_WINDOW_APPLICATION(GraphicsSample)
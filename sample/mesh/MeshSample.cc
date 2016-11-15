#include "application/ApplicationDelegate.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"
#include "io/IO.h"
#include "storage/StorageFilesystem.h"
#include "asset/graphics/MeshLoader.h"
#include "asset/graphics/Camera.h"

#include "../graphics/GraphicsSample.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <gtc/matrix_transform.hpp>

#include <EASTL/chrono.h>
#include <EASTL/tuple.h>

#if X_OPENGL

static const char *vertex_shader =
    "#version 410 core\n"
    "in vec3 aPosition;\n"
    "uniform mat4 uModel;\n"
    "uniform mat4 uView;\n"
    "uniform mat4 uProjection;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);\n"
    "}\n";

static const char *fragment_shader =
    "#version 410 core\n"
    "out vec4 outColor;\n"
    "void main()\n"
    "{\n"
    "    outColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

#elif X_D3D11

static const char *vertex_shader =
  "float4x4 uModel;\n"
  "float4x4 uView;\n"
  "float4x4 uProjection;\n"
  "struct VS_INPUT\n"
  "{\n"
  "    float3 aPosition: POSITION;\n"
  "    float2 aTexcoord0: TEXCOORD;\n"
  "};\n"
  "struct VS_OUTPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "    float2 Texcoord: TEXCOORD;\n"
  "};\n"
  "VS_OUTPUT main(const VS_INPUT input)\n"
  "{\n"
  "    VS_OUTPUT output;\n"
  "    output.Texcoord = input.aTexcoord0;\n"
  "    output.Position = mul(uProjection, mul(uView, mul(uModel, float4(input.aPosition, 1.0))));\n"
  "    return output;\n"
  "}\n";

static const char *fragment_shader =
  "Texture2D uTexture;\n"
  "SamplerState uTexture_sampler;\n"
  "struct PS_INPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "    float2 Texcoord: TEXCOORD;\n"
  "};\n"
  "float4 main(const PS_INPUT input): SV_TARGET\n"
  "{\n"
  "    return uTexture.Sample(uTexture_sampler, input.Texcoord);\n"
  "}\n";

#endif

using namespace xEngine;

class MeshSample : public ApplicationDelegate, WindowDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, 1024, 768, "MeshSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();
    IO::GetInstance().Initialize();
#if X_MACOS
    IO::GetInstance().AddPlaceholder("local", "storage:///Users/leafnsand/Desktop/");
#elif X_WINDOWS
    IO::GetInstance().AddPlaceholder("local", "storage://C:\\Users\\leafnsand\\Desktop\\");
#endif
    IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    load_shader();
    load_camera();
    load_mesh();
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
    camera_->Zoom(offset.y / 50.0f);
  }

 private:
  void load_shader() {
    auto shader_config = ShaderConfig::FromData(Data::Create(vertex_shader, strlen(vertex_shader) + 1), Data::Create(fragment_shader, strlen(fragment_shader) + 1));
    shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config);
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
    IO::GetInstance().Read("local:teapot.xmesh", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        auto mesh_vector = MeshLoader::Load(data);
        for (auto mesh : mesh_vector) {
          auto id = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh.config());
          auto state = DrawCallState::Triangles(mesh.config().index_count);
          mesh_.push_back(eastl::make_tuple(id, state));
        }
      }
    });
    auto jpg = Data::Create(reinterpret_cast<const char *>(test_jpg), test_jpg_len);
    int width, height, components;
    stbi_set_unpremultiply_on_load(1);
    stbi_convert_iphone_png_to_rgb(1);
    auto result = stbi_info_from_memory(reinterpret_cast<const stbi_uc *>(jpg->buffer()),
      static_cast<int>(jpg->size()),
      &width, &height, &components);
    if (result == 1 && width > 0 && height > 0)
    {
      auto buffer = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(jpg->buffer()),
        static_cast<int>(jpg->size()),
        &width, &height, &components, STBI_rgb_alpha);
      auto data = Data::Create(reinterpret_cast<const char *>(buffer), static_cast<size_t>(width * height * 4));
      stbi_image_free(buffer);
      TextureConfig config;
      config.width = width;
      config.height = height;
      config.color_format = PixelFormat::RGBA8;
      config.data[0][0] = data;
      texture_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(config);
    }
  }

  void load_camera() {
    camera_ = Camera::CreateUnique();
    camera_->set_position(glm::vec3(0.0f, 0.0f, 3.0f));
    camera_->set_target(glm::vec3(0.0f, 0.0f, 0.0f));
    camera_->set_up_direction(glm::vec3(0.0f, 1.0f, 0.0f));

    auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
    auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 1000.0f);
    renderer->UpdateShaderResourceData(shader_, "uProjection", Data::Create(glm::value_ptr(projection), sizeof(projection)));
    renderer->UpdateShaderResourceData(shader_, "uModel", Data::Create(glm::value_ptr(glm::mat4()), sizeof(glm::mat4)));
  }

  void load_mesh() {
    IO::GetInstance().Read("local:teapot.xmesh", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        auto mesh_vector = MeshLoader::Load(data);
        for (auto mesh : mesh_vector) {
          auto id = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh.config());
          auto state = DrawCallState::Triangles(mesh.config().index_count);
          mesh_.push_back(eastl::make_tuple(id, state));
        }
      }
    });

    VertexLayout layout;
    layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);

    auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, layout);
    pipeline_config.depth_stencil_state.depth_enable = true;
    pipeline_config.depth_stencil_state.depth_write_enable = true;
//    pipeline_config.rasterizer_state.fill_mode = FillMode::kWireframe;
    pipeline_config.rasterizer_state.cull_face_enable = true;
    pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
  }

  void draw() {
    auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();

    renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
    renderer->ApplyShader(shader_);
    renderer->ApplyPipeline(pipeline_);

    renderer->UpdateShaderResourceData(shader_, "uView", Data::Create(glm::value_ptr(camera_->matrix()), sizeof(glm::mat4)));
    renderer->UpdateShaderResourceTexture(shader_, "uTexture", texture_);
    renderer->UpdateShaderResourceSampler(shader_, "uTexture", sampler_);

    for (auto tuple : mesh_) {
      auto id = eastl::get<0>(tuple);
      auto state = eastl::get<1>(tuple);
      renderer->ApplyMesh(id);
      renderer->Draw(state);
    }

    renderer->Render();
  }

 private:
  CameraUniquePtr camera_;
  eastl::chrono::time_point<eastl::chrono::high_resolution_clock> start_time_;
  ResourceID shader_{kInvalidResourceID};
  ResourceID sampler_{kInvalidResourceID};
  ResourceID texture_{kInvalidResourceID};
  ResourceID pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
  eastl::vector<eastl::tuple<ResourceID, DrawCallState>> mesh_;
};

XENGINE_WINDOW_APPLICATION(MeshSample)
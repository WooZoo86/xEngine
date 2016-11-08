#include "application/ApplicationDelegate.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"

#include "GraphicsSample.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if X_OPENGL

static const char *vertex_shader =
  "#version 410 core\n"
  "in vec2 aPosition;\n"
  "in vec2 aTexcoord0;\n"
  "in vec3 aColor0;\n"
  "out vec2 Texcoord;\n"
  "out vec3 Color;\n"
  "void main() {\n"
  "    Texcoord = aTexcoord0;\n"
  "    Color = aColor0;\n"
  "    gl_Position = vec4(aPosition, 0, 1);\n"
  "}\n";

static const char *fragment_shader =
  "#version 410 core\n"
  "in vec2 Texcoord;\n"
  "in vec3 Color;\n"
  "uniform sampler2D uTexture;\n"
  "out vec4 outColor;\n"
  "void main() {\n"
  "    outColor = texture(uTexture, Texcoord) * vec4(Color, 1);\n"
  "}\n";

#elif X_D3D11

static const char *vertex_shader =
  "struct VS_INPUT\n"
  "{\n"
  "    float2 aPosition: POSITION;\n"
  "    float2 aTexcoord0: TEXCOORD;\n"
  "    float3 aColor0: COLOR;\n"
  "};\n"
  "struct VS_OUTPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "    float2 Texcoord: TEXCOORD;\n"
  "    float3 Color: COLOR;\n"
  "};\n"
  "VS_OUTPUT main(const VS_INPUT input)\n"
  "{\n"
  "    VS_OUTPUT output;\n"
  "    output.Texcoord = input.aTexcoord0;\n"
  "    output.Color = input.aColor0;\n"
  "    output.Position = float4(input.aPosition, 0.0, 1.0);\n"
  "    return output;\n"
  "}\n";

static const char *fragment_shader =
  "Texture2D uTexture;\n"
  "SamplerState uTexture_sampler;\n"
  "struct PS_INPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "    float2 Texcoord: TEXCOORD;\n"
  "    float3 Color: COLOR;\n"
  "};\n"
  "float4 main(const PS_INPUT input): SV_TARGET\n"
  "{\n"
  "    return uTexture.Sample(uTexture_sampler, input.Texcoord) * float4(input.Color, 1.0);\n"
  "}\n";

#endif

using namespace xEngine;

class GraphicsSample : public ApplicationDelegate, WindowDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, 1024, 768, "GraphicsSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

    load_shader();
    load_texture();
    load_mesh();
    state_ = ClearState::ClearColor();
  }

  virtual void Finalize() override {
    Window::GetInstance().Finalize();
  }

  virtual void OnWindowClose() override {
    Window::GetInstance().GetGraphics(window_id_)->Finalize();
    Window::GetInstance().Destroy(window_id_);
    window_id_ = kInvalidResourceID;
  }

  virtual void OnWindowUpdate() override {
    draw();
  }

 private:
  void load_shader() {
    auto shader_config = ShaderConfig::FromData(Data::Create(vertex_shader, strlen(vertex_shader) + 1), Data::Create(fragment_shader, strlen(fragment_shader) + 1));
    shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config);
  }

  void load_texture() {
    auto data = Data::Create(reinterpret_cast<const char *>(test_jpg), test_jpg_len);
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
      auto pixel_data = Data::Create(reinterpret_cast<const char *>(buffer), static_cast<size_t>(width * height * 4));
      stbi_image_free(buffer);
      TextureConfig config;
      config.width = width;
      config.height = height;
      config.color_format = PixelFormat::RGBA8;
      config.data[0][0] = pixel_data;
      texture_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(config);
      sampler_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(SamplerConfig());
    }
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
    mesh_config.index_data = Data::Create(indices, sizeof(indices));
    mesh_config.vertex_count = 4;
    mesh_config.vertex_data = Data::Create(vertices, sizeof(vertices));
    mesh_config.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat2)
        .AddElement(VertexElementSemantic::kColor0, VertexElementFormat::kFloat3)
        .AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
    mesh_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh_config);

    auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, mesh_config.layout);
    pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
  }

  void draw() {
    if (window_id_ != kInvalidResourceID) {
      state_.clear_color += Color(0.01f, 0.005f, 0.0025f, 0.0f);
      state_.clear_color = glm::mod(state_.clear_color, 1.0f);
      auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
      renderer->Reset();
      renderer->ApplyTarget(kInvalidResourceID, state_);
      renderer->ApplyShader(shader_);
      renderer->ApplyMesh(mesh_);
      renderer->ApplyPipeline(pipeline_);
      renderer->UpdateShaderUniformTexture(shader_, "uTexture", texture_);
      renderer->ApplySampler(shader_, "uTexture", sampler_);
      renderer->DrawTopology(VertexTopology::kTriangles, 0, 6);
      renderer->Render();
    }
  }

 private:
  ResourceID shader_{kInvalidResourceID};
  ResourceID texture_{kInvalidResourceID};
  ResourceID sampler_{kInvalidResourceID};
  ResourceID mesh_{kInvalidResourceID};
  ResourceID pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
  ClearState state_;
};

XENGINE_WINDOW_APPLICATION(GraphicsSample)
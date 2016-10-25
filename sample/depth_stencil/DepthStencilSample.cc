#include "application/Application.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"

#include "../graphics/GraphicsSample.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <gtc/matrix_transform.hpp>

#include <EASTL/chrono.h>

#if X_OPENGL

static const char *vertex_shader =
  "#version 410 core\n"
  "in vec3 aPosition;\n"
  "in vec2 aTexcoord0;\n"
  "out vec2 Texcoord;\n"
  "uniform mat4 uModel;\n"
  "uniform mat4 uView;\n"
  "uniform mat4 uProjection;\n"
  "void main()\n"
  "{\n"
  "    Texcoord = aTexcoord0;\n"
  "    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);\n"
  "}\n";

static const char *fragment_shader =
  "#version 410 core\n"
  "in vec2 Texcoord;\n"
  "out vec4 outColor;\n"
  "uniform sampler2D uTexture;\n"
  "uniform vec3 uColor;\n"
  "void main()\n"
  "{\n"
  "    outColor = vec4(uColor, 1.0) * texture(uTexture, Texcoord);\n"
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

class DepthStencilSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    Window::GetInstance().Initialize();
    window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(1024, 768, "DepthStencilSample"));
    Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
    Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

    start_time_ = eastl::chrono::high_resolution_clock::now();

    load_shader();
    load_sampler();
    load_texture();
    load_mesh();

    return Application::Initialize();
  }

  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    Window::GetInstance().Finalize();
    return Application::Finalize();
  }

  virtual ApplicationStatus Loop() override {
    Window::GetInstance().PollEvent();

    draw();

    if (Window::GetInstance().ShouldClose(window_id_)) {
      Window::GetInstance().GetGraphics(window_id_)->Finalize();
      Window::GetInstance().Destroy(window_id_);
      window_id_ = kInvalidResourceID;
    }

    return Window::GetInstance().IsAllClosed() ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  void load_shader() {
    auto shader_config = ShaderConfig::FromData(vertex_shader, fragment_shader);
    shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config);
    Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyShader(shader_);
    auto view = glm::lookAt(
        glm::vec3(2.5f, 2.5f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
    Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderUniformData(shader_, "uView", view);
    auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderUniformData(shader_, "uProjection", projection);
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

  void load_mesh() {
    float32 vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.5f, 1.0f, 0.0f,
         1.0f,  1.0f, -0.5f, 1.0f, 1.0f,
         1.0f,  1.0f, -0.5f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f
    };
    MeshConfig mesh_config;
    mesh_config.vertex_count = 42;
    mesh_config.vertex_data = vertices;
    mesh_config.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3)
        .AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
    mesh_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh_config);

    auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, mesh_config.layout);
    pipeline_config.depth_stencil_state.depth_enable = true;
    pipeline_config.depth_stencil_state.depth_mask = true;
    cube_pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);

    pipeline_config.depth_stencil_state.depth_enable = true;
    pipeline_config.depth_stencil_state.depth_mask = false;
    pipeline_config.depth_stencil_state.stencil_enable = true;
    pipeline_config.depth_stencil_state.front.read_mask = 0xff;
    pipeline_config.depth_stencil_state.front.write_mask = 0xff;
    pipeline_config.depth_stencil_state.front.stencil_value = 1;
    pipeline_config.depth_stencil_state.front.compare = CompareFunction::kAlways;
    pipeline_config.depth_stencil_state.front.fail = StencilOperation::kKeep;
    pipeline_config.depth_stencil_state.front.depth_fail = StencilOperation::kKeep;
    pipeline_config.depth_stencil_state.front.pass = StencilOperation::kReplace;
    pipeline_config.depth_stencil_state.back = pipeline_config.depth_stencil_state.front;
    floor_pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);

    pipeline_config.depth_stencil_state.depth_enable = true;
    pipeline_config.depth_stencil_state.depth_mask = true;
    pipeline_config.depth_stencil_state.stencil_enable = true;
    pipeline_config.depth_stencil_state.front.read_mask = 0xff;
    pipeline_config.depth_stencil_state.front.write_mask = 0x00;
    pipeline_config.depth_stencil_state.front.stencil_value = 1;
    pipeline_config.depth_stencil_state.front.compare = CompareFunction::kEqual;
    pipeline_config.depth_stencil_state.front.fail = StencilOperation::kKeep;
    pipeline_config.depth_stencil_state.front.depth_fail = StencilOperation::kKeep;
    pipeline_config.depth_stencil_state.front.pass = StencilOperation::kReplace;
    pipeline_config.depth_stencil_state.back = pipeline_config.depth_stencil_state.front;
    reflection_pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
  }

  void draw() {
    auto now = eastl::chrono::high_resolution_clock::now();
    auto time = eastl::chrono::duration_cast<eastl::chrono::duration<float>>(now - start_time_).count();

    glm::mat4 model;
    model = glm::rotate(
        model,
        time * glm::radians(180.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
    renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
    renderer->ApplyShader(shader_);
    renderer->ApplyMesh(mesh_);
    renderer->ApplySampler(sampler_, 0);
    renderer->ApplySampler(sampler_, 1);
    renderer->UpdateShaderUniformData(shader_, "uModel", model);
    renderer->UpdateShaderUniformTexture(shader_, "uTexture", texture_);

    renderer->ApplyPipeline(cube_pipeline_);
    renderer->UpdateShaderUniformData(shader_, "uColor", glm::f64vec4(1.0, 1.0, 1.0, 1.0));
    renderer->DrawTopology(VertexTopology::kTriangles, 0, 36);

    renderer->ApplyPipeline(floor_pipeline_);
    renderer->UpdateShaderUniformData(shader_, "uColor", glm::f64vec4(0.0, 0.0, 0.0, 1.0));
    renderer->DrawTopology(VertexTopology::kTriangles, 36, 6);


    renderer->ApplyPipeline(reflection_pipeline_);
    model = glm::scale(
        glm::translate(model, glm::vec3(0, 0, -1)),
        glm::vec3(1, 1, -1)
    );
    renderer->UpdateShaderUniformData(shader_, "uModel", model);
    renderer->UpdateShaderUniformData(shader_, "uColor", glm::f64vec4(0.3, 0.3, 0.3, 1.0));
    renderer->DrawTopology(VertexTopology::kTriangles, 0, 36);

    renderer->Render();
  }

 private:
  eastl::chrono::time_point<eastl::chrono::high_resolution_clock> start_time_;
  ResourceID shader_{kInvalidResourceID};
  ResourceID texture_{kInvalidResourceID};
  ResourceID sampler_{kInvalidResourceID};
  ResourceID mesh_{kInvalidResourceID};
  ResourceID cube_pipeline_{kInvalidResourceID};
  ResourceID floor_pipeline_{kInvalidResourceID};
  ResourceID reflection_pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
};

XENGINE_WINDOW_APPLICATION(DepthStencilSample)
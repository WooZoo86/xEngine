#include "application/ApplicationDelegate.h"
#include "core/Log.h"
#include "graphics/Graphics.h"
#include "window/Window.h"
#include "io/IO.h"
#include "storage/StorageFilesystem.h"
#include "asset/graphics/MeshLoader.h"

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
  "};\n"
  "struct VS_OUTPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "};\n"
  "VS_OUTPUT main(const VS_INPUT input)\n"
  "{\n"
  "    VS_OUTPUT output;\n"
  "    output.Position = mul(uProjection, mul(uView, mul(uModel, float4(input.aPosition, 1.0))));\n"
  "    return output;\n"
  "}\n";

static const char *fragment_shader =
  "struct PS_INPUT\n"
  "{\n"
  "    float4 Position: SV_POSITION;\n"
  "};\n"
  "float4 main(const PS_INPUT input): SV_TARGET\n"
  "{\n"
  "    return float4(1.0, 0.0, 0.0, 1.0);\n"
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

 private:
  void load_shader() {
    auto shader_config = ShaderConfig::FromData(Data::Create(vertex_shader, strlen(vertex_shader) + 1), Data::Create(fragment_shader, strlen(fragment_shader) + 1));
    shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config);
    Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyShader(shader_);
    auto view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderUniformData(shader_, "uView", Data::Create(glm::value_ptr(view), sizeof(view)));
    auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderUniformData(shader_, "uProjection", Data::Create(glm::value_ptr(projection), sizeof(projection)));
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
    pipeline_config.rasterizer_state.cull_face_enable = true;
    pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
  }

  void draw() {
    auto now = eastl::chrono::high_resolution_clock::now();
    auto time = eastl::chrono::duration_cast<eastl::chrono::duration<float>>(now - start_time_).count();

    auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();

    renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
    renderer->ApplyShader(shader_);
    renderer->ApplyPipeline(pipeline_);

    auto model = glm::mat4();
    model = glm::rotate(model, 0.5f * time * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    renderer->UpdateShaderUniformData(shader_, "uModel", Data::Create(glm::value_ptr(model), sizeof(model)));

    for (auto tuple : mesh_) {
      auto id = eastl::get<0>(tuple);
      auto state = eastl::get<1>(tuple);
      renderer->ApplyMesh(id);
      renderer->Draw(state);
    }

    renderer->Render();
  }

 private:
  eastl::chrono::time_point<eastl::chrono::high_resolution_clock> start_time_;
  ResourceID shader_{kInvalidResourceID};
  ResourceID pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
  eastl::vector<eastl::tuple<ResourceID, DrawCallState>> mesh_;
};

XENGINE_WINDOW_APPLICATION(MeshSample)
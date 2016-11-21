#include "MeshViewerWindow.h"

#include <window/Window.h>

#include <gtc/matrix_transform.hpp>

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

namespace xEngine {

static const auto window_width = 800;

static const auto window_height = 600;

MeshViewerWindow::MeshViewerWindow() {
  x_assert(Window::GetInstance().Available());
  window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, window_width, window_height, "MeshViewer"));
  Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
  Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();

  InitializeShader();
  InitializeCamera();
  InitializePipeline();
}

MeshViewerWindow::~MeshViewerWindow() {
  if (window_id_ != kInvalidResourceID) {
    MeshViewerWindow::OnWindowClose();
  }
}

void MeshViewerWindow::OnWindowUpdate() {
  auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();

  renderer->MakeCurrent();

  renderer->ApplyTarget(kInvalidResourceID, ClearState::ClearAll());
  renderer->ApplyShader(shader_);
  renderer->ApplyPipeline(pipeline_);

  renderer->UpdateShaderResourceData(shader_, "uProjection", Data::Create(glm::value_ptr(camera_->projection_matrix()), sizeof(glm::mat4)));
  renderer->UpdateShaderResourceData(shader_, "uView", Data::Create(glm::value_ptr(camera_->view_matrix()), sizeof(glm::mat4)));

  for (auto tuple : mesh_) {
    auto id = eastl::get<0>(tuple);
    auto state = eastl::get<1>(tuple);
    renderer->ApplyMesh(id);
    renderer->Draw(state);
  }

  renderer->Render();
}

void MeshViewerWindow::OnWindowMousePosition(const glm::vec2 &position) {
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

void MeshViewerWindow::OnWindowMouseScroll(const glm::vec2 &offset) {
  camera_->set_zoom(camera_->zoom() + offset.y / 10.0f);
}

void MeshViewerWindow::OnWindowClose() {
  Window::GetInstance().GetGraphics(window_id_)->Finalize();
  Window::GetInstance().Destroy(window_id_);
  window_id_ = kInvalidResourceID;
}

void MeshViewerWindow::Show(Context &context) {
  mesh_.clear();
  for (auto &mesh_info : context.meshes) {
    auto id = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(mesh_info.mesh_util.config());
    auto draw_call_state = DrawCallState::Triangles(mesh_info.mesh_util.config().index_count);
    mesh_.push_back(eastl::make_tuple(id, draw_call_state));
  }
}

void MeshViewerWindow::InitializeShader() {
  auto shader_config = ShaderConfig::FromString(vertex_shader, fragment_shader);
  shader_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config);
}

void MeshViewerWindow::InitializeCamera() {
  camera_ = Camera::CreateUnique();
  camera_->set_render_window(window_id_);
  camera_->set_position(glm::vec3(0.0f, 0.0f, 400.0f));
  camera_->set_target(glm::vec3(0.0f, 100.0f, 0.0f));
  camera_->set_up_direction(glm::vec3(0.0f, 1.0f, 0.0f));

  auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
  renderer->UpdateShaderResourceData(shader_, "uModel", Data::Create(glm::value_ptr(glm::mat4()), sizeof(glm::mat4)));
}

void MeshViewerWindow::InitializePipeline() {
  VertexLayout layout;
  layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3)
    .AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);

  auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, layout);
  pipeline_config.depth_stencil_state.depth_enable = true;
  pipeline_config.depth_stencil_state.depth_write_enable = true;
  pipeline_config.rasterizer_state.fill_mode = FillMode::kWireframe;
  pipeline_config.rasterizer_state.cull_face_enable = false;
  pipeline_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config);
}

}

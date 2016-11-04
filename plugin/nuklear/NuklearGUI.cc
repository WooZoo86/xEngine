#include "NuklearGUI.h"

#include <window/Window.h>

#define NK_IMPLEMENTATION
#include <nuklear.h>

#include <gtc/matrix_transform.hpp>

namespace xEngine {

#if X_OPENGL

static const char *vertex_shader =
    "#version 410 core\n"
    "uniform mat4 uProjectionMatrix;\n"
    "in vec2 aPosition;\n"
    "in vec2 aTexcoord0;\n"
    "in vec4 aColor0;\n"
    "out vec2 Texcoord;\n"
    "out vec4 Color;\n"
    "void main() {\n"
    "   Texcoord = aTexcoord0;\n"
    "   Color = aColor0;\n"
    "   gl_Position = uProjectionMatrix * vec4(aPosition, 0, 1);\n"
    "}\n";

static const char *fragment_shader =
    "#version 410 core\n"
    "precision mediump float;\n"
    "uniform sampler2D uTexture;\n"
    "in vec2 Texcoord;\n"
    "in vec4 Color;\n"
    "out vec4 outColor;\n"
    "void main(){\n"
    "   outColor = Color * texture(uTexture, Texcoord);\n"
    "}\n";

#elif X_D3D11

static const char *vertex_shader = 
    "float4x4 uProjectionMatrix;\n"
    "struct VS_INPUT\n"
    "{\n"
    "  float2 position : POSITION;\n"
    "  float4 color : COLOR0;\n"
    "  float2 texcoord : TEXCOORD0;\n"
    "};\n"
    "struct VS_OUPUT\n"
    "{\n"
    "  float4 position : SV_POSITION;\n"
    "  float4 color : COLOR0;\n"
    "  float2 texcoord : TEXCOORD0;\n"
    "};\n"
    "VS_OUPUT main(VS_INPUT input)\n"
    "{\n"
    "  VS_OUPUT output;\n"
    "  output.position = mul(uProjectionMatrix, float4(input.position.xy, 0.f, 1.f));\n"
    "  output.color = input.color;\n"
    "  output.texcoord  = input.texcoord;\n"
    "  return output;\n"
    "}\n";

const char *fragment_shader =
    "Texture2D uTexture;\n"
    "SamplerState uTexture_sampler;\n"
    "struct PS_INPUT\n"
    "{\n"
    "  float4 position : SV_POSITION;\n"
    "  float4 color : COLOR0;\n"
    "  float2 texcoord : TEXCOORD0;\n"
    "};\n"
    "float4 main(PS_INPUT input) : SV_Target\n"
    "{\n"
    "  return input.color * uTexture.Sample(uTexture_sampler, input.texcoord);\n"
    "}\n";

#endif

struct vertex_struct {
  float32 position[2];
  float32 texcoord[2];
  nk_byte color[4];
};

void NuklearGUI::Initialize(NuklearConfig config) {
  x_assert(!Available());

  gui_config_ = config;

  x_assert(gui_config_.window != kInvalidResourceID);
  window_ = Window::GetInstance().Get(gui_config_.window).get();
  x_assert(window_ && window_->graphics());
  graphics_ = window_->graphics().get();

  static const nk_draw_vertex_layout_element vertex_layout[] = {
      {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, offsetof(vertex_struct, position)},
      {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, offsetof(vertex_struct, texcoord)},
      {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, offsetof(vertex_struct, color)},
      {NK_VERTEX_LAYOUT_END}
  };

  memset(&config_, 0, sizeof(config_));
  config_.global_alpha = gui_config_.global_alpha;
  config_.line_AA = gui_config_.line_AA ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  config_.shape_AA = gui_config_.shape_AA ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  config_.circle_segment_count = gui_config_.circle_segment_count;
  config_.arc_segment_count = gui_config_.arc_segment_count;
  config_.curve_segment_count = gui_config_.curve_segment_count;
  config_.vertex_layout = vertex_layout;
  config_.vertex_size = sizeof(vertex_struct);
  config_.vertex_alignment = NK_ALIGNOF(vertex_struct);

  nk_init_default(&context_, nullptr);

  nk_font_atlas_init_default(&font_atlas_);
  nk_font_atlas_begin(&font_atlas_);
  TextureConfig texture_config;
  texture_config.color_format = PixelFormat::RGBA8;
  auto image_data =
      nk_font_atlas_bake(&font_atlas_, &texture_config.width, &texture_config.height, NK_FONT_ATLAS_RGBA32);
  texture_config.data[0][0] = Data::Create(reinterpret_cast<const char *>(image_data),
                                           static_cast<size_t>(texture_config.width * texture_config.height * 4));
  auto image_id = graphics_->resource_manager()->Create(texture_config);
  image_.push_back(image_id);
  auto image = nk_image_id(static_cast<int32>(image_.size() - 1));
  nk_font_atlas_end(&font_atlas_, image.handle, &config_.null);

  nk_style_set_font(&context_, &font_atlas_.default_font->handle);

  sampler_ = graphics_->resource_manager()->Create(SamplerConfig());

  auto shader_config = ShaderConfig::FromData(Data::Create(vertex_shader, strlen(vertex_shader) + 1), Data::Create(fragment_shader, strlen(fragment_shader) + 1));
  shader_ = graphics_->resource_manager()->Create(shader_config);

  MeshConfig mesh_config;
  mesh_config.index_usage = BufferUsage::kStream;
  mesh_config.index_type = IndexFormat::kUint16;
  mesh_config.index_count = gui_config_.max_index_count;
  mesh_config.vertex_usage = BufferUsage::kStream;
  mesh_config.vertex_count = gui_config_.max_vertex_count;
  mesh_config.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat2).
      AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2).
      AddElement(VertexElementSemantic::kColor0, VertexElementFormat::kUnsignedByte4Normalized);
  mesh_ = graphics_->resource_manager()->Create(mesh_config);

  auto pipeline_config = PipelineConfig::ShaderWithLayout(shader_, mesh_config.layout);

  pipeline_config.blend_state.enable = true;
  pipeline_config.blend_state.src_rgb_factor = BlendFactor::kSrcAlpha;
  pipeline_config.blend_state.dst_rgb_factor = BlendFactor::kOneMinusSrcAlpha;
  pipeline_config.blend_state.color_format = window_->graphics()->config().color_format;
  pipeline_config.blend_state.depth_format = window_->graphics()->config().depth_format;
  pipeline_config.blend_state.color_mask = PixelChannel::kRGB;

  pipeline_config.depth_stencil_state.depth_enable = false;

  pipeline_config.rasterizer_state.scissor_test_enable = true;
  pipeline_config.rasterizer_state.cull_face_enable = false;
  pipeline_config.rasterizer_state.sample = window_->graphics()->config().sample_count;

  pipeline_ = graphics_->resource_manager()->Create(pipeline_config);

  auto vertex_size = mesh_config.vertex_count * mesh_config.layout.size;
  auto index_size = mesh_config.index_count * SizeOfIndexFormat(mesh_config.index_type);

  vertex_data = Data::Create(vertex_size);
  index_data = Data::Create(index_size);

  nk_buffer_init_default(&command);
  nk_buffer_init_fixed(&vertex_buffer, vertex_data->buffer(), vertex_data->size());
  nk_buffer_init_fixed(&index_buffer, index_data->buffer(), index_data->size());
}

void NuklearGUI::Finalize() {
  x_assert(Available());
  graphics_->resource_manager()->Destroy(sampler_);
  graphics_->resource_manager()->Destroy(shader_);
  graphics_->resource_manager()->Destroy(mesh_);
  graphics_->resource_manager()->Destroy(pipeline_);
  for (auto &id : image_) {
    graphics_->resource_manager()->Destroy(id);
  }
  image_.clear();
  nk_font_atlas_clear(&font_atlas_);
  nk_free(&context_);
  graphics_ = nullptr;
}

void NuklearGUI::BeginFrame() {
  nk_input_begin(&context_);
  auto &position = window_->GetMousePosition();
  auto x = static_cast<int32>(position.x);
  auto y = static_cast<int32>(position.y);
  nk_input_motion(&context_, x, y);
  nk_input_scroll(&context_, window_->GetMouseScroll().y);
  nk_input_button(&context_, NK_BUTTON_LEFT, x, y, window_->IsMouseButtonDown(MouseButtonType::kLeft) || window_->IsMouseButtonPressed(MouseButtonType::kLeft));
  nk_input_button(&context_, NK_BUTTON_RIGHT, x, y, window_->IsMouseButtonDown(MouseButtonType::kRight) || window_->IsMouseButtonPressed(MouseButtonType::kRight));
  nk_input_button(&context_, NK_BUTTON_MIDDLE, x, y, window_->IsMouseButtonDown(MouseButtonType::kMiddle) || window_->IsMouseButtonPressed(MouseButtonType::kMiddle));
  nk_input_end(&context_);
}

void NuklearGUI::EndFrame() {
  auto matrix = glm::ortho(0.0f,
                           static_cast<float32>(window_->config().width),
                           static_cast<float32>(window_->config().height),
                           0.0f,
                           -1.0f,
                           1.0f);
  auto scale_x = static_cast<float32>(window_->config().frame_buffer_width) / window_->config().width;
  auto scale_y = static_cast<float32>(window_->config().frame_buffer_height) / window_->config().height;
  nk_buffer_clear(&vertex_buffer);
  nk_buffer_clear(&index_buffer);
  nk_convert(&context_, &command, &vertex_buffer, &index_buffer, &config_);
  graphics_->renderer()->UpdateMesh(mesh_, vertex_data, index_data);
  graphics_->renderer()->UpdateShaderUniformData(shader_, "uProjectionMatrix", Data::Create(glm::value_ptr(matrix), sizeof(matrix)));
  graphics_->renderer()->ApplyPipeline(pipeline_);
  const struct nk_draw_command *cmd = nullptr;
  ResourceID current_texture = kInvalidResourceID;
  auto element_offset = 0;
  nk_draw_foreach(cmd, &context_, &command) {
    auto texture = image_[cmd->texture.id];
    if (current_texture != texture) {
      current_texture = texture;
      graphics_->renderer()->UpdateShaderUniformTexture(shader_, "uTexture", current_texture);
      graphics_->renderer()->ApplySampler(shader_, "uTexture", sampler_);
    }
    graphics_->renderer()->ApplyScissor(static_cast<int32>(cmd->clip_rect.x * scale_x),
                                        static_cast<int32>((window_->config().height - (cmd->clip_rect.y + cmd->clip_rect.h)) * scale_y),
                                        static_cast<int32>(cmd->clip_rect.w * scale_x),
                                        static_cast<int32>(cmd->clip_rect.h * scale_y));
    graphics_->renderer()->DrawTopology(VertexTopology::kTriangles, element_offset, cmd->elem_count);
    element_offset += cmd->elem_count;
  }
  nk_clear(&context_);
}

} // namespace xEngine
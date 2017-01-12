#include "Shader.h"

#include "window/Window.h"

namespace xEngine {

ShaderPtr Shader::Parse(ResourceID window, DataPtr data) {
  x_assert(window != kInvalidResourceID);
  
  auto shader = Shader::Create();
  shader->window_id_ = window;
  
  auto type = Window::GetInstance().GetGraphics(window)->config().type;

  // TODO 

  return shader;
}

void Shader::Initialize() {
  x_assert(shader_id_ == kInvalidResourceID);
  shader_id_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(shader_config_);
  pipeline_config_.shader = shader_id_;
  pipeline_id_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(pipeline_config_);
}

void Shader::Finalize() {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Destroy(shader_id_);
  Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Destroy(pipeline_id_);
}

void Shader::Apply() {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyPipeline(pipeline_id_);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyShader(shader_id_);
}

void Shader::UpdateResourceData(const eastl::string &name, DataPtr data) {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceData(shader_id_, name, data);
}

void Shader::UpdateResourceTexture(const eastl::string &name, ResourceID texture_id) {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceTexture(shader_id_, name, texture_id);
}

void Shader::UpdateResourceSampler(const eastl::string &name, ResourceID sampler_id) {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceSampler(shader_id_, name, sampler_id);
}

void Shader::UpdateResourceBlock(const eastl::string &name, ResourceID uniform_buffer_id) {
  x_assert(shader_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceBlock(shader_id_, name, uniform_buffer_id);
}

} // namespace xEngine
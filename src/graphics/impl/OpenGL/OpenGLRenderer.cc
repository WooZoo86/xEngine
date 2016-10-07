#if X_OPENGL

#include "OpenGLRenderer.h"
#include "graphics/impl/OpenGL/resource/OpenGLGraphicsResourceManager.h"

#include "window/Window.h"

namespace xEngine {

void OpenGLRenderer::PreOpenGLCallback(const char *name, void *function, int len_args, ...) {
  GLenum errorCode;
  errorCode = glad_glGetError();
  if (errorCode != GL_NO_ERROR) {
    Log::GetInstance().Error("[OpenGLRenderer::PreOpenGLCallback] ERROR 0x%04x in %s\n", errorCode, name);
    X_TRAP();
  }
}

void OpenGLRenderer::PostOpenGLCallback(const char *name, void *function, int len_args, ...) {
  GLenum errorCode;
  errorCode = glad_glGetError();
  if (errorCode != GL_NO_ERROR) {
    Log::GetInstance().Error("[OpenGLRenderer::PostOpenGLCallback] ERROR 0x%04x in %s\n", errorCode, name);
    X_TRAP();
  }
}

void OpenGLRenderer::Initialize(const GraphicsConfig &config) {
  config_ = config;
  window_ = Window::GetInstance().Get(config.window).get();
  resource_manager_ = static_cast<OpenGLGraphicsResourceManager *>(window_->graphics()->resource_manager().get());
  if (gladLoadGL() == 0) {
    x_error("OpenGL init error!\n");
  }
#ifdef GLAD_DEBUG
  glad_set_pre_callback(PreOpenGLCallback);
  glad_set_post_callback(PostOpenGLCallback);
#endif
  Reset();
}

void OpenGLRenderer::Finalize() {
  Reset();
  window_ = nullptr;
  resource_manager_ = nullptr;
  config_ = GraphicsConfig();
}

void OpenGLRenderer::Render() {

}

void OpenGLRenderer::ApplyTarget(ResourceID id) {
  if (id != kInvalidResourceID) {
    auto &texture = resource_manager_->texture_pool_.Find(id);
    if (texture.status() == ResourceStatus::kCompleted) {
      if (texture.texture_id != cache_.frame_buffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, texture.texture_id);
        ApplyViewPort(0, 0, texture.config().width, texture.config().height);
        cache_.frame_buffer = texture.texture_id;
      }
      cache_.prepared_mask |= OpenGLRendererCache::PrepareMask::kRenderTarget;
      return;
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  ApplyViewPort(0, 0, window_->config().frame_buffer_width, window_->config().frame_buffer_height);
  cache_.frame_buffer = 0;
  cache_.prepared_mask |= OpenGLRendererCache::PrepareMask::kRenderTarget;
}

void OpenGLRenderer::ApplyClearState(const ClearState &state) {
  if (cache_.rasterizer_state.scissor_test_enable) {
    cache_.rasterizer_state.scissor_test_enable = false;
    glDisable(GL_SCISSOR_TEST);
  }

  GLbitfield clear_mask = 0;

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kColor)) {
    clear_mask |= GL_COLOR_BUFFER_BIT;
    glClearColor(state.clear_color.r, state.clear_color.g, state.clear_color.b, state.clear_color.a);
    if (cache_.blend_state.color_mask != PixelChannel::kRGBA) {
      cache_.blend_state.color_mask = PixelChannel::kRGBA;
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kDepth)) {
    clear_mask |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(state.depth);
    if (!cache_.depth_stencil_state.depth_enable) {
      cache_.depth_stencil_state.depth_enable = true;
      glDepthMask(GL_TRUE);
    }
  }

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kStencil)) {
    clear_mask |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(state.stencil);
    if (cache_.depth_stencil_state.stencil_write_mask != 0xff) {
      cache_.depth_stencil_state.stencil_write_mask = 0xff;
      glStencilMask(0xff);
    }
  }

  if (clear_mask != 0) {
    glClear(clear_mask);
  }
}

void OpenGLRenderer::ApplyBlendState(const BlendState &blend_state) {
  if (blend_state.blend_color != cache_.blend_color) {
    cache_.blend_color = blend_state.blend_color;
    glBlendColor(blend_state.blend_color.r,
                 blend_state.blend_color.g,
                 blend_state.blend_color.b,
                 blend_state.blend_color.a);
  }
  if (blend_state.enable != cache_.blend_state.enable) {
    if (blend_state.enable) {
      glEnable(GL_BLEND);
    } else {
      glDisable(GL_BLEND);
    }
  }
  if (blend_state.src_rgb_factor != cache_.blend_state.src_rgb_factor ||
      blend_state.dst_rgb_factor != cache_.blend_state.dst_rgb_factor ||
      blend_state.src_alpha_factor != cache_.blend_state.src_alpha_factor ||
      blend_state.dst_alpha_factor != cache_.blend_state.dst_alpha_factor) {
    glBlendFuncSeparate(GLEnumForBlendFactor(blend_state.src_rgb_factor),
                        GLEnumForBlendFactor(blend_state.dst_rgb_factor),
                        GLEnumForBlendFactor(blend_state.src_alpha_factor),
                        GLEnumForBlendFactor(blend_state.dst_alpha_factor));
  }
  if (blend_state.rgb_operation != cache_.blend_state.rgb_operation ||
      blend_state.alpha_operation != cache_.blend_state.alpha_operation) {
    glBlendEquationSeparate(GLEnumForBlendOperation(blend_state.rgb_operation),
                            GLEnumForBlendOperation(blend_state.alpha_operation));
  }
  if (blend_state.color_mask != cache_.blend_state.color_mask) {
    glColorMask(static_cast<uint8>(blend_state.color_mask) & static_cast<uint8>(PixelChannel::kRed),
                static_cast<uint8>(blend_state.color_mask) & static_cast<uint8>(PixelChannel::kGreen),
                static_cast<uint8>(blend_state.color_mask) & static_cast<uint8>(PixelChannel::kBlue),
                static_cast<uint8>(blend_state.color_mask) & static_cast<uint8>(PixelChannel::kAlpha));
  }
  cache_.blend_state = blend_state;
}

void OpenGLRenderer::ResetBlendState() {
  cache_.blend_state = BlendState();
  glDisable(GL_BLEND);
  glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  cache_.blend_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
  glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLRenderer::ApplyDepthStencilState(const DepthStencilState &depth_stencil_state) {
  if (cache_.depth_stencil_state != depth_stencil_state) {
    if (cache_.depth_stencil_state.value != depth_stencil_state.value) {
      if (depth_stencil_state.compare != cache_.depth_stencil_state.compare) {
        glDepthFunc(GLEnumForCompareFunction(depth_stencil_state.compare));
      }
      if (depth_stencil_state.depth_enable != cache_.depth_stencil_state.depth_enable) {
        glDepthMask(static_cast<GLboolean>(depth_stencil_state.depth_enable));
      }
      if (depth_stencil_state.stencil_enable != cache_.depth_stencil_state.stencil_enable) {
        if (depth_stencil_state.stencil_enable) {
          glEnable(GL_STENCIL_TEST);
        } else {
          glDisable(GL_STENCIL_TEST);
        }
      }
    }
    if (cache_.depth_stencil_state.front != depth_stencil_state.front) {
      if (depth_stencil_state.front.compare != cache_.depth_stencil_state.compare ||
          depth_stencil_state.stencil_value != cache_.depth_stencil_state.stencil_value ||
          depth_stencil_state.stencil_read_mask != cache_.depth_stencil_state.stencil_read_mask) {
        glStencilFuncSeparate(GL_FRONT,
                              GLEnumForCompareFunction(depth_stencil_state.front.compare),
                              depth_stencil_state.stencil_value,
                              depth_stencil_state.stencil_read_mask);
      }
      if (depth_stencil_state.front.compare == cache_.depth_stencil_state.compare) {
        glStencilOpSeparate(GL_FRONT,
                            GLEnumForStencilOperation(depth_stencil_state.front.fail),
                            GLEnumForStencilOperation(depth_stencil_state.front.depth_fail),
                            GLEnumForStencilOperation(depth_stencil_state.front.pass));
      }
      if (depth_stencil_state.stencil_write_mask != cache_.depth_stencil_state.stencil_write_mask) {
        glStencilMaskSeparate(GL_FRONT, depth_stencil_state.stencil_write_mask);
      }
    }
    if (cache_.depth_stencil_state.back != depth_stencil_state.back) {
      if (depth_stencil_state.back.compare != cache_.depth_stencil_state.compare ||
          depth_stencil_state.stencil_value != cache_.depth_stencil_state.stencil_value ||
          depth_stencil_state.stencil_read_mask != cache_.depth_stencil_state.stencil_read_mask) {
        glStencilFuncSeparate(GL_BACK,
                              GLEnumForCompareFunction(depth_stencil_state.back.compare),
                              depth_stencil_state.stencil_value,
                              depth_stencil_state.stencil_read_mask);
      }
      if (depth_stencil_state.back.compare == cache_.depth_stencil_state.compare) {
        glStencilOpSeparate(GL_BACK,
                            GLEnumForStencilOperation(depth_stencil_state.back.fail),
                            GLEnumForStencilOperation(depth_stencil_state.back.depth_fail),
                            GLEnumForStencilOperation(depth_stencil_state.back.pass));
      }
      if (depth_stencil_state.stencil_write_mask != cache_.depth_stencil_state.stencil_write_mask) {
        glStencilMaskSeparate(GL_BACK, depth_stencil_state.stencil_write_mask);
      }
    }
    cache_.depth_stencil_state = depth_stencil_state;
  }
}

void OpenGLRenderer::ResetDepthStencilState() {
  cache_.depth_stencil_state = DepthStencilState();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  glDisable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilMask(0xFFFFFFFF);
}

void OpenGLRenderer::ApplyRasterizerState(const RasterizerState &rasterizer_state) {
  if (cache_.rasterizer_state != rasterizer_state) {
    auto enable_mask = 0;
    auto disable_mask = 0;
    if (rasterizer_state.cull_face_enable) {
      enable_mask |= GL_CULL_FACE;
    } else {
      disable_mask |= GL_CULL_FACE;
    }
    if (rasterizer_state.depth_offset_enable) {
      enable_mask |= GL_POLYGON_OFFSET_FILL;
    } else {
      disable_mask |= GL_POLYGON_OFFSET_FILL;
    }
    if (rasterizer_state.scissor_test_enable) {
      enable_mask |= GL_SCISSOR_TEST;
    } else {
      disable_mask |= GL_SCISSOR_TEST;
    }
    if (rasterizer_state.dither_enable) {
      enable_mask |= GL_DITHER;
    } else {
      disable_mask |= GL_DITHER;
    }
    if (rasterizer_state.alpha_to_coverage_enable) {
      enable_mask |= GL_SAMPLE_ALPHA_TO_COVERAGE;
    } else {
      disable_mask |= GL_SAMPLE_ALPHA_TO_COVERAGE;
    }
    if (rasterizer_state.sample > 1) {
      enable_mask |= GL_MULTISAMPLE;
    } else {
      disable_mask |= GL_MULTISAMPLE;
    }

    glDisable(static_cast<GLboolean>(disable_mask));

    glEnable(static_cast<GLboolean>(enable_mask));

    if (rasterizer_state.cull_face != cache_.rasterizer_state.cull_face) {
      if (rasterizer_state.cull_face_enable) {
        glCullFace(GLEnumForFaceSide(rasterizer_state.cull_face));
      }
    }

    if (rasterizer_state.front_face != cache_.rasterizer_state.front_face) {
      glFrontFace(GLEnumForFrontFaceType(rasterizer_state.front_face));
    }

    cache_.rasterizer_state = rasterizer_state;
  }
}

void OpenGLRenderer::ResetRasterizerState() {
  cache_.rasterizer_state = RasterizerState();
  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_SCISSOR_TEST);
  glEnable(GL_DITHER);
  glEnable(GL_MULTISAMPLE);
}

void OpenGLRenderer::ApplyViewPort(int32 x, int32 y, int32 width, int32 height) {
  if (cache_.viewport_x != x ||
      cache_.viewport_y != y ||
      cache_.viewport_width != width ||
      cache_.viewport_height != height) {
    cache_.viewport_x = x;
    cache_.viewport_y = y;
    cache_.viewport_width = width;
    cache_.viewport_height = height;
    glViewport(x, y, width, height);
  }
}

void OpenGLRenderer::ApplyScissor(int32 x, int32 y, int32 width, int32 height) {
  if (cache_.scissor_x != x ||
      cache_.scissor_y != y ||
      cache_.scissor_width != width ||
      cache_.scissor_height != height) {
    cache_.scissor_x = x;
    cache_.scissor_y = y;
    cache_.scissor_width = width;
    cache_.scissor_height = height;
    glScissor(x, y, width, height);
  }
}

void OpenGLRenderer::ApplyShader(ResourceID id) {
  auto &shader = resource_manager_->shader_pool_.Find(id);
  if (shader.status() == ResourceStatus::kCompleted) {
    if (shader.program_id != cache_.shader.program_id) {
      glUseProgram(shader.program_id);
      cache_.shader = shader;
      UpdateVertexAttributePointer(false);
    }
    cache_.prepared_mask |= OpenGLRendererCache::PrepareMask::kShader;
  }
}

void OpenGLRenderer::UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) {
  auto &shader = resource_manager_->shader_pool_.Find(id);
  if (shader.status() == ResourceStatus::kCompleted) {
    if (shader.program_id != cache_.shader.program_id) {
      glUseProgram(shader.program_id);
      cache_.shader = shader;
      UpdateVertexAttributePointer(false);
    }
    cache_.prepared_mask |= OpenGLRendererCache::PrepareMask::kShader;
    auto pair = shader.uniform_info.find(name);
    if (pair != shader.uniform_info.end()) {
      auto &info = pair->second;
      switch (format) {
        case UniformFormat::kInt:
        case UniformFormat::kTexture:
        case UniformFormat::kBool:
          glUniform1iv(info.location, 1, reinterpret_cast<const GLint *>(buffer));
          break;
        case UniformFormat::kVector1:
          glUniform1fv(info.location, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector2:
          glUniform2fv(info.location, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector3:
          glUniform3fv(info.location, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector4:
          glUniform4fv(info.location, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix2:
          glUniformMatrix2fv(info.location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix3:
          glUniformMatrix3fv(info.location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix4:
          glUniformMatrix4fv(info.location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
          break;
        default:
          break;
      }
    }
  }
}

void OpenGLRenderer::ResetShader() {
  cache_.shader = OpenGLShader();
  glUseProgram(0);
}

void OpenGLRenderer::ApplyVertexData(ResourceID id, bool force_update) {
  auto &vertex_data = resource_manager_->vertex_data_pool_.Find(id);
  if (vertex_data.status() == ResourceStatus::kCompleted) {
    if (vertex_data.buffer_id != cache_.vertex_buffer) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_data.buffer_id);
      glBindVertexArray(vertex_data.array_object_id);
      cache_.vertex_buffer = vertex_data.buffer_id;
      cache_.vertex_resource_id = id;
      UpdateVertexAttributePointer(force_update);
    }
    cache_.prepared_mask |= OpenGLRendererCache::PrepareMask::kVertexBuffer;
  }
}

void OpenGLRenderer::UpdateVertexData(ResourceID id, int32 offset, DataPtr data) {
  auto &vertex_data = resource_manager_->vertex_data_pool_.Find(id);
  if (vertex_data.id() != kInvalidResourceID) {
    if (vertex_data.status() == ResourceStatus::kCompleted) {
      if (vertex_data.buffer_id != cache_.vertex_buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_data.buffer_id);
        glBindVertexArray(vertex_data.array_object_id);
        cache_.vertex_buffer = vertex_data.buffer_id;
        cache_.vertex_resource_id = id;
        UpdateVertexAttributePointer(false);
      }
      glBufferSubData(GL_ARRAY_BUFFER, offset, data->size(), data->buffer());
    }
  }
}

void OpenGLRenderer::ResetVertexData() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  cache_.vertex_buffer = 0;
  cache_.vertex_resource_id = kInvalidResourceID;
}

void OpenGLRenderer::ApplyIndexData(ResourceID id) {
  auto &index_data = resource_manager_->index_data_pool_.Find(id);
  if (index_data.status() == ResourceStatus::kCompleted) {
    if (index_data.buffer_id != cache_.index_buffer) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_data.buffer_id);
      cache_.index_buffer = index_data.buffer_id;
      cache_.index_format = index_data.config().type;
    }
  }
}

void OpenGLRenderer::UpdateIndexData(ResourceID id, int32 offset, DataPtr data) {
  auto &index_data = resource_manager_->index_data_pool_.Find(id);
  if (index_data.id() != kInvalidResourceID) {
    if (index_data.status() == ResourceStatus::kCompleted) {
      if (index_data.buffer_id != cache_.index_buffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_data.buffer_id);
        cache_.index_buffer = index_data.buffer_id;
        cache_.index_format = index_data.config().type;
      }
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, data->size(), data->buffer());
    }
  }
}

void OpenGLRenderer::ResetIndexData() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  cache_.index_buffer = 0;
  cache_.index_format = IndexFormat::kNone;
}

void OpenGLRenderer::ApplyTexture(ResourceID id, int32 index) {
  auto &texture = resource_manager_->texture_pool_.Find(id);
  if (texture.status() == ResourceStatus::kCompleted) {
    if (texture.config().type == TextureType::kTexture2d) {
      if (texture.texture_id != cache_.texture_2d[index]) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + index));
        glBindTexture(GL_TEXTURE_2D, texture.texture_id);
        cache_.texture_2d[index] = texture.texture_id;
      }
    } else if (texture.config().type == TextureType::kTextureCube) {
      if (texture.texture_id != cache_.texture_cube[index]) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + index));
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture.texture_id);
        cache_.texture_cube[index] = texture.texture_id;
      }
    }
  }
}

void OpenGLRenderer::ResetTexture() {
  for (auto index = 0; index < static_cast<uint16>(OpenGLMaxDefine::kMaxTextureCount); ++index) {
    if (cache_.texture_2d[index] != 0) {
      glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + index));
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (cache_.texture_cube[index] != 0) {
      glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + index));
      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
  }
  memset(cache_.texture_2d, 0, sizeof(cache_.texture_2d));
  memset(cache_.texture_cube, 0, sizeof(cache_.texture_cube));
}

void OpenGLRenderer::DrawTopology(VertexTopology topology, int32 first, int32 count) {
  if (cache_.prepared_mask == OpenGLRendererCache::PrepareMask::kPrepared) {
    if (cache_.index_format == IndexFormat::kNone) {
      glDrawArrays(GLEnumForVertexTopology(topology), first, count);
    } else {
      glDrawElements(GLEnumForVertexTopology(topology),
                     count,
                     GLEnumForIndexFormat(cache_.index_format),
                     reinterpret_cast<GLvoid *>(first * SizeOfIndexFormat(cache_.index_format)));
    }
  }
  cache_.prepared_mask = 0;
}

void OpenGLRenderer::Reset() {
  ResetBlendState();
  ResetDepthStencilState();
  ResetRasterizerState();
  ResetVertexData();
  ResetIndexData();
  ResetShader();
  ResetTexture();
}

void OpenGLRenderer::UpdateVertexAttributePointer(bool force_update) {
  if (cache_.shader.id() == kInvalidResourceID || cache_.vertex_resource_id == kInvalidResourceID) {
    return;
  }
  auto &vertex_data = resource_manager_->vertex_data_pool_.Find(cache_.vertex_resource_id);
  if (vertex_data.id() == kInvalidResourceID) {
    return;
  }
  if (!force_update && vertex_data.vertex_attribute_pointed) {
    return;
  }
  auto &config = vertex_data.config();
  for (auto index = 0; index < static_cast<uint16>(GraphicsMaxDefine::kMaxVertexElementCount); ++index) {
    auto &element = config.element[index];
    if (element.type != VertexElementType::kInvalid) {
      eastl::string attribute_name = AttributeNameForVertexElementType(element.type);
      if (element.index > 0) {
        attribute_name += element.index;
      }
      auto pair = cache_.shader.attribute_info.find(attribute_name);
      if (pair == cache_.shader.attribute_info.end()) {
        continue;
      }
      auto &info = pair->second;
      glEnableVertexAttribArray(info.location);
      glVertexAttribPointer(info.location,
                            VertexCountForVertexElementFormat(element.format),
                            GLEnumForVertexElementFormat(element.format),
                            static_cast<GLboolean>(element.normalized),
                            config.vertex_size,
                            reinterpret_cast<GLvoid *>(element.offset));
    }
  }
  vertex_data.vertex_attribute_pointed = true;
}

} // namespace xEngine

#endif // X_OPENGL

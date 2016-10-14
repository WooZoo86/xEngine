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

void OpenGLRenderer::ApplyTarget(ResourceID id, const ClearState &state) {
  if (id != kInvalidResourceID) {
    auto &texture = resource_manager_->texture_pool_.Find(id);
    if (texture.status() == ResourceStatus::kCompleted) {
      if (texture.texture_id != cache_.frame_buffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, texture.texture_id);
        ApplyViewPort(0, 0, texture.config().width, texture.config().height);
        cache_.frame_buffer = texture.texture_id;
      }
    }
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ApplyViewPort(0, 0, window_->config().frame_buffer_width, window_->config().frame_buffer_height);
    cache_.frame_buffer = 0;
  }

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
  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  glDisable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilMask(0xFFFFFFFF);
}

void OpenGLRenderer::ApplyRasterizerState(const RasterizerState &rasterizer_state) {
  if (cache_.rasterizer_state != rasterizer_state) {
    if (rasterizer_state.cull_face_enable) {
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }
    if (rasterizer_state.depth_offset_enable) {
      glEnable(GL_POLYGON_OFFSET_FILL);
    } else {
      glDisable(GL_POLYGON_OFFSET_FILL);
    }
    if (rasterizer_state.scissor_test_enable) {
      glEnable(GL_SCISSOR_TEST);
    } else {
      glDisable(GL_SCISSOR_TEST);
    }
    if (rasterizer_state.dither_enable) {
      glEnable(GL_DITHER);
    } else {
      glDisable(GL_DITHER);
    }
    if (rasterizer_state.alpha_to_coverage_enable) {
      glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    } else {
      glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    }
    if (rasterizer_state.sample > 1) {
      glEnable(GL_MULTISAMPLE);
    } else {
      glDisable(GL_MULTISAMPLE);
    }

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

void OpenGLRenderer::ApplyShader(ResourceID id) {
  auto &shader = resource_manager_->shader_pool_.Find(id);
  if (shader.status() == ResourceStatus::kCompleted) {
    if (shader.program_id != cache_.shader.program_id) {
      glUseProgram(shader.program_id);
      cache_.shader = shader;
    }
  }
}

void OpenGLRenderer::UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) {
  auto &shader = resource_manager_->shader_pool_.Find(id);
  if (shader.status() == ResourceStatus::kCompleted) {
    if (shader.program_id != cache_.shader.program_id) {
      glUseProgram(shader.program_id);
      cache_.shader = shader;
    }
    auto pair = shader.uniform_location.find(name);
    if (pair != shader.uniform_location.end()) {
      switch (format) {
        case UniformFormat::kInt:
        case UniformFormat::kTexture:
        case UniformFormat::kBool:
          glUniform1iv(pair->second, 1, reinterpret_cast<const GLint *>(buffer));
          break;
        case UniformFormat::kVector1:
          glUniform1fv(pair->second, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector2:
          glUniform2fv(pair->second, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector3:
          glUniform3fv(pair->second, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kVector4:
          glUniform4fv(pair->second, 1, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix2:
          glUniformMatrix2fv(pair->second, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix3:
          glUniformMatrix3fv(pair->second, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
          break;
        case UniformFormat::kMatrix4:
          glUniformMatrix4fv(pair->second, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(buffer));
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

void OpenGLRenderer::ApplyMesh(ResourceID id) {
  auto &mesh = resource_manager_->mesh_pool_.Find(id);
  if (mesh.status() == ResourceStatus::kCompleted) {
    if (mesh.vertex_buffer_id != cache_.vertex_buffer) {
      glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer_id);
      cache_.vertex_buffer = mesh.vertex_buffer_id;
    }
    if (mesh.vertex_array_id != cache_.vertex_array) {
      glBindVertexArray(mesh.vertex_array_id);
      cache_.vertex_array = mesh.vertex_array_id;
    }
    if (mesh.config().index_type != cache_.index_type) {
      cache_.index_type = mesh.config().index_type;
    }
  }
}

void OpenGLRenderer::UpdateMesh(ResourceID id,
                                const void *vertex_buffer,
                                size_t vertex_offset,
                                size_t vertex_size,
                                const void *index_buffer,
                                size_t index_offset,
                                size_t index_size) {
  auto &mesh = resource_manager_->mesh_pool_.Find(id);
  if (mesh.status() == ResourceStatus::kCompleted) {
    if (mesh.vertex_buffer_id != cache_.vertex_buffer) {
      glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer_id);
      cache_.vertex_buffer = mesh.vertex_buffer_id;
    }
    if (vertex_buffer != nullptr && vertex_size != 0) {
      glBufferSubData(GL_ARRAY_BUFFER, vertex_offset, vertex_size, vertex_buffer);
    }
    if (mesh.vertex_array_id != cache_.vertex_array) {
      glBindVertexArray(mesh.vertex_array_id);
      cache_.vertex_array = mesh.vertex_array_id;
    }
    if (mesh.config().index_type != cache_.index_type) {
      cache_.index_type = mesh.config().index_type;
    }
    if (index_buffer != nullptr && index_size != 0) {
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_offset, index_size, index_buffer);
    }
  }
}

void OpenGLRenderer::ResetMesh() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  cache_.vertex_buffer = 0;
  cache_.index_type = IndexFormat::kNone;
}

void OpenGLRenderer::DrawTopology(VertexTopology topology, int32 first, int32 count) {
  if (cache_.index_type == IndexFormat::kNone) {
    glDrawArrays(GLEnumForVertexTopology(topology), first, count);
  } else {
    glDrawElements(GLEnumForVertexTopology(topology),
                   count,
                   GLEnumForIndexFormat(cache_.index_type),
                   reinterpret_cast<GLvoid *>(first * SizeOfIndexFormat(cache_.index_type)));
  }
}

void OpenGLRenderer::Reset() {
  ResetBlendState();
  ResetDepthStencilState();
  ResetRasterizerState();
  ResetShader();
  ResetTexture();
  ResetMesh();
}

} // namespace xEngine

#endif // X_OPENGL

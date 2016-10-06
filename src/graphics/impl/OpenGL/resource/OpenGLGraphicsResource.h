#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H

#if X_OPENGL

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/VertexDataConfig.h"
#include "graphics/config/IndexDataConfig.h"
#include "graphics/config/TextureConfig.h"

#include "resource/Resource.h"

#include <glad/glad.h>

#include <EASTL/hash_map.h>
#include <EASTL/string.h>

namespace xEngine {

struct OpenGLShader: public Resource<ShaderConfig> {

  struct Info {
    GLuint location{0};
    GLenum type{GL_FLOAT};
    GLsizei size{0};
  };

  GLuint program_id{0};

  eastl::hash_map<eastl::string, Info> attribute_info;

  eastl::hash_map<eastl::string, Info> uniform_info;

  virtual void Reset() override {
    program_id = 0;
    attribute_info.clear();
    uniform_info.clear();
  }

};

struct OpenGLVertexData: public Resource<VertexDataConfig> {

  GLuint buffer_id{0};

  virtual void Reset() override {
    buffer_id = 0;
  }

};

struct OpenGLIndexData: public Resource<IndexDataConfig> {

  GLuint buffer_id{0};

  virtual void Reset() override {
    buffer_id = 0;
  }

};

struct OpenGLTexture: public Resource<TextureConfig> {

  GLuint texture_id{0};

  virtual void Reset() override {
    texture_id = 0;
  }

};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
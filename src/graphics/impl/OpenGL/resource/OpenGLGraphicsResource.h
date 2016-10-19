#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H

#if X_OPENGL

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/TextureConfig.h"
#include "graphics/config/MeshConfig.h"
#include "graphics/config/PipelineConfig.h"

#include "resource/Resource.h"

#include <glad/glad.h>

#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/tuple.h>

namespace xEngine {

struct OpenGLShader: public Resource<ShaderConfig> {

  GLuint program_id{0};

  eastl::hash_map<eastl::string, GLuint> uniform_location;

  eastl::hash_map<eastl::string, eastl::tuple<GLuint, GLuint, GLsizei>> uniform_block_info;

  eastl::hash_map<eastl::string, GLuint> texture_location;

  virtual void Reset() override {
    program_id = 0;
    uniform_location.clear();
    uniform_block_info.clear();
    texture_location.clear();
  }

};

struct OpenGLTexture: public Resource<TextureConfig> {

  GLuint texture_id{0};

  virtual void Reset() override {
    texture_id = 0;
  }

};

struct OpenGLMesh: public Resource<MeshConfig> {

  GLuint index_buffer_id{0};

  GLuint vertex_buffer_id{0};

  GLuint vertex_array_id{0};

  virtual void Reset() override {
    index_buffer_id = 0;
    vertex_buffer_id = 0;
    vertex_array_id = 0;
  }

};

struct OpenGLPipeline : public Resource<PipelineConfig> {

  virtual void Reset() override {

  }

};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
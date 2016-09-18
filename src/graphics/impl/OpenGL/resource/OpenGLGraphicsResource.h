#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H

#include "graphics/config/ShaderConfig.h"

#include <resource/Resource.h>

#include <glad/glad.h>

namespace xEngine {

struct OpenGLShader : public Resource<ShaderConfig> {
  GLuint shader_id{ 0 };
};

}

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
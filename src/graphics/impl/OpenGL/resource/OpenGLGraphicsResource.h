#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H

#if X_OPENGL

#include "graphics/config/ShaderConfig.h"

#include <resource/Resource.h>

#include <glad/glad.h>

namespace xEngine {

struct OpenGLShader : public Resource<ShaderConfig> {
  GLuint program_id{ 0 };
};

}

#endif // X_OPENGL

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_RESOURCE_OPENGLGRAPHICSRESOURCE_H
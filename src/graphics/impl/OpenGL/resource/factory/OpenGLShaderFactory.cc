#if X_OPENGL

#include "OpenGLShaderFactory.h"
#include "graphics/impl/OpenGL/OpenGLDefine.h"

#include <core/Log.h>

namespace xEngine {

void OpenGLShaderFactory::Create(OpenGLShader &resource, DataPtr data) {
  x_assert(resource.status() == ResourceStatus::kInvalid);
  resource.Loading();

  auto type = GLEnumFromShaderType(resource.config().type);
  auto source = static_cast<const char *>(data->buffer());
  auto length = static_cast<int32>(data->size());

  auto shader = glCreateShader(type);
  x_assert(shader != 0);

  glShaderSource(shader, 1, &source, &length);
  glCompileShader(shader);

  auto status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

#if X_DEBUG
  auto log_length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  if (status == GL_FALSE && log_length > 0) {
    Log::GetInstance().Debug("[shader source]:\n%s\n", source);
    auto log = static_cast<GLchar *>(malloc(log_length));
    glGetShaderInfoLog(shader, log_length, &log_length, log);
    Log::GetInstance().Debug("[compile log]:\n%s\n", log);
    free(log);
  }
#endif

  if (status == GL_FALSE) {
    glDeleteShader(shader);
    shader = 0;
  }

  resource.shader_id = shader;
  resource.Complete();
}

void OpenGLShaderFactory::Destroy(OpenGLShader &resource) {
  x_assert(resource.status() == ResourceStatus::kCompleted);
  glDeleteShader(resource.shader_id);
}

}

#endif // X_OPENGL

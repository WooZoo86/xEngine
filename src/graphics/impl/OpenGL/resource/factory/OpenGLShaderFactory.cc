#if X_OPENGL

#include "OpenGLShaderFactory.h"
#include "graphics/impl/OpenGL/OpenGLDefine.h"

#include <core/Log.h>

namespace xEngine {

static GLuint CompileShader(GLenum type, DataPtr data) {
  auto source = static_cast<const char *>(data->buffer());
  auto length = static_cast<int32>(data->size());

  auto shader = glCreateShader(GL_VERTEX_SHADER);
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

  return shader;
}

static void DeleteShader(GLuint shader) {
  if (shader != 0) {
    glDeleteShader(shader);
  }
}

void OpenGLShaderFactory::Create(OpenGLShader &resource) {
  x_assert(resource.status() == ResourceStatus::kInvalid);
  resource.Loading();

  auto vertex_shader = CompileShader(GL_VERTEX_SHADER, resource.config().vertex);
  auto fragment_shader = CompileShader(GL_FRAGMENT_SHADER, resource.config().fragment);

  auto program = glCreateProgram();
  x_assert(program != 0);

  if (vertex_shader != 0 && fragment_shader != 0) {
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // TODO bind attribute location

    glLinkProgram(program);

    auto status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

#if X_DEBUG
    auto log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (status == GL_FALSE && log_length > 0) {
      auto log = static_cast<GLchar *>(malloc(log_length));
      glGetProgramInfoLog(program, log_length, &log_length, log);
      Log::GetInstance().Debug("[compile log]:\n%s\n", log);
      free(log);
    }
#endif

    if (status == GL_FALSE) {
      glDeleteProgram(program);
      program = 0;
    }
  }

  DeleteShader(vertex_shader);
  DeleteShader(fragment_shader);

  resource.program_id = program;

  resource.program_id == 0 ? resource.Failed() : resource.Complete();
}

void OpenGLShaderFactory::Destroy(OpenGLShader &resource) {
  x_assert(resource.status() == ResourceStatus::kCompleted || resource.status() == ResourceStatus::kFailed);
  if (resource.program_id != 0) {
    glDeleteProgram(resource.program_id);
  }
}

}

#endif // X_OPENGL

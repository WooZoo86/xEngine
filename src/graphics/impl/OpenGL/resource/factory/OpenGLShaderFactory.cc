#if X_OPENGL

#include "OpenGLShaderFactory.h"
#include "graphics/impl/OpenGL/OpenGLDefine.h"

#include "core/Log.h"

namespace xEngine {

static GLuint CompileShader(GLenum type, const char *data) {
  auto length = static_cast<GLint>(strlen(data));

  auto shader = glCreateShader(type);
  x_assert(shader != 0);

  glShaderSource(shader, 1, &data, &length);
  glCompileShader(shader);

  auto status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

#if X_DEBUG
  Log::GetInstance().Debug("[shader source]:\n%s\n", data);
  if (status == GL_FALSE) {
    auto log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    auto log = static_cast<char *>(malloc(static_cast<size_t>(log_length)));
    glGetShaderInfoLog(shader, log_length, &log_length, log);
    Log::GetInstance().Debug("[compile fail]:\n%s\n", log);
    free(log);
  } else {
    Log::GetInstance().Debug("[compile success]\n");
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
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();

  auto vertex_shader = CompileShader(GL_VERTEX_SHADER, resource.config().vertex);
  auto fragment_shader = CompileShader(GL_FRAGMENT_SHADER, resource.config().fragment);

  auto program = glCreateProgram();
  x_assert(program != 0);

  if (vertex_shader != 0 && fragment_shader != 0) {
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    for (auto semantic_index = 0; semantic_index < static_cast<uint8>(VertexElementSemantic::kMaxSemanticCount); ++semantic_index) {
      glBindAttribLocation(program,
                           static_cast<GLuint>(semantic_index),
                           AttributeNameForVertexElementSemantic(static_cast<VertexElementSemantic>(semantic_index)));
    }

    glLinkProgram(program);

    auto status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

#if X_DEBUG
    if (status == GL_FALSE) {
      auto log_length = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
			auto log = static_cast<char *>(malloc(static_cast<size_t>(log_length)));
      glGetProgramInfoLog(program, log_length, &log_length, log);
      Log::GetInstance().Debug("[link fail]:\n%s\n", log);
      free(log);
    } else {
      Log::GetInstance().Debug("[link success]\n");
    }
#endif

    if (status == GL_FALSE) {
      glDeleteProgram(program);
      program = 0;
    }
  } else {
    glDeleteProgram(program);
    program = 0;
  }

  DeleteShader(vertex_shader);
  DeleteShader(fragment_shader);

  if (program != 0) {
    int32 active_uniform_count, max_uniform_name_length;

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &active_uniform_count);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);

		auto uniform_name = static_cast<char *>(malloc(static_cast<size_t>(max_uniform_name_length)));

    for (auto uniform_index = 0; uniform_index < active_uniform_count; ++uniform_index) {
      GLenum type;
      GLsizei size;
      GLsizei actual_uniform_name_length;
      glGetActiveUniform(program, static_cast<GLuint>(uniform_index), max_uniform_name_length, &actual_uniform_name_length, &size, &type, uniform_name);
      auto location = static_cast<GLuint>(glGetUniformLocation(program, uniform_name));
      resource.uniform_location.insert(eastl::make_pair(eastl::string(uniform_name, static_cast<size_t>(actual_uniform_name_length)), location));
    }

    free(uniform_name);
  }

  resource.config().vertex = nullptr;
  resource.config().fragment = nullptr;

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

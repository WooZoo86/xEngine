#if X_OPENGL

#include "OpenGLVertexDataFactory.h"

#include "graphics/impl/OpenGL/OpenGLDefine.h"

#include "core/Log.h"

namespace xEngine {

void OpenGLVertexDataFactory::Create(OpenGLVertexData &resource) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();

  if (resource.config().data == nullptr || resource.config().data->Empty()) {
    Log::GetInstance().Error("[OpenGLVertexDataFactory::Create] vertex data wrong size\n");
    resource.Failed();
    return;
  }

  GLuint buffer;
  glGenBuffers(1, &buffer);
  x_assert(buffer != 0);

  GLint current_buffer;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_buffer);

  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  glBufferData(GL_ARRAY_BUFFER,
               resource.config().data->size(),
               resource.config().data->buffer(),
               GLEnumForBufferUsage(resource.config().buffer_usage));

  glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(current_buffer));
  
  resource.buffer_id = buffer;

  resource.Complete();
}
void OpenGLVertexDataFactory::Destroy(OpenGLVertexData &resource) {
  x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
  if (resource.buffer_id != 0) {
    glDeleteBuffers(1, &resource.buffer_id);
  }
}

}

#endif // X_OPENGL
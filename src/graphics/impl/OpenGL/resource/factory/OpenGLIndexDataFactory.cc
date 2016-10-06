#if X_OPENGL

#include "OpenGLIndexDataFactory.h"

#include "graphics/impl/OpenGL/OpenGLDefine.h"

#include "core/Log.h"

namespace xEngine {

void OpenGLIndexDataFactory::Create(OpenGLIndexData &resource) {
  x_assert(resource.status() == ResourceStatus::kPending);
  resource.Loading();
  
  if (resource.config().type != IndexFormat::kNone &&
      (resource.config().data == nullptr ||
       resource.config().data->size() <
       SizeOfIndexFormat(resource.config().type) * resource.config().count)) {
    Log::GetInstance().Error("[OpenGLIndexDataFactory::Create] index data wrong size\n");
    resource.Failed();
    return;
  }

  GLuint buffer;
  glGenBuffers(1, &buffer);
  x_assert(buffer != 0);

  GLint current_buffer;
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &current_buffer);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               SizeOfIndexFormat(resource.config().type) * resource.config().count,
               resource.config().data->buffer(),
               GLEnumForBufferUsage(resource.config().buffer_usage));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(current_buffer));

  resource.config().data.reset();

  resource.buffer_id = buffer;

  resource.Complete();
}
void OpenGLIndexDataFactory::Destroy(OpenGLIndexData &resource) {
  x_assert(resource.status() == ResourceStatus::kFailed || resource.status() == ResourceStatus::kCompleted);
  if (resource.buffer_id != 0) {
    glDeleteBuffers(1, &resource.buffer_id);
  }
}

}

#endif // X_OPENGL
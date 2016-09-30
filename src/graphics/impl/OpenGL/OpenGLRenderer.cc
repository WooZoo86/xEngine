#if X_OPENGL

#include "OpenGLRenderer.h"

void xEngine::OpenGLRenderer::Initialize(const xEngine::GraphicsConfig &config) {
  if (gladLoadGL() == 0)
  {
    x_error("OpenGL init error!\n");
  }
#ifdef GLAD_DEBUG
  glad_set_pre_callback(PreOpenGLCallback);
  glad_set_post_callback(PostOpenGLCallback);
#endif
  Reset();
}

void xEngine::OpenGLRenderer::Finalize() {
  Reset();
}

void xEngine::OpenGLRenderer::Render() {
}

void xEngine::OpenGLRenderer::ApplyTarget(xEngine::ResourceID id) {

}

void xEngine::OpenGLRenderer::ApplyClearState(const ClearState &state) {
  glClearColor(state.clear_color.r, state.clear_color.g, state.clear_color.b, state.clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void xEngine::OpenGLRenderer::Reset() {

}

void xEngine::OpenGLRenderer::PreOpenGLCallback(const char *name, void *funcptr, int len_args, ...) {
  GLenum errorCode;
  errorCode = glad_glGetError();
  if (errorCode != GL_NO_ERROR)
  {
    Log::GetInstance().Error("[OpenGLRenderer::PreOpenGLCallback] ERROR 0x%04x in %s\n", errorCode, name);
    X_TRAP();
  }
}

void xEngine::OpenGLRenderer::PostOpenGLCallback(const char *name, void *funcptr, int len_args, ...) {
  GLenum errorCode;
  errorCode = glad_glGetError();
  if (errorCode != GL_NO_ERROR)
  {
    Log::GetInstance().Error("[OpenGLRenderer::PostOpenGLCallback] ERROR 0x%04x in %s\n", errorCode, name);
    X_TRAP();
  }
}

#endif // X_OPENGL

#include "Graphics.h"

#if X_OPENGL
#include "graphics/impl/OpenGL/OpenGLRenderer.h"
#include "graphics/impl/OpenGL/resource/OpenGLGraphicsResourceManager.h"
#endif

namespace xEngine {

void Graphics::Initialize(const GraphicsConfig &config) {
  x_assert(!Available());
  config_ = config;
  x_assert(config.window != kInvalidResourceID);
  switch (config_.type) {
    case GraphicsType::kOpenGL3:
#if X_OPENGL
      renderer_.reset(new OpenGLRenderer);
      resource_manager_.reset(new OpenGLGraphicsResourceManager);
#endif
      break;
    default:
      Log::GetInstance().Error("[Graphics::Initialize] unsupported graphics type!\n");
      break;
  }
	x_assert(renderer_ && resource_manager_);
  renderer_->Initialize(config_);
  resource_manager_->Initialize(config_);
}
void Graphics::Finalize() {
  x_assert(Available());
  config_ = GraphicsConfig();
  resource_manager_->Finalize();
  renderer_->Finalize();
}

bool Graphics::Available() {
  return renderer_ != nullptr;
}

}
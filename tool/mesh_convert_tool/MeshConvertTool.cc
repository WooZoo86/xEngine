#include "MeshConvertToolDefine.h"
#include "MeshConvertToolWindow.h"
#include "MeshViewerWindow.h"

#include <application/ApplicationDelegate.h>
#include <window/Window.h>

namespace xEngine {

void MeshConvertTool::Initialize() {
  Window::GetInstance().Initialize();
  window_ = new MeshConvertToolWindow;
  viewer_ = new MeshViewerWindow;
}

void MeshConvertTool::Finalize() {
  delete window_;
  delete viewer_;
  Window::GetInstance().Finalize();
}

} // namespace xEngine

XENGINE_WINDOW_APPLICATION(xEngine::MeshConvertTool)

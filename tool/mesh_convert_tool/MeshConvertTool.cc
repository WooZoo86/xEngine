#include "MeshConvertToolDefine.h"
#include "MeshConvertToolWindow.h"
#include "MeshViewerWindow.h"

#include <application/ApplicationDelegate.h>
#include <io/IO.h>
#include <storage/Storage.h>
#include <window/Window.h>

namespace xEngine {

void MeshConvertTool::Initialize() {
  Window::GetInstance().Initialize();
  IO::GetInstance().Initialize();
  IO::GetInstance().AddPlaceholder("texture", "storage://" +
      Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("texture").string() + Path::separator());
  IO::GetInstance().AddPlaceholder("shader", "storage://" +
      Path::ExecutablePath().ParentDirectory().ParentDirectory().Append("assets").Append("shader").string() + Path::separator());
  IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);
  window_ = new MeshConvertToolWindow;
  viewer_ = new MeshViewerWindow;
}

void MeshConvertTool::Finalize() {
  delete window_;
  delete viewer_;
  IO::GetInstance().Finalize();
  Window::GetInstance().Finalize();
}

} // namespace xEngine

XENGINE_WINDOW_APPLICATION(xEngine::MeshConvertTool)

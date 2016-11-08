#include "MeshConvertToolWindow.h"

#include <application/ApplicationDelegate.h>
#include <window/Window.h>

using namespace xEngine;

class MeshConvertTool: public ApplicationDelegate {
 public:
  virtual void Initialize() override {
    Window::GetInstance().Initialize();
    window_ = new MeshConvertToolWindow;
  }
  virtual void Finalize() override {
    delete window_;
    Window::GetInstance().Finalize();
  }

 private:
  MeshConvertToolWindow *window_{nullptr};
};

XENGINE_WINDOW_APPLICATION(MeshConvertTool)
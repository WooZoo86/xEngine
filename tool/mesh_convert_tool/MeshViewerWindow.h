#ifndef XENGINE_TOOL_MESHCONVERTTOOL_MESHVIEWERWINDOW_H
#define XENGINE_TOOL_MESHCONVERTTOOL_MESHVIEWERWINDOW_H

#include "MeshConvertToolDefine.h"

#include <asset/graphics/Camera.h>
#include <graphics/Graphics.h>
#include <resource/Resource.h>
#include <window/WindowDelegate.h>

#include <EASTL/tuple.h>

namespace xEngine {

class MeshViewerWindow : public WindowDelegate {
 public:
  MeshViewerWindow();

  virtual ~MeshViewerWindow();

  virtual void OnWindowUpdate() override;

  virtual void OnWindowClose() override;

  virtual void OnWindowMousePosition(const glm::vec2 &position) override;

  virtual void OnWindowMouseScroll(const glm::vec2 &offset) override;

  void Show(Context &context);

 private:
  void InitializeShader();

  void InitializeCamera();

  void InitializePipeline();

 private:
  CameraUniquePtr camera_;
  ResourceID shader_{kInvalidResourceID};
  ResourceID pipeline_{kInvalidResourceID};
  ResourceID window_id_{kInvalidResourceID};
  eastl::vector<eastl::tuple<ResourceID, DrawCallState>> mesh_;
};

} // namespace xEngine

#endif // XENGINE_TOOL_MESHCONVERTTOOL_MESHVIEWERWINDOW_H

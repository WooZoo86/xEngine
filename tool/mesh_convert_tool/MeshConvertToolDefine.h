#ifndef XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTTOOLDEFINE_H
#define XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTTOOLDEFINE_H

#include <asset/graphics/util/MeshUtil.h>
#include <application/ApplicationDelegate.h>

#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace xEngine {

struct MeshInfo {
  eastl::string name;
  eastl::vector<eastl::string> materials;
  MeshUtil mesh_util;
};

struct Context {
  eastl::string file;
  eastl::vector<MeshInfo> meshes;
};

struct PostProcessFlag {
  int32 triangulate{1};
};

struct RemoveComponentFlag {
  int32 texcoord0{0};
  int32 texcoord1{0};
  int32 texcoord2{0};
  int32 texcoord3{0};
  int32 color0{0};
  int32 color1{0};
  int32 normal{0};
  int32 tangent{0};
  int32 binormal{0};
};

struct RemoveTopologyFlag {
  int32 point{1};
  int32 line{1};
  int32 triangle{0};
  int32 polygon{1};
};

class MeshConvertToolWindow;

class MeshViewerWindow;

class MeshConvertTool : public ApplicationDelegate {
 public:
  virtual void Initialize() override;

  virtual void Finalize() override;

  MeshViewerWindow *viewer() const { return viewer_; }

 private:
  MeshConvertToolWindow *window_{nullptr};
  MeshViewerWindow *viewer_{nullptr};
};

} // namespace xEngine

#endif XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTTOOLDEFINE_H

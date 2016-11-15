#ifndef XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H
#define XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H

#include "MeshConvertToolDefine.h"

#include <fbxsdk.h>

namespace xEngine {

class MeshConverter {
 public:
  static void Convert(const eastl::string &in, const eastl::string &out);

 private:
  static void ProcessMeshConfig(MeshInfo &info, FbxMesh *mesh);

  static void ProcessMeshVertexPosition(MeshInfo &info, FbxMesh *mesh);

  static void ProcessMeshVertexTexcoord(MeshInfo &info, FbxMesh *mesh);

  static void ProcessMeshVertex(MeshInfo &info, FbxMesh *mesh);

  static void ProcessMeshIndex(MeshInfo &info, FbxMesh *mesh);

  static void ProcessMeshMaterial(MeshInfo &info, FbxNode *node);

  static void ProcessMesh(Context &context, FbxNode *node);

  static void ProcessNode(Context &context, FbxNode *node);

  static void SaveContext(Context &context);

public:
  static PostProcessFlag post_process_flag;

  static RemoveComponentFlag remove_component_flag;

  static RemoveTopologyFlag remove_topology_flag;
};

} // namespace xEngine

#endif // XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H

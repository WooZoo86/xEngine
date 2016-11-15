#include "MeshConverter.h"
#include "MeshViewerWindow.h"

#include <application/Application.h>
#include <core/Log.h>

namespace xEngine {

PostProcessFlag MeshConverter::post_process_flag;

RemoveComponentFlag MeshConverter::remove_component_flag;

RemoveTopologyFlag MeshConverter::remove_topology_flag;

void MeshConverter::Convert(const eastl::string &in, const eastl::string &out) {
  auto manager = FbxManager::Create();
  if (manager == nullptr) {
    Log::GetInstance().Error("unable to create FBX manager\n");
    return;
  }
  auto io_setting = FbxIOSettings::Create(manager, IOSROOT);
  if (io_setting == nullptr) {
    Log::GetInstance().Error("unable to create FBX IO Setting\n");
    return;
  }
  manager->SetIOSettings(io_setting);
  auto scene = FbxScene::Create(manager, "FBX scene");
  if (scene == nullptr) {
    Log::GetInstance().Error("unable to create FBX scene\n");
    return;
  }
  auto importer = FbxImporter::Create(manager, "FBX impoter");
  if (importer == nullptr) {
    Log::GetInstance().Error("unable to create FBX importer\n");
    return;
  }
  if (!importer->Initialize(in.c_str(), -1, io_setting)) {
    Log::GetInstance().Error("unable to initialize FBX importer, file: %s, error: %s\n", in.c_str(), importer->GetStatus().GetErrorString());
    return;
  }
  if (!importer->Import(scene)) {
    Log::GetInstance().Error("unable to import FBX scene, file: %s, error: %s\n", in.c_str(), importer->GetStatus().GetErrorString());
    return;
  }
  importer->Destroy();
  if (post_process_flag.triangulate) {
    FbxGeometryConverter converter(manager);
    converter.Triangulate(scene, true);
  }
  Context context;
  context.file = out;
  ProcessNode(context, scene->GetRootNode());
  manager->Destroy();

  static_cast<MeshConvertTool *>(Application::GetInstance().delegate())->viewer()->Show(context);
}

void MeshConverter::ProcessMeshConfig(MeshInfo &info, FbxMesh *mesh) {
  info.mesh_util.config().vertex_count = mesh->GetControlPointsCount();
  info.mesh_util.config().index_count = mesh->GetPolygonCount() * 3;
  info.mesh_util.config().index_type = mesh->GetPolygonCount() * 3 > eastl::numeric_limits<uint16>::max() ? IndexFormat::kUint32 : IndexFormat::kUint16;

  info.mesh_util.config().layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);

  if (mesh->GetElementUVCount() > 0 && !remove_component_flag.texcoord0) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
  }
  if (mesh->GetElementUVCount() > 1 && !remove_component_flag.texcoord1) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kTexcoord1, VertexElementFormat::kFloat2);
  }
  if (mesh->GetElementUVCount() > 2 && !remove_component_flag.texcoord2) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kTexcoord2, VertexElementFormat::kFloat2);
  }
  if (mesh->GetElementUVCount() > 3 && !remove_component_flag.texcoord3) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kTexcoord3, VertexElementFormat::kFloat2);
  }
  if (mesh->GetElementUVCount() > 4) {
    Log::GetInstance().Warning("unsupported: too many element uv component\n");
  }

  if (mesh->GetElementVertexColorCount() > 0 && !remove_component_flag.color0) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kColor0, VertexElementFormat::kFloat4);
  }
  if (mesh->GetElementVertexColorCount() > 1 && !remove_component_flag.color1) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kColor1, VertexElementFormat::kFloat4);
  }
  if (mesh->GetElementVertexColorCount() > 2) {
    Log::GetInstance().Warning("unsupported: too many element vertex color component\n");
  }

  if (mesh->GetElementNormalCount() > 0 && !remove_component_flag.normal) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kNormal, VertexElementFormat::kFloat3);
  }
  if (mesh->GetElementNormalCount() > 1) {
    Log::GetInstance().Warning("unsupported: too many element normal component\n");
  }

  if (mesh->GetElementTangentCount() > 0 && !remove_component_flag.tangent) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kTangent, VertexElementFormat::kFloat3);
  }
  if (mesh->GetElementTangentCount() > 1) {
    Log::GetInstance().Warning("unsupported: too many element tangent component\n");
  }

  if (mesh->GetElementBinormalCount() > 0 && !remove_component_flag.binormal) {
    info.mesh_util.config().layout.AddElement(VertexElementSemantic::kBinormal, VertexElementFormat::kFloat3);
  }
  if (mesh->GetElementBinormalCount() > 1) {
    Log::GetInstance().Warning("unsupported: too many element binormal component\n");
  }
}

void MeshConverter::ProcessMeshVertexPosition(MeshInfo &info, FbxMesh *mesh) {
  for (auto index = 0; index < mesh->GetControlPointsCount(); ++index) {
    auto position = mesh->GetControlPointAt(index);
    info.mesh_util.Vertex(VertexElementSemantic::kPosition, index, position[0], position[1], position[2]);
  }
}

void MeshConverter::ProcessMeshVertexTexcoord(MeshInfo &info, FbxMesh *mesh) {
  for (auto uv_layer_index = 0; uv_layer_index < mesh->GetElementUVCount(); ++uv_layer_index) {
    if (uv_layer_index == 0 && remove_component_flag.texcoord0) continue;
    if (uv_layer_index == 1 && remove_component_flag.texcoord1) continue;
    if (uv_layer_index == 2 && remove_component_flag.texcoord2) continue;
    if (uv_layer_index == 3 && remove_component_flag.texcoord3) continue;
    auto uv_layer = mesh->GetElementUV(uv_layer_index);
    auto semantic = static_cast<VertexElementSemantic>(static_cast<uint8>(VertexElementSemantic::kTexcoord0) + uv_layer_index);
    switch (uv_layer->GetMappingMode()) {
    case FbxGeometryElement::eByControlPoint: {
      switch (uv_layer->GetReferenceMode()) {
      case FbxGeometryElement::eDirect: {
        for (auto index = 0; index < mesh->GetControlPointsCount(); ++index) {
          auto uv = uv_layer->GetDirectArray().GetAt(index);
          info.mesh_util.Vertex(semantic, index, uv[0], uv[1]);
        }
      }
      case FbxGeometryElement::eIndexToDirect: {
        for (auto index = 0; index < mesh->GetControlPointsCount(); ++index) {
          auto uv = uv_layer->GetDirectArray().GetAt(uv_layer->GetIndexArray().GetAt(index));
          info.mesh_util.Vertex(semantic, index, uv[0], uv[1]);
        }
      }
      default: break;
      }
      break;
    }
    case FbxGeometryElement::eByPolygonVertex: {
      switch (uv_layer->GetReferenceMode()) {
      case FbxGeometryElement::eDirect:
      case FbxGeometryElement::eIndexToDirect: {
        for (auto polygon_index = 0; polygon_index < mesh->GetPolygonCount(); ++polygon_index) {
          for (auto position_index = 0; position_index < mesh->GetPolygonSize(polygon_index); ++position_index) {
            auto uv = uv_layer->GetDirectArray().GetAt(mesh->GetTextureUVIndex(polygon_index, position_index));
            auto index = mesh->GetPolygonVertex(polygon_index, position_index);
            info.mesh_util.Vertex(semantic, index, uv[0], uv[1]);
          }
        }
      }
      default: break;
      }
      break;
    }
    default: break;
    }
  }
}

void MeshConverter::ProcessMeshVertex(MeshInfo &info, FbxMesh *mesh) {
  info.mesh_util.BeginVertex();
  ProcessMeshVertexPosition(info, mesh);
  ProcessMeshVertexTexcoord(info, mesh);
  info.mesh_util.EndVertex();
}

void MeshConverter::ProcessMeshIndex(MeshInfo &info, FbxMesh *mesh) {
  info.mesh_util.BeginIndex();
  for (auto polygon_index = 0; polygon_index < mesh->GetPolygonCount(); ++polygon_index) {
    auto vertex_count = mesh->GetPolygonSize(polygon_index);
    if (vertex_count != 3) continue;
    for (auto position_index = 0; position_index < vertex_count; ++position_index) {
      auto index = mesh->GetPolygonVertex(polygon_index, position_index);
      if (info.mesh_util.config().index_type == IndexFormat::kUint16) {
        info.mesh_util.Index16(index);
      }
      else {
        info.mesh_util.Index32(index);
      }
    }
  }
  info.mesh_util.EndIndex();
}

void MeshConverter::ProcessMeshMaterial(MeshInfo &info, FbxNode *node) {
  for (auto index = 0; index < node->GetMaterialCount(); ++index) {
    auto material = node->GetMaterial(index);
    info.materials.push_back(material->GetName());
  }
}

void MeshConverter::ProcessMesh(Context &context, FbxNode *node) {
  MeshInfo info;
  info.name = node->GetName();
  
  auto mesh = node->GetMesh();

  ProcessMeshConfig(info, mesh);
  ProcessMeshVertex(info, mesh);
  ProcessMeshIndex(info, mesh);
  ProcessMeshMaterial(info, node);

  context.meshes.push_back(info);
}

void MeshConverter::ProcessNode(Context &context, FbxNode *node) {
  if (node->GetNodeAttribute() != nullptr) {
    switch (node->GetNodeAttribute()->GetAttributeType()) {
      case FbxNodeAttribute::eMesh: {
        ProcessMesh(context, node);
        break;
      }
      case FbxNodeAttribute::eSkeleton: {
        break;
      }
      default: {
        break;
      }
    }
  }
  for (auto index = 0; index < node->GetChildCount(); ++index) {
    ProcessNode(context, node->GetChild(index));
  }
}

void MeshConverter::SaveContext(Context &context) {
  auto file = fopen(context.file.c_str(), "wb");

  fclose(file);
}

} // namespace xEngine

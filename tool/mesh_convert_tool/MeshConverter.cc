#include "MeshConverter.h"

#include <core/Log.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace xEngine {

static const auto g_magic = 'XMSH';

static void write_int32(FILE *file, int32 value) {
  fwrite(&value, 1, sizeof(int32), file);
}

static void write_float32(FILE *file, float32 value) {
  fwrite(&value, 1, sizeof(float32), file);
}

void MeshConverter::Convert(const eastl::string &in, const eastl::string &out) {
  Assimp::Importer importer;
  importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, GetRemoveComponentFlag());
  importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, GetRemoveTopologyFlag());

  auto scene = importer.ReadFile(in.c_str(), GetPostProcessFlag());
  if (scene == nullptr) {
    Log::GetInstance().Warning("cannot load mesh model from %s\n", in.c_str());
    return;
  }

  auto file = fopen(out.c_str(), "wb");
  write_int32(file, g_magic);
  write_int32(file, scene->mNumMeshes);

  for (auto mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    auto mesh = scene->mMeshes[mesh_index];
    write_int32(file, mesh->mNumVertices);
    for (auto vertex_index = 0; vertex_index < mesh->mNumVertices; ++vertex_index) {
      auto vertex = mesh->mVertices[vertex_index];
      write_float32(file, vertex.x);
      write_float32(file, vertex.y);
      write_float32(file, vertex.z);
    }
    write_int32(file, mesh->mNumFaces * 3);
    for (auto face_index = 0; face_index < mesh->mNumFaces; ++face_index) {
      auto &face = mesh->mFaces[face_index];
      for (auto index = 0; index < face.mNumIndices; ++index) {
        write_int32(file, face.mIndices[index]);
      }
    }
  }

  importer.FreeScene();

  fclose(file);
}

uint32 MeshConverter::GetPostProcessFlag() {
  uint32 flag = 0;
  if (PostProcessFlag::calc_tangent_space) flag |= aiProcess_CalcTangentSpace;
  if (PostProcessFlag::join_identical_vertices) flag |= aiProcess_JoinIdenticalVertices;
  if (PostProcessFlag::make_left_handed) flag |= aiProcess_MakeLeftHanded;
  if (PostProcessFlag::triangulate) flag |= aiProcess_Triangulate;
  if (PostProcessFlag::remove_component) flag |= aiProcess_RemoveComponent;
  if (PostProcessFlag::gen_normals) flag |= aiProcess_GenNormals;
  if (PostProcessFlag::gen_smooth_normals) flag |= aiProcess_GenSmoothNormals;
  if (PostProcessFlag::split_large_meshes) flag |= aiProcess_SplitLargeMeshes;
  if (PostProcessFlag::pre_transform_vertices) flag |= aiProcess_PreTransformVertices;
  if (PostProcessFlag::limit_bone_weights) flag |= aiProcess_LimitBoneWeights;
  if (PostProcessFlag::validate_data_structure) flag |= aiProcess_ValidateDataStructure;
  if (PostProcessFlag::improve_cache_locality) flag |= aiProcess_ImproveCacheLocality;
  if (PostProcessFlag::remove_redundant_materials) flag |= aiProcess_RemoveRedundantMaterials;
  if (PostProcessFlag::fix_infacing_normals) flag |= aiProcess_FixInfacingNormals;
  if (PostProcessFlag::sort_by_ptype) flag |= aiProcess_SortByPType;
  if (PostProcessFlag::find_degenerates) flag |= aiProcess_FindDegenerates;
  if (PostProcessFlag::find_invalid_data) flag |= aiProcess_FindInvalidData;
  if (PostProcessFlag::gen_uv_coords) flag |= aiProcess_GenUVCoords;
  if (PostProcessFlag::transform_uv_coords) flag |= aiProcess_TransformUVCoords;
  if (PostProcessFlag::find_instances) flag |= aiProcess_FindInstances;
  if (PostProcessFlag::optimize_meshes) flag |= aiProcess_OptimizeMeshes;
  if (PostProcessFlag::optimize_graph) flag |= aiProcess_OptimizeGraph;
  if (PostProcessFlag::flip_uvs) flag |= aiProcess_FlipUVs;
  if (PostProcessFlag::flip_winding_order) flag |= aiProcess_FlipWindingOrder;
  if (PostProcessFlag::split_by_bone_count) flag |= aiProcess_SplitByBoneCount;
  if (PostProcessFlag::debone) flag |= aiProcess_Debone;
  return flag;
}

uint32 MeshConverter::GetRemoveComponentFlag() {
  uint32 flag = 0;
  if (RemoveComponentFlag::normals) flag |= aiComponent_NORMALS;
  if (RemoveComponentFlag::tangents_and_bitangents) flag |= aiComponent_TANGENTS_AND_BITANGENTS;
  if (RemoveComponentFlag::colors) flag |= aiComponent_COLORS;
  if (RemoveComponentFlag::colors0) flag |= aiComponent_COLORSn(0);
  if (RemoveComponentFlag::colors1) flag |= aiComponent_COLORSn(1);
  if (RemoveComponentFlag::colors2) flag |= aiComponent_COLORSn(2);
  if (RemoveComponentFlag::colors3) flag |= aiComponent_COLORSn(3);
  if (RemoveComponentFlag::texcoords) flag |= aiComponent_TEXCOORDS;
  if (RemoveComponentFlag::texcoords0) flag |= aiComponent_TEXCOORDSn(0);
  if (RemoveComponentFlag::texcoords1) flag |= aiComponent_TEXCOORDSn(1);
  if (RemoveComponentFlag::texcoords2) flag |= aiComponent_TEXCOORDSn(2);
  if (RemoveComponentFlag::texcoords3) flag |= aiComponent_TEXCOORDSn(3);
  if (RemoveComponentFlag::boneweights) flag |= aiComponent_BONEWEIGHTS;
  if (RemoveComponentFlag::animations) flag |= aiComponent_ANIMATIONS;
  if (RemoveComponentFlag::textures) flag |= aiComponent_TEXTURES;
  if (RemoveComponentFlag::lights) flag |= aiComponent_LIGHTS;
  if (RemoveComponentFlag::cameras) flag |= aiComponent_CAMERAS;
  if (RemoveComponentFlag::meshes) flag |= aiComponent_MESHES;
  if (RemoveComponentFlag::materials) flag |= aiComponent_MATERIALS;
  return flag;
}

uint32 MeshConverter::GetRemoveTopologyFlag() {
  uint32 flag = 0;
  if (RemoveTopologyFlag::point) flag |= aiPrimitiveType_POINT;
  if (RemoveTopologyFlag::line) flag |= aiPrimitiveType_LINE;
  if (RemoveTopologyFlag::triangle) flag |= aiPrimitiveType_TRIANGLE;
  if (RemoveTopologyFlag::polygon) flag |= aiPrimitiveType_POLYGON;
  return flag;
}

int32 MeshConverter::PostProcessFlag::calc_tangent_space = 0;
int32 MeshConverter::PostProcessFlag::join_identical_vertices = 0;
int32 MeshConverter::PostProcessFlag::make_left_handed = 0;
int32 MeshConverter::PostProcessFlag::triangulate = 1;
int32 MeshConverter::PostProcessFlag::remove_component = 0;
int32 MeshConverter::PostProcessFlag::gen_normals = 0;
int32 MeshConverter::PostProcessFlag::gen_smooth_normals = 0;
int32 MeshConverter::PostProcessFlag::split_large_meshes = 0;
int32 MeshConverter::PostProcessFlag::pre_transform_vertices = 0;
int32 MeshConverter::PostProcessFlag::limit_bone_weights = 0;
int32 MeshConverter::PostProcessFlag::validate_data_structure = 0;
int32 MeshConverter::PostProcessFlag::improve_cache_locality = 0;
int32 MeshConverter::PostProcessFlag::remove_redundant_materials = 0;
int32 MeshConverter::PostProcessFlag::fix_infacing_normals = 0;
int32 MeshConverter::PostProcessFlag::sort_by_ptype = 0;
int32 MeshConverter::PostProcessFlag::find_degenerates = 0;
int32 MeshConverter::PostProcessFlag::find_invalid_data = 0;
int32 MeshConverter::PostProcessFlag::gen_uv_coords = 0;
int32 MeshConverter::PostProcessFlag::transform_uv_coords = 0;
int32 MeshConverter::PostProcessFlag::find_instances = 0;
int32 MeshConverter::PostProcessFlag::optimize_meshes = 0;
int32 MeshConverter::PostProcessFlag::optimize_graph = 0;
int32 MeshConverter::PostProcessFlag::flip_uvs = 0;
int32 MeshConverter::PostProcessFlag::flip_winding_order = 0;
int32 MeshConverter::PostProcessFlag::split_by_bone_count = 0;
int32 MeshConverter::PostProcessFlag::debone = 0;

int32 MeshConverter::RemoveComponentFlag::normals = 0;
int32 MeshConverter::RemoveComponentFlag::tangents_and_bitangents = 0;
int32 MeshConverter::RemoveComponentFlag::colors = 0;
int32 MeshConverter::RemoveComponentFlag::colors0 = 0;
int32 MeshConverter::RemoveComponentFlag::colors1 = 0;
int32 MeshConverter::RemoveComponentFlag::colors2 = 0;
int32 MeshConverter::RemoveComponentFlag::colors3 = 0;
int32 MeshConverter::RemoveComponentFlag::texcoords = 0;
int32 MeshConverter::RemoveComponentFlag::texcoords0 = 0;
int32 MeshConverter::RemoveComponentFlag::texcoords1 = 0;
int32 MeshConverter::RemoveComponentFlag::texcoords2 = 0;
int32 MeshConverter::RemoveComponentFlag::texcoords3 = 0;
int32 MeshConverter::RemoveComponentFlag::boneweights = 0;
int32 MeshConverter::RemoveComponentFlag::animations = 0;
int32 MeshConverter::RemoveComponentFlag::textures = 0;
int32 MeshConverter::RemoveComponentFlag::lights = 0;
int32 MeshConverter::RemoveComponentFlag::cameras = 0;
int32 MeshConverter::RemoveComponentFlag::meshes = 0;
int32 MeshConverter::RemoveComponentFlag::materials = 0;

int32 MeshConverter::RemoveTopologyFlag::point = 1;
int32 MeshConverter::RemoveTopologyFlag::line = 1;
int32 MeshConverter::RemoveTopologyFlag::triangle = 0;
int32 MeshConverter::RemoveTopologyFlag::polygon = 0;

} // namespace xEngine

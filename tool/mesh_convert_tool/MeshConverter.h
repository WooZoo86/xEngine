#ifndef XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H
#define XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H

#include <core/Types.h>

#include <EASTL/string.h>

namespace xEngine {

class MeshConverter {
 public:
  static void Convert(const eastl::string &in, const eastl::string &out);

 private:
  static uint32 GetPostProcessFlag();

  static uint32 GetRemoveComponentFlag();

  static uint32 GetRemoveTopologyFlag();

 public:
  struct PostProcessFlag {
    static int32 calc_tangent_space;
    static int32 join_identical_vertices;
    static int32 make_left_handed;
    static int32 triangulate;
    static int32 remove_component;
    static int32 gen_normals;
    static int32 gen_smooth_normals;
    static int32 split_large_meshes;
    static int32 pre_transform_vertices;
    static int32 limit_bone_weights;
    static int32 validate_data_structure;
    static int32 improve_cache_locality;
    static int32 remove_redundant_materials;
    static int32 fix_infacing_normals;
    static int32 sort_by_ptype;
    static int32 find_degenerates;
    static int32 find_invalid_data;
    static int32 gen_uv_coords;
    static int32 transform_uv_coords;
    static int32 find_instances;
    static int32 optimize_meshes;
    static int32 optimize_graph;
    static int32 flip_uvs;
    static int32 flip_winding_order;
    static int32 split_by_bone_count;
    static int32 debone;
  };

  struct RemoveComponentFlag {
    static int32 normals;
    static int32 tangents_and_bitangents;
    static int32 colors;
    static int32 colors0;
    static int32 colors1;
    static int32 colors2;
    static int32 colors3;
    static int32 texcoords;
    static int32 texcoords0;
    static int32 texcoords1;
    static int32 texcoords2;
    static int32 texcoords3;
    static int32 boneweights;
    static int32 animations;
    static int32 textures;
    static int32 lights;
    static int32 cameras;
    static int32 meshes;
    static int32 materials;
  };

  struct RemoveTopologyFlag {
    static int32 point;
    static int32 line;
    static int32 triangle;
    static int32 polygon;
  };
};

} // namespace xEngine

#endif // XENGINE_TOOL_MESHCONVERTTOOL_MESHCONVERTOR_H

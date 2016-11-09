#include "MeshLoader.h"

namespace xEngine {

int32 read_int32(void **pointer) {
  auto cast_pointer = static_cast<int32 *>(*pointer);
  auto value = *cast_pointer++;
  *pointer = cast_pointer;
  return value;
}

float32 read_float32(void **pointer) {
  auto cast_pointer = static_cast<float32 *>(*pointer);
  auto value = *cast_pointer++;
  *pointer = cast_pointer;
  return value;
}

eastl::vector<MeshUtil> MeshLoader::Load(DataPtr data) {
  eastl::vector<MeshUtil> vector;

  auto pointer = data->buffer();
  if (read_int32(&pointer) != 'XMSH') {
    Log::GetInstance().Error("load mesh error, wrong magic!\n");
    return vector;
  }

  auto mesh_count = read_int32(&pointer);
  for (auto mesh_index = 0; mesh_index < mesh_count; ++mesh_index) {
    MeshUtil mesh;
    mesh.config().layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);
    mesh.config().vertex_count = static_cast<size_t>(read_int32(&pointer));
    mesh.config().vertex_usage = BufferUsage::kImmutable;

    mesh.BeginVertex();
    for (size_t vertex_index = 0; vertex_index < mesh.config().vertex_count; ++vertex_index) {
      mesh.Vertex(VertexElementSemantic::kPosition, vertex_index, read_float32(&pointer), read_float32(&pointer), read_float32(&pointer));
    }
    mesh.EndVertex();

    mesh.config().index_count = static_cast<size_t>(read_int32(&pointer));
    mesh.config().index_type = IndexFormat::kUint16;
    mesh.config().index_usage = BufferUsage::kImmutable;

    mesh.BeginIndex();
    for (auto index_index = 0; index_index < mesh.config().index_count; ++index_index) {
      mesh.Index16(static_cast<uint16>(read_int32(&pointer)));
    }
    mesh.EndIndex();

    vector.push_back(mesh);
  }

  return vector;
}

} // namespace xEngine
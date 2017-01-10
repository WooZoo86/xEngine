#include "Mesh.h"

#include "window/Window.h"

namespace xEngine {

MeshPtr Mesh::Parse(ResourceID window, DataPtr data) {
  if (window == kInvalidResourceID || data == nullptr) return nullptr;

  auto pointer = data->buffer();
  auto data_available = data->size();

  MeshConfig config;

  if (data_available < sizeof(uint8)) {
    Log::GetInstance().Error("load mesh error, no semantic count!\n");
    return nullptr;
  }

  auto semantic_count = read<uint8>(&pointer);

  data_available -= sizeof(uint8);

  for (auto semantic_index = 0; semantic_index < semantic_count; ++semantic_index) {
    if (data_available < sizeof(uint8) * 2) {
      Log::GetInstance().Error("load mesh error, no vertex element info of %d!\n", semantic_index + 1);
      return nullptr;
    }

    auto semantic_type = static_cast<VertexElementSemantic>(read<uint8>(&pointer));
    auto semantic_format = static_cast<VertexElementFormat>(read<uint8>(&pointer));
    config.layout.AddElement(semantic_type, semantic_format);

    data_available -= sizeof(uint8) * 2;
  }

  if (data_available < sizeof(size_t)) {
    Log::GetInstance().Error("load mesh error, no vertex count!\n");
    return nullptr;
  }
  
  auto vertex_count = read<size_t>(&pointer);
  config.vertex_count = vertex_count;

  data_available -= sizeof(size_t);

  if (data_available < sizeof(uint8)) {
    Log::GetInstance().Error("load mesh error, no index type!\n");
    return nullptr;
  }
  
  auto index_type = static_cast<IndexFormat>(read<uint8>(&pointer));
  config.index_type = index_type;

  data_available -= sizeof(uint8);

  if (data_available < sizeof(size_t)) {
    Log::GetInstance().Error("load mesh error, no index count!\n");
    return nullptr;
  }
  
  auto index_count = read<size_t>(&pointer);
  config.index_count = index_count;

  data_available -= sizeof(size_t);

  if (data_available < config.layout.size * config.vertex_count) {
    Log::GetInstance().Error("load mesh error, no vertex data!\n");
    return nullptr;
  }

  auto vertex_data = Data::Create(config.layout.size * config.vertex_count);
  memcpy(vertex_data->buffer(), pointer, config.layout.size * config.vertex_count);
  config.vertex_data = vertex_data;

  data_available -= config.layout.size * config.vertex_count;

  if (data_available < SizeOfIndexFormat(config.index_type) * config.index_count) {
    Log::GetInstance().Error("load mesh error, no vertex data!\n");
    return nullptr;
  }

  auto index_data = Data::Create(SizeOfIndexFormat(config.index_type) * config.index_count);
  memcpy(index_data->buffer(), pointer, SizeOfIndexFormat(config.index_type) * config.index_count);
  config.index_data = index_data;

  data_available -= SizeOfIndexFormat(config.index_type) * config.index_count;

  if (data_available != 0) {
    Log::GetInstance().Warning("load mesh, invalid data, size: %d!\n", data_available);
  }

  auto mesh = Mesh::Create();
  mesh->window_id_ = window;
  mesh->mesh_id_ = Window::GetInstance().GetGraphics(window)->resource_manager()->Create(config);
  mesh->draw_call_state_ = DrawCallState::Triangles(config.index_count);

  return mesh;
}

MeshPtr Mesh::Parse(ResourceID window, MeshUtil util) {
  auto mesh = Mesh::Create();
  mesh->window_id_ = window;
  mesh->mesh_id_ = Window::GetInstance().GetGraphics(window)->resource_manager()->Create(util.config());
  mesh->draw_call_state_ = DrawCallState::Triangles(util.config().index_count);

  return mesh;

}

void Mesh::Apply() {
  auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
  renderer->ApplyMesh(mesh_id_);
  renderer->Draw(draw_call_state_);
}

} // namespace xEngine

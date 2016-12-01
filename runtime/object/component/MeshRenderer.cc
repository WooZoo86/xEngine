#include "MeshRenderer.h"
#include "MeshFilter.h"

#include "object/core/GameObject.h"

namespace xEngine {

void MeshRenderer::Update() {
  auto mesh_filter = game_object().GetComponent<MeshFilter>(ComponentType::kMeshFilter);
  if (mesh_filter != nullptr) {
    mesh_filter->Apply();
  }
  for (auto &material : materials_) {
    material->Apply();
  }
}

void MeshRenderer::Serialize() {

}

void MeshRenderer::Deserialize() {

}

} // namespace xEngine
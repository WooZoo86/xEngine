#include "MeshRenderer.h"

namespace xEngine {

void MeshRenderer::Update() {
  for (auto &material : materials_) {
    if (material == nullptr) continue;
    material->Apply();
    if (mesh_ == nullptr) continue;
    mesh_->Apply();
  }
}

void MeshRenderer::Serialize() {

}

void MeshRenderer::Deserialize() {

}

} // namespace xEngine
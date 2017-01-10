#include "MeshRenderer.h"

#include "object/core/GameObject.h"

namespace xEngine {

void MeshRenderer::Update() {
  for (auto &material : materials_) {
    material->Apply();
    if (mesh_ != nullptr) {
      mesh_->Apply();
    }
  }
}

void MeshRenderer::Serialize() {

}

void MeshRenderer::Deserialize() {

}

} // namespace xEngine
#include "MeshFilter.h"

namespace xEngine {

void MeshFilter::Apply() {
  if (mesh_ != nullptr) {
    mesh_->Apply();
  }
}

void MeshFilter::Serialize() {

}

void MeshFilter::Deserialize() {

}

} // namespace xEngine

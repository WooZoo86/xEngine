#include "Material.h"

namespace xEngine {

void Material::Apply() {
  if (shader_ != nullptr) {
    shader_->Apply();
  }
}

} // namespace xEngine
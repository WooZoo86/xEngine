#ifndef XENGINE_OBJECT_COMPONENT_MESHRENDERER_H
#define XENGINE_OBJECT_COMPONENT_MESHRENDERER_H

#include "asset/Material.h"
#include "object/core/Component.h"

#include "core/Core.h"

#include <EASTL/vector.h>

namespace xEngine {

class MeshRenderer : public Component {
 public:
  CREATE_FUNC_DECLARE(MeshRenderer)

  MeshRenderer(GameObject &game_object) : Component(game_object) {}

  virtual ~MeshRenderer() {}

  void SetMaterialCount(size_t value) { materials_.resize(value); }

  MaterialPtr GetMaterial(size_t index) { return index < materials_.size() ?  materials_.at(index) : nullptr; }

  virtual void Update() override;

  virtual void Serialize() override;

  virtual void Deserialize() override;

  virtual ComponentType type() override { return ComponentType::kMeshRenderer; }

 private:
  eastl::vector<MaterialPtr> materials_;
};

} // namespace xEngine

#endif // XENGINE_OBJECT_COMPONENT_MESHRENDERER_H

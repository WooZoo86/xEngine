#ifndef XENGINE_OBJECT_COMPONENT_MESHFILTER_H
#define XENGINE_OBJECT_COMPONENT_MESHFILTER_H

#include "asset/Mesh.h"

#include "object/core/Component.h"

#include "core/Core.h"

namespace xEngine {

class MeshFilter : public Component {
 public:
  CREATE_FUNC_DECLARE(MeshFilter)

  MeshFilter(GameObject &game_object) : Component(game_object) {}

  virtual ~MeshFilter() {}

  void Apply();

  virtual void Serialize() override;

  virtual void Deserialize() override;

  virtual ComponentType type() override { return ComponentType::kMeshFilter; }

  MeshPtr mesh() const { return mesh_; }

  void set_mesh(MeshPtr value) { mesh_ = value; }

 private:
  MeshPtr mesh_;
};

} // namespace xEngine

#endif // XENGINE_OBJECT_COMPONENT_MESHFILTER_H

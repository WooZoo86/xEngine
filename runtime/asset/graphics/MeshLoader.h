#ifndef XENGINE_ASSET_GRAPHICS_MESHLOADER_H
#define XENGINE_ASSET_GRAPHICS_MESHLOADER_H

#include "asset/graphics/util/MeshUtil.h"

#include "core/Data.h"

#include <EASTL/vector.h>

namespace xEngine {

class MeshLoader {
 public:
  static eastl::vector<MeshUtil> Load(DataPtr data);
};

} // namespace xEngine

#endif // XENGINE_ASSET_GRAPHICS_MESHLOADER_H
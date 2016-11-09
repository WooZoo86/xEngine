#ifndef XENGINE_GRAPHICS_STATE_DRAWSTATE_H
#define XENGINE_GRAPHICS_STATE_DRAWSTATE_H

#include "graphics/GraphicsDefine.h"

namespace xEngine {

struct DrawState {

  static DrawState Triangles(size_t count) {
    DrawState state;
    state.count = count;
    return state;
  }

  size_t first{0};

  size_t count{0};

  VertexTopology topology{VertexTopology::kTriangles};

};

}

#endif // XENGINE_GRAPHICS_STATE_DRAWSTATE_H
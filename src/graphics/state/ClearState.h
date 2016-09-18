#ifndef XENGINE_GRAPHICS_STATE_CLEARSTATE_H
#define XENGINE_GRAPHICS_STATE_CLEARSTATE_H

#include "graphics/GraphicsDefine.h"

namespace xEngine {

struct ClearState {

  Color clear_color = Color(1.0f, 1.0f, 1.0f, 1.0f);

  float32 depth = 1.0f;

  uint8 stencil = 0;

  ClearType type = ClearType::kAll;

};

}

#endif // XENGINE_GRAPHICS_STATE_CLEARSTATE_H
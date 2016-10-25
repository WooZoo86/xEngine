#ifndef XENGINE_GRAPHICS_STATE_DEPTHSTENCILSTATE_H
#define XENGINE_GRAPHICS_STATE_DEPTHSTENCILSTATE_H

#include "graphics/GraphicsDefine.h"

namespace xEngine {

struct StencilState {
  union {
#pragma pack(push, 1)
    struct {
      StencilOperation fail : 8;
      StencilOperation depth_fail : 8;
      StencilOperation pass : 8;
      CompareFunction compare : 8;
      uint8 read_mask : 8;
      uint8 write_mask : 8;
      uint8 stencil_value : 8;
    };
#pragma pack(pop)
    uint64 value;
  };

  StencilState();

  bool operator==(const StencilState &other) const;

  bool operator!=(const StencilState &other) const;
};

inline StencilState::StencilState() {
  static_assert(sizeof(StencilState) == sizeof(uint64), "wrong size of StencilState");
  value = 0;
  fail = StencilOperation::kKeep;
  depth_fail = StencilOperation::kKeep;
  pass = StencilOperation::kKeep;
  compare = CompareFunction::kAlways;
  read_mask = 0xff;
  write_mask = 0xff;
  stencil_value = 0;
}

inline bool StencilState::operator==(const StencilState &other) const {
  return this->value == other.value;
}

inline bool StencilState::operator!=(const StencilState &other) const {
  return this->value != other.value;
}

struct DepthStencilState {
  StencilState front;

  StencilState back;

  union {
#pragma pack(push, 1)
    struct {
      CompareFunction compare : 4;
      bool depth_enable : 1;
      bool depth_mask : 1;
      bool stencil_enable : 1;
    };
#pragma pack(pop)
    uint8 value;
  };

  DepthStencilState();

  bool operator==(const DepthStencilState &other) const;

  bool operator!=(const DepthStencilState &other) const;
};

inline DepthStencilState::DepthStencilState() {
  static_assert(sizeof(DepthStencilState) == (sizeof(uint64) + 2 * sizeof(StencilState)), "wrong size of DepthStencilState");
  value = 0;
  compare = CompareFunction::kLess;
  depth_enable = false;
  depth_mask = false;
  stencil_enable = false;
}

inline bool DepthStencilState::operator==(const DepthStencilState &other) const {
  return this->value == other.value && this->front == other.front && this->back == other.back;
}

inline bool DepthStencilState::operator!=(const DepthStencilState &other) const {
  return this->value != other.value || this->front != other.front || this->back != other.back;
}

} // namespace xEngine

#endif // XENGINE_GRAPHICS_STATE_DEPTHSTENCILSTATE_H
#ifndef XENGINE_WINDOW_WINDOWDEFINE_H
#define XENGINE_WINDOW_WINDOWDEFINE_H

#include "core/Types.h"

namespace xEngine {

enum class PixelChannel: uint8 {
  kNone = 0,

  kAlpha = 1 << 0,
  kRed = 1 << 1,
  kGreen = 1 << 2,
  kBlue = 1 << 3,
  kDepth = 1 << 4,
  kStencil = 1 << 5,

  kDepthStentil = kDepth | kStencil,

  kRGBA = kAlpha | kRed | kGreen | kBlue,

  kAll = kAlpha | kRed | kGreen | kBlue | kDepth | kStencil,
};

enum class PixelFormat: uint8 {
  NONE,
  RGBA8,          ///< 32-bit wide, 4 channels @ 8-bit
  RGB8,           ///< 24-bit wide, 3 channels @ 8-bit
  RGBA4,          ///< 16-bit wide, 4 channels @ 4-bit
  R5G6B5,         ///< 16-bit wide, 3 channels @ 5/6/5 bits
  R5G5B5A1,       ///< 16-bit wide, 4 channels @ 1-bit alpha, 5-bit rgb
  RGBA32F,        ///< 128-bit wide, 4 channel @ 32-bit float
  RGBA16F,        ///< 64-bit wide, 4 channel @ 16-bit float
  L8,             ///< 8-bit wide, single channel
  DXT1,           ///< DXT1 compressed format
  DXT3,           ///< DXT3 compressed format
  DXT5,           ///< DXT5 compressed format
  D16,            ///< 16-bit depth
  D32,            ///< 32-bit depth
  D24S8,          ///< 24-bit depth, 8-bit stencil
  PVRTC2_RGB,     ///< PVRTC2 compressed format (RGB)
  PVRTC4_RGB,     ///< PVRTC4 compressed format (RGB)
  PVRTC2_RGBA,    ///< PVRTC2 compressed format (RGBA)
  PVRTC4_RGBA,    ///< PVRTC4 compressed format (RGBA)
  ETC2_RGB8,      ///< ETC2 compressed format (RGB8)
  ETC2_SRGB8,     ///< ETC2 compressed format (SRGB8)
};

enum class MouseCursorMode: uint8 {
  kNormal,
  kHidden,
  kDisable,
};

enum class MouseButtonType: uint8 {
  kUnknown,
  kLeft,
  kRight,
  kMiddle,
};

enum class KeyboardButtonType: uint8 {
  UNKNOWN,
  ESC,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  NUM_1,
  NUM_2,
  NUM_3,
  NUM_4,
  NUM_5,
  NUM_6,
  NUM_7,
  NUM_8,
  NUM_9,
  NUM_0,
  NUM_PAD_1,
  NUM_PAD_2,
  NUM_PAD_3,
  NUM_PAD_4,
  NUM_PAD_5,
  NUM_PAD_6,
  NUM_PAD_7,
  NUM_PAD_8,
  NUM_PAD_9,
  NUM_PAD_0,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,

  COUNT,
};

inline bool IsDepthPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::D16:
    case PixelFormat::D32:
    case PixelFormat::D24S8:
      return true;
    default:
      return false;
  }
}

inline bool IsCompressedPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::DXT1:
    case PixelFormat::DXT3:
    case PixelFormat::DXT5:
    case PixelFormat::PVRTC2_RGB:
    case PixelFormat::PVRTC4_RGB:
    case PixelFormat::PVRTC2_RGBA:
    case PixelFormat::PVRTC4_RGBA:
    case PixelFormat::ETC2_RGB8:
    case PixelFormat::ETC2_SRGB8:
      return true;
    default:
      return false;
  }
}

inline size_t SizeOfPixelFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
      return 4;
    case PixelFormat::RGB8:
      return 3;
    case PixelFormat::RGBA4:
      return 2;
    case PixelFormat::R5G6B5:
      return 2;
    case PixelFormat::R5G5B5A1:
      return 2;
    case PixelFormat::RGBA32F:
      return 16;
    case PixelFormat::RGBA16F:
      return 8;
    case PixelFormat::L8:
      return 1;
    default:
      return 0;
  }
}

inline int8 ChannelBitOfPixelFormat(PixelFormat format, PixelChannel channel) {
  int8 count = 0;
  switch (format) {
    case PixelFormat::RGBA32F:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)
          || (PixelChannel::kAlpha == channel)) {
        count = 32;
      }
      break;
    case PixelFormat::RGBA16F:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)
          || (PixelChannel::kAlpha == channel)) {
        count = 16;
      }
      break;
    case PixelFormat::RGBA8:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)
          || (PixelChannel::kAlpha == channel)) {
        count = 8;
      }
      break;
    case PixelFormat::RGB8:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)) {
        count = 8;
      }
      break;
    case PixelFormat::R5G6B5:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kBlue == channel)) {
        count = 5;
      }
      else if (PixelChannel::kGreen == channel) {
        count = 6;
      }
      break;
    case PixelFormat::R5G5B5A1:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)) {
        count = 5;
      }
      else if (PixelChannel::kAlpha == channel) {
        count = 1;
      }
      break;
    case PixelFormat::RGBA4:
      if ((PixelChannel::kRed == channel) || (PixelChannel::kGreen == channel) || (PixelChannel::kBlue == channel)
          || (PixelChannel::kAlpha == channel)) {
        count = 4;
      }
      break;
    case PixelFormat::L8:
      if (PixelChannel::kRed == channel) {
        count = 8;
      }
      break;
    case PixelFormat::D16:
      if (PixelChannel::kDepth == channel) {
        count = 16;
      }
      break;
    case PixelFormat::D32:
      if (PixelChannel::kDepth == channel) {
        count = 32;
      }
      break;
    case PixelFormat::D24S8:
      if (PixelChannel::kDepth == channel) {
        count = 24;
      }
      else if (PixelChannel::kStencil == channel) {
        count = 8;
      }
      break;
    default:
      break;
  }
  return count;
}

} // namespace xEngine

#endif // XENGINE_WINDOW_WINDOWDEFINE_H
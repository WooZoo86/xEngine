#ifndef XENGINE_GRAPHICS_GRAPHICSDEFINE_H
#define XENGINE_GRAPHICS_GRAPHICSDEFINE_H

#include "core/Core.h"
#include "core/Types.h"
#include "resource/Resource.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace xEngine {

typedef glm::vec4 Color;

typedef glm::highp_ivec4 IntColor;

enum class GraphicsMaxDefine: uint16 {
  kMaxTextureCount = 16,
  kMaxVertexElementCount = 16, // OpenGL ES 3.0
  kMaxCubeTextureFaceCount = 6,
  kMaxTextureMipMapCount = 12,
  kMaxUniformBlockCount = 16,
  kMaxSamplerCount = 48,
};

enum class GraphicsType: uint8 {
  kNone,
  kOpenGL3,
  kD3D11,
};

enum class GraphicsResourceType: uint16 {
  kVertexBuffer,
  kIndexBuffer,
  kShader,
  kProgram,
  kTexture,
};

enum class BlendFactor: uint8 {
  kZero,
  kOne,

  kSrcColor,
  kOneMinusSrcColor,

  kSrcAlpha,
  kOneMinusSrcAlpha,

  kDstColor,
  kOneMinusDstColor,

  kDstAlpha,
  kOneMinusDstAlpha,

  kBlendColor,
  kOneMinusBlendColor,

  kBlendAlpha,
  kOneMinusBlendAlpha,

  kSrcAlphaSaturated,
};

enum class BlendOperation: uint8 {
  kAdd,
  kSubtract,
  kReverseSubtract,
  kMin,
  kMax,
};

enum class TextureType: uint8 {
  kTexture2d,
  kTexture3d,
  kTextureCube,
};

enum class BufferUsage: uint8 {
  kImmutable,
  kDynamic,
  kStream,
};

enum class CompareFunction: uint8 {
  kNever,
  kLess,
  kLessEqual,
  kGreater,
  kGreaterEqual,
  kEqual,
  kNotEqual,
  kAlways,
};

enum class StencilOperation: uint8 {
  kKeep,
  kZero,
  kReplace,
  kIncrement,
  kIncrementWrap,
  kDecrement,
  kDecrementWrap,
  kInvert,
};

enum class FrontFaceType: uint8 {
  kClockWise,
  kCounterClockWise,
};

enum class FaceSide: uint8 {
  kFront,
  kBack,
  kBoth,
};

enum class IndexFormat: uint8 {
  kNone,
  kUint16,
  kUint32,
};

static size_t SizeOfIndexFormat(IndexFormat type) {
  auto size = 0;
  switch (type) {
    case IndexFormat::kNone:
      size = 0;
      break;
    case IndexFormat::kUint16:
      size = 2;
      break;
    case IndexFormat::kUint32:
      size = 4;
      break;
    default:
      break;
  }
  return size;
}

enum class VertexElementSemantic: uint8 {
  kPosition,
  kTexcoord0,
  kTexcoord1,
  kTexcoord2,
  kTexcoord3,
  kColor0,
  kColor1,
  kNormal,
  kTangent,
  kBinormal,
  kWeights,
  kIndices,
  kInstance0,
  kInstance1,
  kInstance2,
  kInstance3,

  kMaxSemanticCount,
  kInvalid
};

enum class VertexElementFormat: uint8 {
  kInvalid,
  kFloat1,
  kFloat2,
  kFloat3,
  kFloat4,
  kByte4,
  kByte4Normalized,
  kUnsignedByte4,
  kUnsignedByte4Normalized,
  kShort2,
  kShort2Normalized,
  kShort4,
  kShort4Normalized,
};

static bool IsNormalizedForVertexElementFormat(VertexElementFormat format)
{
  switch (format)
  {
    case VertexElementFormat::kFloat1: return false;
    case VertexElementFormat::kFloat2: return false;
    case VertexElementFormat::kFloat3: return false;
    case VertexElementFormat::kFloat4: return false;
    case VertexElementFormat::kByte4: return false;
    case VertexElementFormat::kByte4Normalized: return true;
    case VertexElementFormat::kUnsignedByte4: return false;
    case VertexElementFormat::kUnsignedByte4Normalized: return true;
    case VertexElementFormat::kShort2: return false;
    case VertexElementFormat::kShort2Normalized: return true;
    case VertexElementFormat::kShort4: return false;
    case VertexElementFormat::kShort4Normalized: return true;
    default: x_error("unknown VertexElementFormat\n"); return 0;
  }
}

static size_t SizeOfVertexElementFormat(VertexElementFormat format) {
  switch (format) {
    case VertexElementFormat::kFloat1:
      return 4;
    case VertexElementFormat::kFloat2:
      return 8;
    case VertexElementFormat::kFloat3:
      return 12;
    case VertexElementFormat::kFloat4:
      return 16;
    case VertexElementFormat::kByte4:
      return 4;
    case VertexElementFormat::kByte4Normalized:
      return 4;
    case VertexElementFormat::kUnsignedByte4:
      return 4;
    case VertexElementFormat::kUnsignedByte4Normalized:
      return 4;
    case VertexElementFormat::kShort2:
      return 4;
    case VertexElementFormat::kShort2Normalized:
      return 4;
    case VertexElementFormat::kShort4:
      return 8;
    case VertexElementFormat::kShort4Normalized:
      return 8;
    default:
      x_error("unknown VertexElementFormat\n");
      return 0;
  }
}

enum class UniformFormat: uint8 {
  kInvalid,
  kInt,
  kBool,
  kVector1,
  kVector2,
  kVector3,
  kVector4,
  kMatrix2,
  kMatrix3,
  kMatrix4,
  kTexture,
};

static int32 SizeOfUniformFormat(UniformFormat format) {
  switch (format) {
    case UniformFormat::kInt:
      return sizeof(int32);
    case UniformFormat::kBool:
      return sizeof(int32);
    case UniformFormat::kVector1:
      return sizeof(float32);
    case UniformFormat::kVector2:
      return 2 * sizeof(float32);
    case UniformFormat::kVector3:
      return 3 * sizeof(float32);
    case UniformFormat::kVector4:
      return 4 * sizeof(float32);
    case UniformFormat::kMatrix2:
      return 2 * 2 * sizeof(int32);
    case UniformFormat::kMatrix3:
      return 3 * 3 * sizeof(int32);
    case UniformFormat::kMatrix4:
      return 4 * 4 * sizeof(int32);
    case UniformFormat::kTexture:
    default:
      x_error("unknown UniformFormat\n");
      return 0;
  }
}

enum class VertexTopology: uint8 {
  kPoints,
  kLines,
  kLineStrip,
  kLineLoop,
  kTriangles,
  kTriangleStrip,
  kTriangleFan,
};

enum class TextureWrapMode: uint8 {
  kClampToEdge,
  kRepeat,
  kMirroredRepeat,
};

enum class TextureFilterMode: uint8 {
  kNearest,
  kLinear,
  kNearestMipmapNearest,
  kNearestMipmapLinear,
  kLinearMipmapNearest,
  kLinearMipmapLinear,
};

static bool IsTextureFilterModeUseMipmap(TextureFilterMode mode) {
  return !(mode == TextureFilterMode::kNearest || mode == TextureFilterMode::kLinear);
}

enum class ClearType: uint8 {
  kNone = 0,
  kColor = 1 << 0,
  kDepth = 1 << 1,
  kStencil = 1 << 2,

  kDepthAndStencil = static_cast<uint8>(kDepth) | static_cast<uint8>(kStencil),
  kAll = static_cast<uint8>(kColor) | static_cast<uint8>(kDepthAndStencil),
};

enum class GraphicsPipelineStage: uint8 {
  kVertexShader,
  kFragmentShader,
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSDEFINE_H
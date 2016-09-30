#ifndef XENGINE_GRAPHICS_GRAPHICSDEFINE_H
#define XENGINE_GRAPHICS_GRAPHICSDEFINE_H

#include <core/Core.h>
#include <core/Types.h>
#include <resource/Resource.h>

#include <glm.hpp>

namespace xEngine {

typedef glm::vec4 Color;

typedef glm::highp_ivec4 IntColor;

enum class GraphicsMaxDefine : uint16 {
  kMaxTextureCount = 16,
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
  kOneMinuxDstAlpha,

  kBlendColor,
  kOneMinusBlendColor,

  kBlendAlpha,
  kOneMinusBlendAlpha,

  kSrcAlphaSaturated,
};

enum class BlendOperation: uint8 {
  kAdd,
  kSubstract,
  kReverseSubtract,
};

enum class TextureType: uint8 {
  kTexture2d,
  kTexture3d,
  kTextureCube,
};

enum class BufferUsage: uint8 {
  kImmutable,
  kStatic,
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

enum class FaceSide: uint8 {
  kFront,
  kBack,
  kBoth,
};

enum class IndexAttributeType: uint8 {
  kNone,
  kUint16,
  kUint32,
};

static int32 SizeOfIndexAttributeType(IndexAttributeType type) {
  auto size = 0;
  switch (type) {
    case IndexAttributeType::kNone:
      size = 0;
      break;
    case IndexAttributeType::kUint16:
      size = 2;
      break;
    case IndexAttributeType::kUint32:
      size = 4;
      break;
    default:
      break;
  }
  return size;
}

enum class VertexAttributeType: uint8 {
  kPosition,
  kTexcoord0,
  kTexcoord1,
  kTexcorrd2,
  kTexcorrd3,
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

  kMaxCount,
};

static const char *VertexAttributeNameFromType(VertexAttributeType attribute) {
  switch (attribute) {
    case VertexAttributeType::kPosition:
      return "aPosition";
    case VertexAttributeType::kTexcoord0:
      return "aTexcoord0";
    case VertexAttributeType::kTexcoord1:
      return "aTexcoord1";
    case VertexAttributeType::kTexcorrd2:
      return "aTexcorrd2";
    case VertexAttributeType::kTexcorrd3:
      return "aTexcorrd3";
    case VertexAttributeType::kColor0:
      return "aColor0";
    case VertexAttributeType::kColor1:
      return "aColor1";
    case VertexAttributeType::kNormal:
      return "aNormal";
    case VertexAttributeType::kTangent:
      return "aTangent";
    case VertexAttributeType::kBinormal:
      return "aBinormal";
    case VertexAttributeType::kWeights:
      return "aWeights";
    case VertexAttributeType::kIndices:
      return "aIndices";
    case VertexAttributeType::kInstance0:
      return "aInstance0";
    case VertexAttributeType::kInstance1:
      return "aInstance1";
    case VertexAttributeType::kInstance2:
      return "aInstance2";
    case VertexAttributeType::kInstance3:
      return "aInstance3";
    default:
      x_error("unknown VertexAttributeType\n");
      return nullptr;
  }
}

enum class VertexAttributeFormat: uint8 {
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

static int32 SizeOfVertexAttributeFormat(VertexAttributeFormat format) {
  switch (format) {
    case VertexAttributeFormat::kFloat1:
      return 4;
    case VertexAttributeFormat::kFloat2:
      return 8;
    case VertexAttributeFormat::kFloat3:
      return 12;
    case VertexAttributeFormat::kFloat4:
      return 16;
    case VertexAttributeFormat::kByte4:
      return 4;
    case VertexAttributeFormat::kByte4Normalized:
      return 4;
    case VertexAttributeFormat::kUnsignedByte4:
      return 4;
    case VertexAttributeFormat::kUnsignedByte4Normalized:
      return 4;
    case VertexAttributeFormat::kShort2:
      return 4;
    case VertexAttributeFormat::kShort2Normalized:
      return 4;
    case VertexAttributeFormat::kShort4:
      return 8;
    case VertexAttributeFormat::kShort4Normalized:
      return 8;
    default:
      x_error("unknown VertexAttributeFormat\n");
      return 0;
  }
}

enum class UniformAttributeFormat: uint8 {
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

static int32 SizeOfUniformAttributeFormat(UniformAttributeFormat format) {
  switch (format) {
    case UniformAttributeFormat::kInt:
      return sizeof(int32);
    case UniformAttributeFormat::kBool:
      return sizeof(int32);
    case UniformAttributeFormat::kVector1:
      return sizeof(float32);
    case UniformAttributeFormat::kVector2:
      return 2 * sizeof(float32);
    case UniformAttributeFormat::kVector3:
      return 3 * sizeof(float32);
    case UniformAttributeFormat::kVector4:
      return 4 * sizeof(float32);
    case UniformAttributeFormat::kMatrix2:
      return 2 * 2 * sizeof(int32);
    case UniformAttributeFormat::kMatrix3:
      return 3 * 3 * sizeof(int32);
    case UniformAttributeFormat::kMatrix4:
      return 4 * 4 * sizeof(int32);
    case UniformAttributeFormat::kTexture:
    default:
      x_error("unknown UniformAttributeFormat\n");
      return 0;
  }
}

enum class DrawType : uint8 {
  kPoints,
  kLines,
  kLineStrip,
  kLineLoop,
  kTriangles,
  kTriangleStrip,
  kTriangleFan,
};

enum class TextureWrapMode : uint8 {
  kClampToEdge,
  kRepeat,
  kMirroredRepeat,
};

enum class TextureFilterMode : uint8 {
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

enum class ClearType : uint8 {
  kNone = 0,
  kColor = 1 << 0,
  kDepth = 1 << 1,
  kStencil = 1 << 2,

  kDepthAndStencil = kDepth | kStencil,
  kAll = kColor | kDepthAndStencil,
};

}

#endif // XENGINE_GRAPHICS_GRAPHICSDEFINE_H
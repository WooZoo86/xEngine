#ifndef XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLDEFINE_H
#define XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLDEFINE_H

#include "graphics/GraphicsDefine.h"
#include "graphics/config/GraphicsConfig.h"
#include "graphics/state/BlendState.h"
#include "graphics/state/DepthStencilState.h"
#include "graphics/state/RasterizerState.h"

#include <window/WindowDefine.h>

#include <glad/glad.h>

namespace xEngine {

struct OpenGLVertexAttributeParam {
  bool enabled{false};

  bool streaming{false};

  GLuint index{0};

  GLsizei stride{0};

  GLint size{0};

  GLboolean normalized{0};

  GLvoid *offset{nullptr};

  GLenum type{0};

  bool operator==(const OpenGLVertexAttributeParam &other) const {
    return enabled == other.enabled &&
        streaming == other.streaming &&
        index == other.index &&
        stride == other.stride &&
        size == other.size &&
        normalized == other.normalized &&
        offset == other.offset &&
        type == other.type;
  }

  bool operator!=(const OpenGLVertexAttributeParam &other) const {
    return enabled != other.enabled ||
        streaming != other.streaming ||
        index != other.index ||
        stride != other.stride ||
        size != other.size ||
        normalized != other.normalized ||
        offset != other.offset ||
        type != other.type;
  }
};

struct OpenGLRendererCache {
  BlendState blend_state_cache;
  DepthStencilState depth_stencil_state;
  RasterizerState rasterizer_state;

  GLint ViewportX;
  GLint ViewportY;
  GLsizei ViewportWidth;
  GLsizei ViewportHeight;

  GLint ScissorX;
  GLint ScissorY;
  GLsizei ScissorWidth;
  GLsizei ScissorHeight;

  Color BlendColor;

  GLuint VAO;
  GLuint VertexBufferCache;
  GLuint IndexBufferCache;
  IndexAttributeType IndexBufferType{IndexAttributeType::kNone};
  GLuint ProgramCache;

  GLuint Texture2DCache[static_cast<uint16>(GraphicsMaxDefine::kMaxTextureCount)];
  GLuint TextureCubeCache[static_cast<uint16>(GraphicsMaxDefine::kMaxTextureCount)];

  GLuint VertexAttributeBufferCache[static_cast<uint8>(VertexAttributeType::kMaxCount)];
  OpenGLVertexAttributeParam VertexAttributeParamCache[static_cast<uint8>(VertexAttributeType::kMaxCount)];
};

inline GLenum GLEnumFromBufferUsage(BufferUsage usage) {
  switch (usage) {
    case BufferUsage::kImmutable:
      return GL_STATIC_DRAW;
    case BufferUsage::kStatic:
      return GL_STATIC_DRAW;
    case BufferUsage::kDynamic:
      return GL_DYNAMIC_DRAW;
    case BufferUsage::kStream:
      return GL_STREAM_DRAW;
    default:
      x_error("unknown BufferUsage\n");
      return 0;
  }
}

inline GLenum GLEnumFromTextureFilterMode(TextureFilterMode mode) {
  switch (mode) {
    case TextureFilterMode::kNearest:
      return GL_NEAREST;
    case TextureFilterMode::kLinear:
      return GL_LINEAR;
    case TextureFilterMode::kNearestMipmapNearest:
      return GL_NEAREST_MIPMAP_NEAREST;
    case TextureFilterMode::kNearestMipmapLinear:
      return GL_NEAREST_MIPMAP_LINEAR;
    case TextureFilterMode::kLinearMipmapNearest:
      return GL_LINEAR_MIPMAP_NEAREST;
    case TextureFilterMode::kLinearMipmapLinear:
      return GL_LINEAR_MIPMAP_LINEAR;
    default:
      x_error("unknown TextureFilterMode\n");
      return 0;
  }
}

inline GLenum GLEnumFromTextureWrapMode(TextureWrapMode mode) {
  switch (mode) {
    case TextureWrapMode::kClampToEdge:
      return GL_CLAMP_TO_EDGE;
    case TextureWrapMode::kRepeat:
      return GL_REPEAT;
    case TextureWrapMode::kMirroredRepeat:
      return GL_MIRRORED_REPEAT;
    default:
      x_error("unknown TextureWrapMode\n");
      return 0;
  }
}

inline GLenum GLEnumFromTextureType(TextureType type) {
  switch (type) {
    case TextureType::kTexture2d:
      return GL_TEXTURE_2D;
    case TextureType::kTexture3d:
      return GL_TEXTURE_3D;
    case TextureType::kTextureCube:
      return GL_TEXTURE_CUBE_MAP;
    default:
      x_error("unknown TextureType\n");
      return 0;
  }
}

inline GLenum GLEnumFromPixelFormatAsInternal(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
      return GL_RGBA8;
    case PixelFormat::RGB8:
      return GL_RGB8;
    case PixelFormat::RGBA4:
      return GL_RGBA4;
    case PixelFormat::R5G6B5:
      return GL_RGB5;
    case PixelFormat::R5G5B5A1:
      return GL_RGB5_A1;
    case PixelFormat::RGBA32F:
      return GL_RGBA32F;
    case PixelFormat::RGBA16F:
      return GL_RGBA16F;
    case PixelFormat::L8:
      return GL_R8;
    case PixelFormat::DXT1:
      return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    case PixelFormat::DXT3:
      return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    case PixelFormat::DXT5:
      return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case PixelFormat::D16:
      return GL_DEPTH_COMPONENT16;
    case PixelFormat::D32:
      return GL_DEPTH_COMPONENT32F;
    case PixelFormat::D24S8:
      return GL_DEPTH24_STENCIL8;
    case PixelFormat::PVRTC2_RGB:
      return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
    case PixelFormat::PVRTC4_RGB:
      return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    case PixelFormat::PVRTC2_RGBA:
      return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
    case PixelFormat::PVRTC4_RGBA:
      return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    case PixelFormat::ETC2_RGB8:
      return GL_COMPRESSED_RGB8_ETC2;
    case PixelFormat::ETC2_SRGB8:
      return GL_COMPRESSED_SRGB8_ETC2;
    default:
      x_error("unknown PixelFormat\n");
      return 0;
  }
}

inline GLenum GLEnumFromPixelFormatAsLayout(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA32F:
      return GL_FLOAT;
    case PixelFormat::RGBA16F:
      return GL_HALF_FLOAT;
    case PixelFormat::RGBA8:
    case PixelFormat::RGB8:
    case PixelFormat::L8:
      return GL_UNSIGNED_BYTE;
    case PixelFormat::R5G5B5A1:
      return GL_UNSIGNED_SHORT_5_5_5_1;
    case PixelFormat::R5G6B5:
      return GL_UNSIGNED_SHORT_5_6_5;
    case PixelFormat::RGBA4:
      return GL_UNSIGNED_SHORT_4_4_4_4;
    case PixelFormat::D16:
      return GL_UNSIGNED_SHORT;
    case PixelFormat::D32:
      return GL_UNSIGNED_INT;
    case PixelFormat::D24S8:
      return GL_UNSIGNED_INT_24_8;
    default:
      x_error("unknown PixelFormat\n");
      return 0;
  }
}

inline GLenum GLEnumFromPixelFormatAsFormat(PixelFormat format) {
  switch (format) {
    case PixelFormat::RGBA8:
    case PixelFormat::R5G5B5A1:
    case PixelFormat::RGBA4:
    case PixelFormat::RGBA32F:
    case PixelFormat::RGBA16F:
      return GL_RGBA;
    case PixelFormat::RGB8:
    case PixelFormat::R5G6B5:
      return GL_RGB;
    case PixelFormat::L8:
      return GL_RED;
    case PixelFormat::D16:
    case PixelFormat::D32:
      return GL_DEPTH_COMPONENT;
    case PixelFormat::D24S8:
      return GL_DEPTH_STENCIL;
    case PixelFormat::DXT1:
      return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    case PixelFormat::DXT3:
      return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    case PixelFormat::DXT5:
      return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case PixelFormat::PVRTC2_RGB:
      return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
    case PixelFormat::PVRTC4_RGB:
      return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    case PixelFormat::PVRTC2_RGBA:
      return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
    case PixelFormat::PVRTC4_RGBA:
      return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    case PixelFormat::ETC2_RGB8:
      return GL_COMPRESSED_RGB8_ETC2;
    case PixelFormat::ETC2_SRGB8:
      return GL_COMPRESSED_SRGB8_ETC2;
    default:
      x_error("unknown PixelFormat\n");
      return 0;
  }
}

inline GLenum GLEnumFromShaderType(ShaderType type) {
  switch (type) {
    case ShaderType::kVertexShader:
      return GL_VERTEX_SHADER;
    case ShaderType::kFragmentShader:
      return GL_FRAGMENT_SHADER;
    default:
      x_error("unknown ShaderType\n");
      return 0;
  }
}

inline GLint VertexCountFromVertexAttributeFormat(VertexAttributeFormat format) {
  switch (format) {
    case VertexAttributeFormat::kFloat1:
      return 1;
    case VertexAttributeFormat::kFloat2:
      return 2;
    case VertexAttributeFormat::kFloat3:
      return 3;
    case VertexAttributeFormat::kFloat4:
      return 4;
    case VertexAttributeFormat::kByte4:
      return 4;
    case VertexAttributeFormat::kByte4Normalized:
      return 4;
    case VertexAttributeFormat::kUnsignedByte4:
      return 4;
    case VertexAttributeFormat::kUnsignedByte4Normalized:
      return 4;
    case VertexAttributeFormat::kShort2:
      return 2;
    case VertexAttributeFormat::kShort2Normalized:
      return 2;
    case VertexAttributeFormat::kShort4:
      return 4;
    case VertexAttributeFormat::kShort4Normalized:
      return 4;
    default:
      x_error("unknown VertexAttributeFormat\n");
      return 0;
  }
}

inline GLenum GLEnumFromVertexAttributeFormat(VertexAttributeFormat format) {
  switch (format) {
    case VertexAttributeFormat::kFloat1:
      return GL_FLOAT;
    case VertexAttributeFormat::kFloat2:
      return GL_FLOAT;
    case VertexAttributeFormat::kFloat3:
      return GL_FLOAT;
    case VertexAttributeFormat::kFloat4:
      return GL_FLOAT;
    case VertexAttributeFormat::kByte4:
      return GL_BYTE;
    case VertexAttributeFormat::kByte4Normalized:
      return GL_BYTE;
    case VertexAttributeFormat::kUnsignedByte4:
      return GL_UNSIGNED_BYTE;
    case VertexAttributeFormat::kUnsignedByte4Normalized:
      return GL_UNSIGNED_BYTE;
    case VertexAttributeFormat::kShort2:
      return GL_SHORT;
    case VertexAttributeFormat::kShort2Normalized:
      return GL_SHORT;
    case VertexAttributeFormat::kShort4:
      return GL_SHORT;
    case VertexAttributeFormat::kShort4Normalized:
      return GL_SHORT;
    default:
      x_error("unknown VertexAttributeFormat\n");
      return 0;
  }
}

inline GLboolean IsNormalizedFromVertexAttributeFormat(VertexAttributeFormat format) {
  switch (format) {
    case VertexAttributeFormat::kFloat1:
      return GL_FALSE;
    case VertexAttributeFormat::kFloat2:
      return GL_FALSE;
    case VertexAttributeFormat::kFloat3:
      return GL_FALSE;
    case VertexAttributeFormat::kFloat4:
      return GL_FALSE;
    case VertexAttributeFormat::kByte4:
      return GL_FALSE;
    case VertexAttributeFormat::kByte4Normalized:
      return GL_TRUE;
    case VertexAttributeFormat::kUnsignedByte4:
      return GL_FALSE;
    case VertexAttributeFormat::kUnsignedByte4Normalized:
      return GL_TRUE;
    case VertexAttributeFormat::kShort2:
      return GL_FALSE;
    case VertexAttributeFormat::kShort2Normalized:
      return GL_TRUE;
    case VertexAttributeFormat::kShort4:
      return GL_FALSE;
    case VertexAttributeFormat::kShort4Normalized:
      return GL_TRUE;
    default:
      x_error("unknown VertexAttributeFormat\n");
      return 0;
  }
}

inline GLenum GLEnumFromDrawType(DrawType type) {
  switch (type) {
    case DrawType::kPoints:
      return GL_POINTS;
    case DrawType::kLines:
      return GL_LINES;
    case DrawType::kLineStrip:
      return GL_LINE_STRIP;
    case DrawType::kLineLoop:
      return GL_LINE_LOOP;
    case DrawType::kTriangles:
      return GL_TRIANGLES;
    case DrawType::kTriangleStrip:
      return GL_TRIANGLE_STRIP;
    case DrawType::kTriangleFan:
      return GL_TRIANGLE_FAN;
    default:
      x_error("unknown DrawType\n");
      return 0;
  }
}

inline GLenum GLEnumFromIndexAttributeType(IndexAttributeType type) {
  switch (type) {
    case IndexAttributeType::kUint16:
      return GL_UNSIGNED_SHORT;
    case IndexAttributeType::kUint32:
      return GL_UNSIGNED_INT;
    default:
      x_error("unknown IndexAttributeType\n");
      return 0;
  }
}

inline GLenum GLEnumFromBlendFactor(BlendFactor type) {
  switch (type) {
    case BlendFactor::kZero:
      return GL_ZERO;
    case BlendFactor::kOne:
      return GL_ONE;
    case BlendFactor::kSrcColor:
      return GL_SRC_COLOR;
    case BlendFactor::kOneMinusSrcColor:
      return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::kSrcAlpha:
      return GL_SRC_COLOR;
    case BlendFactor::kOneMinusSrcAlpha:
      return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::kDstColor:
      return GL_DST_COLOR;
    case BlendFactor::kOneMinusDstColor:
      return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::kDstAlpha:
      return GL_DST_ALPHA;
    case BlendFactor::kOneMinuxDstAlpha:
      return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::kBlendColor:
      return GL_CONSTANT_COLOR;
    case BlendFactor::kOneMinusBlendColor:
      return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::kBlendAlpha:
      return GL_CONSTANT_ALPHA;
    case BlendFactor::kOneMinusBlendAlpha:
      return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::kSrcAlphaSaturated:
      return GL_SRC_ALPHA_SATURATE;
    default:
      x_error("unknown BlendFactor\n");
      return 0;
  }
}

inline GLenum GLEnumFromBlendOperation(BlendOperation type) {
  switch (type) {
    case BlendOperation::kAdd:
      return GL_FUNC_ADD;
    case BlendOperation::kSubstract:
      return GL_FUNC_SUBTRACT;
    case BlendOperation::kReverseSubtract:
      return GL_FUNC_REVERSE_SUBTRACT;
    default:
      x_error("unknown BlendOperation\n");
      return 0;
  }
}

inline GLenum GLEnumFromCompareFunction(CompareFunction func) {
  switch (func) {
    case CompareFunction::kNever:
      return GL_NEVER;
    case CompareFunction::kLess:
      return GL_LESS;
    case CompareFunction::kLessEqual:
      return GL_LEQUAL;
    case CompareFunction::kGreater:
      return GL_GREATER;
    case CompareFunction::kGreaterEqual:
      return GL_GEQUAL;
    case CompareFunction::kEqual:
      return GL_EQUAL;
    case CompareFunction::kNotEqual:
      return GL_NOTEQUAL;
    case CompareFunction::kAlways:
      return GL_ALWAYS;
    default:
      x_error("unknown CompareFunction\n");
      return 0;
  }
}

inline GLenum GLEnumFromStencilOperation(StencilOperation type) {
  switch (type) {
    case StencilOperation::kKeep:
      return GL_KEEP;
    case StencilOperation::kZero:
      return GL_ZERO;
    case StencilOperation::kReplace:
      return GL_REPLACE;
    case StencilOperation::kIncrement:
      return GL_INCR;
    case StencilOperation::kIncrementWrap:
      return GL_INCR_WRAP;
    case StencilOperation::kDecrement:
      return GL_DECR;
    case StencilOperation::kDecrementWrap:
      return GL_DECR_WRAP;
    case StencilOperation::kInvert:
      return GL_INVERT;
    default:
      x_error("unknown StencilOperation\n");
      return 0;
  }
}

inline GLenum GLEnumFromFaceSide(FaceSide face) {
  switch (face) {
    case FaceSide::kFront:
      return GL_FRONT;
    case FaceSide::kBack:
      return GL_BACK;
    case FaceSide::kBoth:
      return GL_FRONT_AND_BACK;
    default:
      x_error("unknown FaceSide\n");
      return 0;
  }
}

}

#endif // XENGINE_GRAPHICS_IMPL_OPENGL_OPENGLDEFINE_H
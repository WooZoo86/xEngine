#ifndef XENGINE_GRAPHICS_CONFIG_MESHCONFIG_H
#define XENGINE_GRAPHICS_CONFIG_MESHCONFIG_H

#include "graphics/GraphicsDefine.h"
#include "graphics/GraphicsResourceSignature.h"

#include "core/Data.h"

namespace xEngine {

struct VertexLayout {

  VertexLayout &AddElement(VertexElementSemantic semantic, VertexElementFormat format) {
    x_assert(element_count <= static_cast<uint16>(GraphicsMaxDefine::kMaxVertexElementCount));
    elements[element_count].semantic = semantic;
    elements[element_count].format = format;
    elements[element_count].offset = size;
    size += SizeOfVertexElementFormat(format);
    ++element_count;
    return *this;
  }

  struct {

    VertexElementSemantic semantic{VertexElementSemantic::kInvalid};

    VertexElementFormat format{VertexElementFormat::kInvalid};

    size_t offset{0};

  } elements[static_cast<uint16>(GraphicsMaxDefine::kMaxVertexElementCount)];

  uint16 element_count{0};

  size_t size{0};

};

struct MeshConfig {

  ResourceIdentity identity{ResourceIdentity::Shared(MeshSignature)};

  BufferUsage index_usage{BufferUsage::kImmutable};

  IndexFormat index_type{IndexFormat::kNone};

  int32 index_count{0};

  DataPtr index_data;

  BufferUsage vertex_usage{BufferUsage::kImmutable};

  int32 vertex_count{0};

  DataPtr vertex_data;

  VertexLayout layout;

};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_MESHCONFIG_H
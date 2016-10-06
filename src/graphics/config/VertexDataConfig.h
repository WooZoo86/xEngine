#ifndef XENGINE_GRAPHICS_CONFIG_VERTEXDATACONFIG_H
#define XENGINE_GRAPHICS_CONFIG_VERTEXDATACONFIG_H

#include "core/Data.h"
#include "graphics/GraphicsDefine.h"
#include "resource/ResourceIdentity.h"

#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/tuple.h>

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature VertexDataSignature = GetResourceSignature();

struct VertexElementConfig {

  VertexElementType type{VertexElementType::kInvalid};

  int8 index{0};

  VertexElementFormat format;

  bool is_normalized{false};

  size_t offset{0};

};

struct VertexDataConfig {

  ResourceIdentity identity{ResourceIdentity::Shared(VertexDataSignature)};

  BufferUsage buffer_usage{BufferUsage::kImmutable};

  int32 vertex_size{0};

  DataPtr data{nullptr};

  VertexElementConfig element[static_cast<uint16>(GraphicsMaxDefine::kMaxVertexElementCount)];

};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_VERTEXDATACONFIG_H
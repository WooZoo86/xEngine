#ifndef XENGINE_GRAPHICS_CONFIG_INDEXDATACONFIG_H
#define XENGINE_GRAPHICS_CONFIG_INDEXDATACONFIG_H

#include "core/Data.h"
#include "graphics/GraphicsDefine.h"
#include "resource/ResourceIdentity.h"

namespace xEngine {

IncreaseResourceSignatureCounter();
static const ResourceSignature IndexDataSignature = GetResourceSignature();

struct IndexDataConfig {

  ResourceIdentity identity{ResourceIdentity::Shared(IndexDataSignature)};

  BufferUsage buffer_usage{BufferUsage::kImmutable};

  IndexFormat type{IndexFormat::kNone};

  int32 count{0};

  DataPtr data{nullptr};

};

} // namespace xEngine

#endif // XENGINE_GRAPHICS_CONFIG_INDEXDATACONFIG_H
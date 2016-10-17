#ifndef XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
#define XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H

#if X_D3D11

#include "graphics/impl/D3D11/D3D11Define.h"

#include "graphics/config/ShaderConfig.h"
#include "graphics/config/VertexDataConfig.h"
#include "graphics/config/IndexDataConfig.h"
#include "graphics/config/TextureConfig.h"

#include "resource/Resource.h"

namespace xEngine {

	struct D3D11Shader : public Resource<ShaderConfig> {

		virtual void Reset() override {
		}

	};

	struct D3D11VertexData : public Resource<VertexDataConfig> {

		ID3D11Buffer *buffer{nullptr};

		virtual void Reset() override {
			buffer = nullptr;
		}

	};

	struct D3D11IndexData : public Resource<IndexDataConfig> {

		ID3D11Buffer *buffer{ nullptr };

		virtual void Reset() override {
			buffer = nullptr;
		}

	};

	struct D3D11Texture : public Resource<TextureConfig> {

		virtual void Reset() override {
		}

	};

} // namespace xEngine

#endif // X_D3D11

#endif // XENGINE_GRAPHICS_IMPL_D3D11_RESOURCE_D3D11GRAPHICSRESOURCE_H
#if X_D3D11

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "D3D11Define.h"
#include "D3D11Renderer.h"
#include "graphics/impl/D3D11/resource/D3D11GraphicsResourceManager.h"

#include "window/Window.h"

#include <gtc/type_ptr.hpp>

#include <Windows.h>

namespace xEngine {

void D3D11Renderer::Initialize(const GraphicsConfig &config) {
  window_ = Window::GetInstance().Get(config.window).get();
  resource_manager_ = static_cast<D3D11GraphicsResourceManager *>(window_->graphics()->resource_manager().get());

  int32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if X_DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  DXGI_SWAP_CHAIN_DESC swap_chain_desc;
  ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
  swap_chain_desc.BufferCount = 1;
  swap_chain_desc.BufferDesc.Width = window_->config().width;
  swap_chain_desc.BufferDesc.Height = window_->config().height;
  swap_chain_desc.BufferDesc.Format = SwapChainFormatFromPixelFormat(window_->config().color_format);
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.OutputWindow = static_cast<HWND>(window_->GetNativeHandle());
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = window_->config().sample_count > 1 ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;
  swap_chain_desc.Windowed = !window_->config().is_full_screen;

  D3D_FEATURE_LEVEL feature_level;
  x_d3d11_assert_msg(D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    flags,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &swap_chain_desc,
    &swap_chain_,
    &device_,
    &feature_level,
    &context_
  ), "D3D11 init error!\n");

  x_assert(device_ && context_ && swap_chain_);

  x_d3d11_assert_msg(swap_chain_->GetBuffer(
    0,
    __uuidof(ID3D11Texture2D),
    reinterpret_cast<void **>(&render_target_)
  ), "get render target failed!\n");

  x_assert(render_target_);

  x_d3d11_assert_msg(device_->CreateRenderTargetView(
    render_target_,
    nullptr,
    &render_target_view_
  ), "create render target view failed!\n");

  x_assert(render_target_view_);

  if (window_->config().depth_format != PixelFormat::NONE) {
    D3D11_TEXTURE2D_DESC depth_stencil_desc;
    ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
    depth_stencil_desc.Width = window_->config().width;
    depth_stencil_desc.Height = window_->config().height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = RenderTargetFormatFromPixelFormat(window_->config().depth_format);
    depth_stencil_desc.SampleDesc = swap_chain_desc.SampleDesc;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;

    x_d3d11_assert_msg(device_->CreateTexture2D(
      &depth_stencil_desc,
      nullptr,
      &depth_stencil_buffer_
    ), "create depth and stencil buffer failed!\n");

    x_assert(depth_stencil_buffer_);

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
    depth_stencil_view_desc.Format = depth_stencil_desc.Format;
    depth_stencil_view_desc.ViewDimension =
        window_->config().sample_count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    x_d3d11_assert_msg(device_->CreateDepthStencilView(
      depth_stencil_buffer_,
      &depth_stencil_view_desc,
      &depth_stencil_view_
    ), "create depth and stencil view failed!\n");

    x_assert(depth_stencil_view_);
  }

  Reset();
}

void D3D11Renderer::Finalize() {
  if (render_target_) {
    render_target_->Release();
    render_target_ = nullptr;
  }
  if (render_target_view_) {
    render_target_view_->Release();
    render_target_view_ = nullptr;
  }
  if (depth_stencil_buffer_) {
    depth_stencil_buffer_->Release();
    depth_stencil_buffer_ = nullptr;
  }
  if (depth_stencil_view_) {
    depth_stencil_view_->Release();
    depth_stencil_view_ = nullptr;
  }
  if (swap_chain_) {
    swap_chain_->Release();
    swap_chain_ = nullptr;
  }
  if (device_) {
    device_->Release();
    device_ = nullptr;
  }
  if (context_) {
    context_->Release();
    context_ = nullptr;
  }
}

void D3D11Renderer::Render() {
  swap_chain_->Present(window_->config().swap_interval, 0);
}

void D3D11Renderer::ApplyTarget(ResourceID id, const ClearState &state) {
  if (id != kInvalidResourceID) {
    auto &texture = resource_manager_->texture_pool_.Find(id);
    if (texture.status() == ResourceStatus::kCompleted &&
        texture.render_target_view != nullptr &&
        texture.depth_stencil_view != nullptr) {
      if (texture.render_target_view != cache_.render_target_view ||
          texture.depth_stencil_view != cache_.depth_stencil_view) {
        context_->OMSetRenderTargets(1, &texture.render_target_view, texture.depth_stencil_view);
        ApplyViewPort(0, 0, texture.config().width, texture.config().height);
        cache_.render_target_view = texture.render_target_view;
        cache_.depth_stencil_view = texture.depth_stencil_view;
      }
    }
  }
  else {
    if (render_target_view_ != cache_.render_target_view ||
        depth_stencil_view_ != cache_.depth_stencil_view) {
      context_->OMSetRenderTargets(1, &render_target_view_, depth_stencil_view_);
      ApplyViewPort(0, 0, window_->config().frame_buffer_width, window_->config().frame_buffer_height);
      cache_.render_target_view = render_target_view_;
      cache_.depth_stencil_view = depth_stencil_view_;
    }
  }

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kColor)) {
    if (cache_.render_target_view != nullptr) {
      context_->ClearRenderTargetView(cache_.render_target_view, glm::value_ptr(state.clear_color));
    }
  }

  auto flags = 0;

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kDepth)) {
    flags |= D3D11_CLEAR_DEPTH;
  }

  if (static_cast<uint8>(state.type) & static_cast<uint8>(ClearType::kStencil)) {
    flags |= D3D11_CLEAR_STENCIL;
  }

  if (cache_.depth_stencil_view != nullptr) {
    context_->ClearDepthStencilView(cache_.depth_stencil_view, flags, state.depth, state.stencil);
  }
}

void D3D11Renderer::ApplyViewPort(int32 x, int32 y, int32 width, int32 height) {
  if (cache_.viewport.TopLeftX != x ||
      cache_.viewport.TopLeftY != y ||
      cache_.viewport.Width != width ||
      cache_.viewport.Height != height) {
    cache_.viewport.TopLeftX = static_cast<FLOAT>(x);
    cache_.viewport.TopLeftY = static_cast<FLOAT>(y);
    cache_.viewport.Width = static_cast<FLOAT>(width);
    cache_.viewport.Height = static_cast<FLOAT>(height);
    cache_.viewport.MinDepth = 0.0f;
    cache_.viewport.MaxDepth = 1.0f;
    context_->RSSetViewports(1, &cache_.viewport);
  }
}

void D3D11Renderer::ApplyScissor(int32 x, int32 y, int32 width, int32 height) {
  if (cache_.scissor.left != x ||
      cache_.scissor.top != y ||
      cache_.scissor.right != x + width ||
      cache_.scissor.bottom != y + height) {
    cache_.scissor.left = static_cast<FLOAT>(x);
    cache_.scissor.top = static_cast<FLOAT>(y);
    cache_.scissor.right = static_cast<FLOAT>(x + width);
    cache_.scissor.bottom = static_cast<FLOAT>(y + height);
    context_->RSSetScissorRects(1, &cache_.scissor);
  }
}

void D3D11Renderer::ApplyBlendState(const BlendState &blend_state) {}

void D3D11Renderer::ResetBlendState() {}

void D3D11Renderer::ApplyDepthStencilState(const DepthStencilState &depth_stencil_state) {}

void D3D11Renderer::ResetDepthStencilState() {}

void D3D11Renderer::ApplyRasterizerState(const RasterizerState &rasterizer_state) {}

void D3D11Renderer::ResetRasterizerState() {}

void D3D11Renderer::ApplyShader(ResourceID id) {}

void D3D11Renderer::UpdateShaderUniform(ResourceID id, eastl::string name, UniformFormat format, const void *buffer) {}

void D3D11Renderer::ResetShader() {}

void D3D11Renderer::ApplyTexture(ResourceID id, int32 index) {}

void D3D11Renderer::ResetTexture() {}

void D3D11Renderer::ApplyMesh(ResourceID id) {}

void D3D11Renderer::UpdateMesh(ResourceID id, const void *vertex_buffer, size_t vertex_offset, size_t vertex_size, const void *index_buffer, size_t index_offset, size_t index_size) {}

void D3D11Renderer::ResetMesh() {}

void D3D11Renderer::DrawTopology(VertexTopology topology, int32 first, int32 count) {}

void D3D11Renderer::Reset() {
  ResetBlendState();
  ResetDepthStencilState();
  ResetRasterizerState();
  ResetShader();
  ResetTexture();
  ResetMesh();
}

}

#endif
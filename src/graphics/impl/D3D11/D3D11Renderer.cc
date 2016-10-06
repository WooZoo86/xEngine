#if X_D3D11

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "D3D11Define.h"
#include "D3D11Renderer.h"

#include "window/Window.h"

#include <gtc/type_ptr.hpp>

#include <Windows.h>

namespace xEngine {

void D3D11Renderer::Initialize(const GraphicsConfig &config) {
  auto &window = Window::GetInstance().Get(config.window);
  swap_interval_ = window->config().swap_interval;

  int32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if X_DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  DXGI_SWAP_CHAIN_DESC swap_chain_desc;
  ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
  swap_chain_desc.BufferCount = 1;
  swap_chain_desc.BufferDesc.Width = window->config().width;
  swap_chain_desc.BufferDesc.Height = window->config().height;
  swap_chain_desc.BufferDesc.Format = SwapChainFormatFromPixelFormat(window->config().color_format);
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.OutputWindow = static_cast<HWND>(window->GetNativeHandle());
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = window->config().sample_count > 1 ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;
  swap_chain_desc.Windowed = !window->config().is_full_screen;

  D3D_FEATURE_LEVEL feature_level;
  auto result = D3D11CreateDeviceAndSwapChain(
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
  );

  x_assert_msg(SUCCEEDED(result), "D3D11 init error!\n");
  x_assert(device_ && context_ && swap_chain_);

  result = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&render_target_));

  x_assert_msg(SUCCEEDED(result), "get render target failed!\n");
  x_assert(render_target_);

  result = device_->CreateRenderTargetView(render_target_, nullptr, &render_target_view_);

  x_assert_msg(SUCCEEDED(result), "create render target view failed!\n");
  x_assert(render_target_view_);

  if (window->config().depth_format != PixelFormat::NONE) {
    D3D11_TEXTURE2D_DESC depth_stencil_desc;
    ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
    depth_stencil_desc.Width = window->config().width;
    depth_stencil_desc.Height = window->config().height;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = RenderTargetFormatFromPixelFormat(window->config().depth_format);
    depth_stencil_desc.SampleDesc = swap_chain_desc.SampleDesc;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;

    result = device_->CreateTexture2D(&depth_stencil_desc, nullptr, &depth_stencil_buffer_);

    x_assert_msg(SUCCEEDED(result), "create depth and stencil buffer failed!\n");
    x_assert(depth_stencil_buffer_);

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
    depth_stencil_view_desc.Format = depth_stencil_desc.Format;
    depth_stencil_view_desc.ViewDimension =
        window->config().sample_count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    result = device_->CreateDepthStencilView(depth_stencil_buffer_, &depth_stencil_view_desc, &depth_stencil_view_);

    x_assert_msg(SUCCEEDED(result), "create depth and stencil view failed!\n");
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
  swap_chain_->Present(swap_interval_, 0);
}

void D3D11Renderer::ApplyTarget(ResourceID id) {}

void D3D11Renderer::ApplyClearState(const ClearState &state) {
  context_->ClearRenderTargetView(render_target_view_, glm::value_ptr(state.clear_color));
}

void D3D11Renderer::Reset() {}
}

#endif
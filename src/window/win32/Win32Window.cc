#if X_WINDOWS

#include "Win32Window.h"

#include <windowsx.h>

namespace xEngine {

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  auto self = static_cast<Win32Window *>(GetProp(hwnd, "xEngine"));
  switch (message) {
    case WM_CLOSE: {
      if (self) {
        self->should_close_ = true;
      }
      break;
    }
    case WM_SIZE: {
      self->config_.width = self->config_.frame_buffer_width = LOWORD(lParam);
      self->config_.height = self->config_.frame_buffer_height = HIWORD(lParam);
      break;
    }
    case WM_MOUSEMOVE: {
      self->mouse_position_.x = GET_X_LPARAM(lParam);
      self->mouse_position_.y = GET_Y_LPARAM(lParam);
      break;
    }
    case WM_LBUTTONDOWN: {
      self->mouse_button_down_status_ |= static_cast<uint8>(MouseButtonType::kLeft);
      self->mouse_button_status_cache_ |= static_cast<uint8>(MouseButtonType::kLeft);
      break;
    }
    case WM_LBUTTONUP: {
      self->mouse_button_up_status_ |= static_cast<uint8>(MouseButtonType::kLeft);
      self->mouse_button_status_cache_ ^= static_cast<uint8>(MouseButtonType::kLeft);
      break;
    }
    case WM_RBUTTONDOWN: {
      self->mouse_button_down_status_ |= static_cast<uint8>(MouseButtonType::kRight);
      self->mouse_button_status_cache_ |= static_cast<uint8>(MouseButtonType::kRight);
      break;
    }
    case WM_RBUTTONUP: {
      self->mouse_button_up_status_ |= static_cast<uint8>(MouseButtonType::kRight);
      self->mouse_button_status_cache_ ^= static_cast<uint8>(MouseButtonType::kRight);
      break;
    }
    case WM_MBUTTONDOWN: {
      self->mouse_button_down_status_ |= static_cast<uint8>(MouseButtonType::kMiddle);
      self->mouse_button_status_cache_ |= static_cast<uint8>(MouseButtonType::kMiddle);
      break;
    }
    case WM_MBUTTONUP: {
      self->mouse_button_up_status_ |= static_cast<uint8>(MouseButtonType::kMiddle);
      self->mouse_button_status_cache_ ^= static_cast<uint8>(MouseButtonType::kMiddle);
      break;
    }
    default:
      return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}

void Win32Window::Initialize() {
  WNDCLASS wndClass;
  ZeroMemory(&wndClass, sizeof(wndClass));
  wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndClass.lpfnWndProc = WndProc;
  wndClass.hInstance = GetModuleHandle(nullptr);
  wndClass.lpszClassName = "xEngine";
  wndClass.hIcon = static_cast<HICON>(LoadImage(GetModuleHandle(nullptr), "xEngineIcon", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  if (!wndClass.hIcon) {
    wndClass.hIcon = static_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  }
  if (!RegisterClass(&wndClass)) {
    x_error("register class error, code: %d\n", GetLastError());
  }
}

void Win32Window::Finalize() {
  UnregisterClass("xEngine", GetModuleHandle(nullptr));
}

void Win32Window::PollEvent() {
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void Win32Window::Create(const WindowConfig &config) {
  config_ = config;
  window_ = CreateWindow(
    "xEngine",
    config_.title.c_str(),
    config_.is_full_screen ? WS_POPUP : WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    config_.width,
    config_.height,
    nullptr,
    nullptr,
    GetModuleHandle(nullptr),
    nullptr
  );
  if (window_ == nullptr)
  x_error("fail to create window, code: %d\n", GetLastError());
  SetProp(window_, "xEngine", this);
  ShowWindow(window_, SW_SHOWNORMAL);
  SetCursor(LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW)));
}

void Win32Window::Destroy() {
  CloseWindow(window_);
  window_ = nullptr;
}

void Win32Window::SetTitle(const eastl::string &name) {
  SetWindowText(window_, name.c_str());
}

} // namespace xEngine

#endif // X_WINDOWS

#if X_WINDOWS

#include "Win32Window.h"

namespace xEngine {

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

LRESULT CALLBACK Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  auto self = static_cast<Win32Window *>(GetProp(hwnd, "xEngine"));
  switch (message) {
  case WM_CLOSE: {
        if (self) {
            self->CloseEvent();
        }
        break;
  }
  default:
        return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
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
  if (window_ == nullptr) x_error("fail to create window, code: %d\n", GetLastError());
  SetProp(window_, "xEngine", this);
  ShowWindow(window_, SW_SHOWNORMAL);
}

void Win32Window::Destroy() {
  CloseWindow(window_);
  window_ = nullptr;
}

bool Win32Window::Available() const {
  return window_ != nullptr;
}

bool Win32Window::ShouldClose() {
  return closed_;
}

void Win32Window::SetTitle(const eastl::string &name) {}

} // namespace xEngine

#endif // X_WINDOWS

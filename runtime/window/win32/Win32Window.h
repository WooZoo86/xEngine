#ifndef XENGINE_WINDOW_WIN32_WIN32WINDOW_H
#define XENGINE_WINDOW_WIN32_WIN32WINDOW_H

#if X_WINDOWS

#include "window/WindowInterface.h"

#include "application/Application.h"

#include <Windows.h>

namespace xEngine {

class Win32Window : public WindowInterface {
 public:
  virtual void Create(const WindowConfig &config) override;

  virtual void Destroy() override;

  virtual void SetTitle(const eastl::string &name) override;

  virtual void *GetNativeHandle() override { return window_; }

 private:
	LoopID loop_id_{kInvalidLoopID};
  HWND window_{nullptr};

  friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

} // namespace xEngine

#endif

#endif // XENGINE_WINDOW_WIN32_WIN32WINDOW_H
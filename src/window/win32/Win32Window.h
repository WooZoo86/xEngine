#ifndef XENGINE_WINDOW_WIN32_WIN32WINDOW_H
#define XENGINE_WINDOW_WIN32_WIN32WINDOW_H

#if X_WINDOWS

#include "window/WindowInterface.h"

#include <Windows.h>

namespace xEngine {

class Win32Window : public WindowInterface {
 public:
	static void Initialize();

	static void Finalize();

	static void PollEvent();

 private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

 public:
	virtual void Create(const WindowConfig &config) override;

	virtual void Destroy() override;

	virtual bool Available() override;

	virtual void MakeCurrent() override;

	virtual bool ShouldClose() override;

	virtual void Present() override;

	virtual void SetTitle(const eastl::string &name) override;

	virtual const WindowConfig &GetConfig() const override { return config_; }

 private:
	 void CloseEvent() { closed_ = true; };

 private:
	WindowConfig config_;
	HWND window_{nullptr};
	bool closed_{false};
};

} // namespace xEngine

#endif

#endif // XENGINE_WINDOW_WIN32_WIN32WINDOW_H
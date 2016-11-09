#ifndef XENGINE_APPLICATION_APPLICATIONDELEGATE_H
#define XENGINE_APPLICATION_APPLICATIONDELEGATE_H

#include "Application.h"

#if X_WINDOWS

#include <Windows.h>

#define XENGINE_APPLICATION(clazz) \
  int wmain(int argc, wchar_t *argv[]) { \
    xEngine::Application::GetInstance().Run(new clazz); \
    return 0; \
  }

#define XENGINE_WINDOW_APPLICATION(clazz) \
  int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) { \
    xEngine::Application::GetInstance().Run(new clazz); \
    return 0; \
  }

#else

#define XENGINE_APPLICATION(clazz) \
  int main(int argc, char *argv[]) { \
    xEngine::Application::GetInstance().Run(new clazz); \
    return 0; \
  }

#define XENGINE_WINDOW_APPLICATION(clazz) XENGINE_APPLICATION(clazz)

#endif // X_WINDOWS

namespace xEngine {

class ApplicationDelegate {
 public:
  virtual ~ApplicationDelegate() {}

  virtual void Initialize() {}

  virtual void Finalize() {}
};

class ApplicationLoopDelegate {
 public:
  virtual ~ApplicationLoopDelegate() {}

  virtual void OnBeforeEventLoop() {}

  virtual void OnAfterEventLoop() {}
};

} // namespace xEngine

#endif // XENGINE_APPLICATION_APPLICATIONDELEGATE_H
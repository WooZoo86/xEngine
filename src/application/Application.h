#ifndef XENGINE_APPLICATION_APPLICATION_H
#define XENGINE_APPLICATION_APPLICATION_H

#if X_WINDOWS

#include <Windows.h>

#define XENGINE_APPLICATION(clazz) \
  int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) { \
    auto app = new clazz; \
    app->Run(); \
    delete app; \
    return 0; \
  }

#else

#define XENGINE_APPLICATION(clazz) \
  int main(int argc, char *argv[]) { \
    auto app = new clazz; \
    app->Run(); \
    delete app; \
    return 0; \
  }

#endif // X_WINDOWS


namespace xEngine {

enum class ApplicationStatus {
  kInvalid,
  kInitialize,
  kFinalize,
  kLoop,
};

class Application {
 public:
  static Application *GetInstance() {
    return instance_;
  }

  Application();

  virtual ~Application();

  void Run();

  void Quit();

  virtual ApplicationStatus Initialize() { return ApplicationStatus::kLoop; }

  virtual ApplicationStatus Finalize() { return ApplicationStatus::kInvalid; }

  virtual ApplicationStatus Loop() { return ApplicationStatus::kFinalize; }

 private:
  static Application *instance_;

  ApplicationStatus status_{ApplicationStatus::kInitialize};
};

} // namespace xEngine

#endif // XENGINE_APPLICATION_APPLICATION_H
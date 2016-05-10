#include "application/Application.h"
#include "core/Log.h"

using namespace xEngine;

class ApplicationSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    Log::GetInstance().Info("Loop\n");
    return Application::Loop();
  }
};

XENGINE_APPLICATION(ApplicationSample)
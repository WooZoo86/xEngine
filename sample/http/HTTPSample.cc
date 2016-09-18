#include "application/Application.h"
#include "core/Log.h"
#include "io/IO.h"
#include "http/HTTPFilesystem.h"

using namespace xEngine;

class HTTPSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    IO::GetInstance().Initialize();
    IO::GetInstance().AddPlaceholder("web", "http://leafnsand.com/");
    IO::GetInstance().RegisterFilesystem("http", HTTPFilesystem::Creator);

    IO::GetInstance().Read("web:index.html", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        Log::GetInstance().Debug("read complete: %s\n", data->buffer());
        complete_ = true;
      }
    });
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    IO::GetInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    IO::GetInstance().Tick();
    return complete_ ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  bool complete_{false};
};

XENGINE_APPLICATION(HTTPSample)
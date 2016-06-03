#include "application/Application.h"
#include "core/Log.h"
#include "io/IO.h"
#include "http/HTTPFilesystem.h"

using namespace xEngine;

class HTTPSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    IO::getInstance().Initialize();
    IO::getInstance().AddPlaceholder("web", "http://leafnsand.com/");
    IO::getInstance().RegisterFilesystem("http", HTTPFilesystem::Creator);

    IO::getInstance().Read("web:index.html", [&](Location location, IOStatus status, DataPtr data) {
      if (status == IOStatus::kSuccess) {
        Log::GetInstance().Debug("read complete: %s\n", data->buffer());
        complete = true;
      }
    });
    return Application::Initialize();
  }
  virtual ApplicationStatus Finalize() override {
    Log::GetInstance().Info("Finalize\n");
    IO::getInstance().Finalize();
    return Application::Finalize();
  }
  virtual ApplicationStatus Loop() override {
    IO::getInstance().Tick();
    return complete ? ApplicationStatus::kFinalize : ApplicationStatus::kLoop;
  }

 private:
  bool complete{false};
};

XENGINE_APPLICATION(HTTPSample)
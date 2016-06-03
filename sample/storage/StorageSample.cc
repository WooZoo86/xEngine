#include "application/Application.h"
#include "core/Log.h"
#include "io/IO.h"
#include "storage/StorageFilesystem.h"

using namespace xEngine;

class ApplicationSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    IO::getInstance().Initialize();
    IO::getInstance().AddPlaceholder("local", "storage:///Users/leafnsand/Desktop/");
    IO::getInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    auto hello = "hello world!";
    auto test_data = Data::Create();
    test_data->Copy(hello, strlen(hello) + 1);
    IO::getInstance().Write("local:test.txt", test_data, [&](Location location1, IOStatus status1, DataPtr data1) {
      if (status1 == IOStatus::kSuccess) {
        Log::GetInstance().Debug("write complete.\n");
        IO::getInstance().Read("local:test.txt", [&](Location location2, IOStatus status2, DataPtr data2) {
          if (status2 == IOStatus::kSuccess) {
            Log::GetInstance().Debug("read complete: %s\n", data2->buffer());
            complete = true;
          }
        });
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

XENGINE_APPLICATION(ApplicationSample)
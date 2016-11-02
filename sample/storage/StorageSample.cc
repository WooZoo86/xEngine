#include "application/Application.h"
#include "core/Log.h"
#include "io/IO.h"
#include "storage/StorageFilesystem.h"

using namespace xEngine;

class StorageSample : public Application {
 public:
  virtual ApplicationStatus Initialize() override {
    Log::GetInstance().Info("Initialize\n");
    IO::GetInstance().Initialize();
#if X_MACOS
    IO::GetInstance().AddPlaceholder("local", "storage:///Users/leafnsand/Desktop/");
#elif X_WINDOWS
    IO::GetInstance().AddPlaceholder("local", "storage://C:\\Users\\leafnsand\\Desktop\\");
#endif
    IO::GetInstance().RegisterFilesystem("storage", StorageFilesystem::Creator);

    auto hello = "hello world!";
    auto test_data = Data::Create(hello, strlen(hello) + 1);
    IO::GetInstance().Write("local:test.txt", test_data, [&](Location location1, IOStatus status1, DataPtr data1) {
      if (status1 == IOStatus::kSuccess) {
        Log::GetInstance().Debug("write complete.\n");
        IO::GetInstance().Read("local:test.txt", [&](Location location2, IOStatus status2, DataPtr data2) {
          if (status2 == IOStatus::kSuccess) {
            Log::GetInstance().Debug("read complete: %s\n", data2->buffer());
            complete_ = true;
          }
        });
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

XENGINE_APPLICATION(StorageSample)
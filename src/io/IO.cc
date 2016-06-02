#include "IO.h"

#include "core/Core.h"

namespace xEngine {

void IO::Initialize(int32 threadCount) {
  x_assert(!available_);
  available_ = true;
  for (auto i = 0; i < threadCount; ++i) {
    auto worker = eastl::make_shared<IOWorker>();
    worker->Initialize();
    workers_.push_back(worker);
  }
}

void IO::Finalize() {
  x_assert(available_);
  available_ = false;
  for (auto &worker : workers_) {
    worker->Finalize();
  }
}

void IO::Tick() {
  x_assert(available_);
  for (auto &worker : workers_) {
    worker->Tick();
  }
}

void IO::Read(const eastl::string &file, IOCallbackFunction callback) {
  auto message = eastl::make_shared<IOReadMessage>(location_placeholder_manager_.ResolveLocation(file), callback);
  workers_[++last_worker_ % workers_.size()]->Handle(message);
}

void IO::Write(const eastl::string &file, DataPtr data, IOCallbackFunction callback) {
  auto message = eastl::make_shared<IOWriteMessage>(location_placeholder_manager_.ResolveLocation(file), data, callback);
  workers_[++last_worker_ % workers_.size()]->Handle(message);
}

}
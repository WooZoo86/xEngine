#include "core/Core.h"
#include "application/Application.h"

namespace xEngine {

Application *Application::instance_ = nullptr;

Application::Application() {
  x_assert_msg(instance_ == nullptr, "should only have one Application instance exist!");
  instance_ = this;
}

Application::~Application() {
  instance_ = nullptr;
}

void Application::Run() {
  while (status_ != ApplicationStatus::kInvalid) {
    switch (status_) {
      case ApplicationStatus::kInitialize: {
        status_ = Initialize();
        break;
      }
      case ApplicationStatus::kFinalize: {
        status_ = Finalize();
        break;
      }
      case ApplicationStatus::kLoop: {
        status_ = Loop();
        break;
      }
      default: {
        break;
      }
    }
  }
}

void Application::Quit() {
  status_ = ApplicationStatus::kFinalize;
}

} // namespace xEngine
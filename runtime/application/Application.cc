#include "application/Application.h"

#include "core/Core.h"

namespace xEngine {

extern void NativeApplicationInitialize(ApplicationDelegate *delegate);

extern void NativeApplicationFinalize(ApplicationDelegate *delegate);

extern bool PollNativeApplicationEvent();

void Application::Run(ApplicationDelegate *delegate) {
  x_assert(delegate_ == nullptr);
  delegate_ = delegate;
  NativeApplicationInitialize(delegate_);
  while (true) {
    if (PollNativeApplicationEvent()) break;
    InvokeLoop();
    DoRemoveLoop();
  }
}

void Application::Quit() {
  NativeApplicationFinalize(delegate_);
}

LoopID Application::AddLoop(Loop loop) {
  ++current_id_;
  loop_list_.push_back(eastl::make_tuple(current_id_, loop));
  return current_id_;
}

void Application::RemoveLoop(LoopID id) {
  remove_id_.push_back(id);
}

void Application::InvokeLoop() {
  for (auto &pair : loop_list_) {
    auto &id = eastl::get<0>(pair);
    if (eastl::find(remove_id_.begin(), remove_id_.end(), id) == remove_id_.end()) {
      eastl::get<1>(pair)();
    }
  }
}

void Application::DoRemoveLoop() {
  for (auto &id : remove_id_) {
    for (auto iterator = loop_list_.begin(); iterator != loop_list_.end();) {
      if (eastl::get<0>(*iterator) == id) {
        iterator = loop_list_.erase(iterator);
      } else {
        ++iterator;
      }
    }
  }
  remove_id_.clear();
}

} // namespace xEngine
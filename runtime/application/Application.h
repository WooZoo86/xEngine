#ifndef XENGINE_APPLICATION_APPLICATION_H
#define XENGINE_APPLICATION_APPLICATION_H

#include "core/Types.h"

#include <EASTL/functional.h>
#include <EASTL/list.h>
#include <EASTL/tuple.h>
#include <EASTL/vector.h>

namespace xEngine {

typedef uint32 LoopID;

enum: LoopID {
  kInvalidLoopID = 0,
};

typedef eastl::function<void()> Loop;

class ApplicationDelegate;

class Application {
 public:
  static Application &GetInstance() {
    static Application instance;
    return instance;
  }

  void Run(ApplicationDelegate *delegate);

  void Quit();

  LoopID AddLoop(Loop loop);

  void RemoveLoop(LoopID id);

 private:
  void InvokeLoop();

  void DoRemoveLoop();

 private:
  ApplicationDelegate *delegate_{nullptr};
  LoopID current_id_{kInvalidLoopID};
  eastl::list<eastl::tuple<LoopID, Loop>> loop_list_;
  eastl::vector<LoopID> remove_id_;
};

} // namespace xEngine

#endif // XENGINE_APPLICATION_APPLICATION_H
#ifndef XENGINE_APPLICATION_APPLICATION_H
#define XENGINE_APPLICATION_APPLICATION_H

#include "core/Types.h"

#include <EASTL/list.h>

namespace xEngine {

class ApplicationDelegate;

class ApplicationLoopDelegate;

class Application {
 public:
  static Application &GetInstance() {
    static Application instance;
    return instance;
  }

  void Run(ApplicationDelegate *delegate);

  void Quit();

  void AddLoopDelegate(ApplicationLoopDelegate *delegate);

  void RemoveLoopDelegate(ApplicationLoopDelegate *delegate);

 private:
  void InvokeBeforeEvent();

  void InvokeAfterEvent();

 private:
  ApplicationDelegate *delegate_{nullptr};
  eastl::list<ApplicationLoopDelegate *> loop_delegate_list_;
};

} // namespace xEngine

#endif // XENGINE_APPLICATION_APPLICATION_H
#ifndef XENGINE_WINDOW_WINDOWFACTORY_H
#define XENGINE_WINDOW_WINDOWFACTORY_H

#include "WindowInterface.h"

#include <resource/ResourceFactory.h>
#include <resource/ResourceIdentity.h>

#include <EASTL/unique_ptr.h>

namespace xEngine {

class WindowResource;

class WindowFactory: public ResourceFactory<WindowResource> {
 public:
  virtual void Create(WindowResource &resource) override;

  virtual void Destroy(WindowResource &resource) override;
};

class WindowResource: public Resource<WindowConfig> {
 public:
  const eastl::unique_ptr<WindowInterface> &window() const { return window_; }

 private:
  eastl::unique_ptr<WindowInterface> window_{nullptr};

  friend void WindowFactory::Create(WindowResource &resource);
};

}

#endif // XENGINE_WINDOW_WINDOWFACTORY_H
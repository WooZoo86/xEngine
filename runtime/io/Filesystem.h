#ifndef XENGINE_IO_FILESYSTEM_H
#define XENGINE_IO_FILESYSTEM_H

#include "IOMessage.h"

#include <EASTL/shared_ptr.h>

#define FS_CREATOR_DECLARE(clazz) \
  static FilesystemPtr Creator() \
  { \
    return eastl::static_pointer_cast<Filesystem>(eastl::make_shared<clazz>()); \
  }

namespace xEngine {

class Filesystem {
 public:
  typedef eastl::function<eastl::shared_ptr<Filesystem>()> Creator;

  virtual ~Filesystem() {}

  virtual DataPtr Read(Location location) = 0;

  virtual void Write(Location location, DataPtr data) = 0;

  virtual IOStatus last_status() const = 0;
};

PTR_DECLARE(Filesystem)

}

#endif // XENGINE_IO_FILESYSTEM_H
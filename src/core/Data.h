#ifndef XENGINE_CORE_DATA_H
#define XENGINE_CORE_DATA_H

#include "Core.h"

#include <EASTL/allocator.h>
#include <EASTL/shared_ptr.h>

namespace xEngine {

class Data final {
 public:
  CREATE_FUNC_DECLARE(Data)

  Data() {}

  explicit Data(size_t size) : size_(size) { if (size_ > 0) buffer_ = static_cast<char *>(eastl::GetDefaultAllocator()->allocate(size_)); }

  Data(const char *buffer, size_t size) { Copy(buffer, size); }

  Data(Data &other) { Copy(other.buffer_, other.size_); }

  Data(Data &&other) { Assign(other.buffer_, other.size_); other.buffer_ = nullptr; other.size_ = 0; }

  ~Data() { if (buffer_ != nullptr) free(buffer_); }

  void operator=(Data &other) { Copy(other.buffer_, other.size_); }

  void operator=(Data &&other) { Assign(other.buffer_, other.size_); other.buffer_ = nullptr; other.size_ = 0; }

  bool operator==(const Data &other) { return size_ == other.size_ && buffer_ == other.buffer_; }

  bool Empty() { return buffer_ == nullptr || size_ == 0; }

  void Assign(char *buffer, size_t size, size_t offset = 0);

  void Copy(const char *buffer, size_t size, size_t offset = 0);

  void Append(const char *buffer, size_t size, size_t offset = 0);

  void Append(const Data &other) { Append(other.buffer_, other.size_); }

  size_t size() const { return size_; }

  void set_size(size_t size);

  const char *buffer() const { return buffer_; }

  char *buffer() { return buffer_; }

 private:
  size_t size_{0};
  char *buffer_{nullptr};
};

typedef eastl::shared_ptr<Data> DataPtr;

} // namespace xEngine

#endif //XENGINE_CORE_DATA_H

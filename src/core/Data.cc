#include "core/Data.h"

namespace xEngine {

void Data::Assign(char *buffer, size_t size) {
  if (buffer_ != nullptr) {
    eastl::GetDefaultAllocator()->deallocate(buffer_, size_);
  }
  buffer_ = buffer;
  size_ = size;
}

void Data::Copy(const char *buffer, size_t size, size_t offset) {
  set_size(size);
  eastl::copy(buffer + offset, buffer + offset + size_, buffer_);
}

void Data::Append(const char *buffer, size_t size, size_t offset) {
  auto orig_size = size_;
  set_size(size_ + size);
  eastl::copy(buffer + offset, buffer + offset + size, buffer_ + orig_size);
}

void Data::set_size(size_t size) {
  if (size_ != size) {
    if (buffer_ != nullptr) {
      eastl::GetDefaultAllocator()->deallocate(buffer_, size_);
      buffer_ = nullptr;
    }
    size_ = size;
    if (size_ > 0) {
      buffer_ = static_cast<char *>(eastl::GetDefaultAllocator()->allocate(size_));
    }
  }
}

} // namespace xEngine
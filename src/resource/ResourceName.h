#ifndef XENGINE_RESOURCE_RESOURCENAME_H
#define XENGINE_RESOURCE_RESOURCENAME_H

#include "core/Core.h"
#include "core/Types.h"

#include <EASTL/string.h>
#include <EASTL/hash_map.h>

namespace xEngine {

typedef uint32 ResourceSignature;

enum: ResourceSignature {
  kDefaultSignature = 0x7fffffff,
  kUniqueSignature = 0xffffffff,
};

class ResourceName {
 public:
  static ResourceName Unique() {
    ResourceName instance;
    instance.signature_ = kUniqueSignature;
    return instance;
  }

  static ResourceName Unique(const eastl::string &name) {
    ResourceName instance;
    instance.name_ = name;
    instance.signature_ = kUniqueSignature;
    return instance;
  }

  static ResourceName Unique(const char *name) {
    ResourceName instance;
    instance.name_ = name;
    instance.signature_ = kUniqueSignature;
    return instance;
  }

  static ResourceName Shared(ResourceSignature signature = kDefaultSignature) {
    x_assert(signature != kUniqueSignature);
    ResourceName instance;
    instance.signature_ = signature;
    return instance;
  }

  static ResourceName Shared(const eastl::string &name, ResourceSignature signature = kDefaultSignature) {
    x_assert(signature != kUniqueSignature);
    ResourceName instance;
    instance.name_ = name;
    instance.signature_ = signature;
    return instance;
  }

  static ResourceName Shared(const char *name, ResourceSignature signature = kDefaultSignature) {
    x_assert(signature != kUniqueSignature);
    ResourceName instance;
    instance.name_ = name;
    instance.signature_ = signature;
    return instance;
  }

  void operator=(const ResourceName &other) {
    name_ = other.name_;
    signature_ = other.signature_;
  }

  bool IsUnique() const {
    return signature_ == kUniqueSignature;
  }

  bool operator<(const ResourceName &other) const {
    return signature_ < other.signature_ || name_ < other.name_;
  }

  bool operator==(const ResourceName &other) const {
    return signature_ == other.signature_ && name_ == other.name_;
  }

  bool operator!=(const ResourceName &other) const {
    return signature_ != other.signature_ || name_ != other.name_;
  }

  const eastl::string &name() const { return name_; }

  size_t Hash() const { return eastl::hash<eastl::string>()(name_) ^ eastl::hash<int32>()(signature_); }

 private:
  eastl::string name_;
  ResourceSignature signature_{kDefaultSignature};
};

} // namespace xEngine

namespace eastl {

template<>
struct hash<xEngine::ResourceName> {
  size_t operator()(const xEngine::ResourceName &other) const { return other.Hash(); }
};

} // namespace eastl

#endif // XENGINE_RESOURCE_RESOURCENAME_H
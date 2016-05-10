#ifndef XENGINE_RESOURCE_RESOURCEPOOL_H
#define XENGINE_RESOURCE_RESOURCEPOOL_H

#include "Resource.h"

#include "core/Core.h"

#include <EASTL/queue.h>
#include <EASTL/vector.h>

namespace xEngine {

template<class RESOURCE, class CONFIG>
class ResourcePool {
 public:
  virtual ~ResourcePool() { x_assert(pool_id_ == kInvalidResourcePoolID); }

  void Initialize(uint16 size, ResourcePoolID pool_id);

  void Finalize();

  ResourceID Create();

  void Destroy(ResourceID id);

  RESOURCE &Get(ResourceID id, const CONFIG &config);

  RESOURCE *Find(ResourceID id);

  ResourcePoolID pool_id() const { return pool_id_; }

 protected:
  ResourceUniqueID next_unique_id_{0};
  ResourcePoolID pool_id_{kInvalidResourcePoolID};
  eastl::vector<RESOURCE> slots_;
  eastl::queue<uint16> available_slots_;
};

template<class RESOURCE, class CONFIG>
void ResourcePool<RESOURCE, CONFIG>::Initialize(uint16 size, ResourcePoolID pool_id) {
  x_assert(pool_id_ == kInvalidResourcePoolID);
  pool_id_ = pool_id;
  slots_.resize(size);
  for (auto i = 0; i < size; ++i) {
    available_slots_.push(i);
  }
}

template<class RESOURCE, class CONFIG>
void ResourcePool<RESOURCE, CONFIG>::Finalize() {
  x_assert(pool_id_ != kInvalidResourcePoolID);
  pool_id_ = kInvalidResourcePoolID;
  slots_.clear();
  while (!available_slots_.empty()) {
    available_slots_.pop();
  }
}

template<class RESOURCE, class CONFIG>
ResourceID ResourcePool<RESOURCE, CONFIG>::Create() {
  x_assert(pool_id_ != kInvalidResourcePoolID && !available_slots_.empty());
  auto slotID = available_slots_.front();
  available_slots_.pop();
  return GenerateResourceID(next_unique_id_++, slotID, pool_id_);
}

template<class RESOURCE, class CONFIG>
void ResourcePool<RESOURCE, CONFIG>::Destroy(ResourceID id) {
  x_assert(pool_id_ != kInvalidResourcePoolID);
  auto &slot = slots_[GetSlotIDOfResourceID(id)];
  x_assert(slot.ID == id);
  x_assert(slot.Status != ResourceStatus::kInvalid);
  slot.Finalize();
  available_slots_.push(GetSlotIDOfResourceID(id));
}

template<class RESOURCE, class CONFIG>
RESOURCE &ResourcePool<RESOURCE, CONFIG>::Get(ResourceID id, const CONFIG &config) {
  x_assert(pool_id_ != kInvalidResourcePoolID);
  auto &slot = slots_[GetSlotIDOfResourceID(id)];
  x_assert(slot.Status == ResourceStatus::kInvalid);
  slot.Initialize(id, config);
  return slot;
}

template<class RESOURCE, class CONFIG>
RESOURCE *ResourcePool<RESOURCE, CONFIG>::Find(ResourceID id) {
  x_assert(pool_id_ != kInvalidResourcePoolID);
  RESOURCE *resource = nullptr;
  if (GetResourcePoolIDOfResourceID(id) == pool_id_) {
    auto &slot = slots_[GetSlotIDOfResourceID(id)];
    resource = slot.Status == ResourceStatus::kInvalid ? nullptr : &slot;
  }
  return resource;
}

} // namespace xEngine

#endif // XENGINE_RESOURCE_RESOURCEPOOL_H
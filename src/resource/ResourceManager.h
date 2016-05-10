#ifndef XENGINE_RESOURCE_RESOURCEMANAGER_H
#define XENGINE_RESOURCE_RESOURCEMANAGER_H

#include "Resource.h"
#include "ResourceName.h"

#include <EASTL/hash_map.h>
#include <EASTL/vector.h>

namespace xEngine {

class ResourceManager {
 public:
  void Add(const ResourceName &name, ResourceID id);

  void Remove(ResourceID id);

  void RemoveAll();

  ResourceID Find(const ResourceName &name) const;

  bool Contains(ResourceID id) const;

  size_t Size() const { return resource_id_cache_.size(); }

 protected:
  eastl::vector<ResourceID> resource_id_cache_;
  eastl::hash_map<ResourceName, ResourceID> name_to_id_;
  eastl::hash_map<ResourceID, ResourceName> id_to_name_;
};

} // namespace xEngine

#endif // XENGINE_RESOURCE_RESOURCEMANAGER_H
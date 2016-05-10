#include "ResourceManager.h"

#include <EASTL/algorithm.h>

namespace xEngine {

void ResourceManager::Add(const ResourceName &name, ResourceID id) {
  resource_id_cache_.push_back(id);
  if (!name.IsUnique()) {
    name_to_id_.insert({name, id});
    id_to_name_.insert({id, name});
  }
}

void ResourceManager::Remove(ResourceID id) {
  auto cache = eastl::find(resource_id_cache_.begin(), resource_id_cache_.end(), id);
  if (cache != resource_id_cache_.end()) {
    resource_id_cache_.erase(cache);
    auto node = id_to_name_.find(id);
    if (node != id_to_name_.end()) {
      name_to_id_.erase(node->second);
      id_to_name_.erase(id);
    }
  }
}

void ResourceManager::RemoveAll() {
  resource_id_cache_.clear();
  name_to_id_.clear();
}

ResourceID ResourceManager::Find(const ResourceName &name) const {
  ResourceID id = kInvalidResourceID;
  if (!name.IsUnique()) {
    auto node = name_to_id_.find(name);
    if (node != name_to_id_.end()) {
      id = node->second;
    }
  }
  return id;
}

bool ResourceManager::Contains(ResourceID id) const {
  return eastl::find(resource_id_cache_.begin(), resource_id_cache_.end(), id) != resource_id_cache_.end();
}

} // namespace xEngine
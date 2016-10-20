#ifndef XENGINE_PLUGIN_NUKLEAR_NUKLEARGUI_H
#define XENGINE_PLUGIN_NUKLEAR_NUKLEARGUI_H

#include "NuklearConfig.h"

#include <graphics/Graphics.h>

#define NK_ASSERT(expr) x_assert(expr)
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS
#include <nuklear.h>

#include <EASTL/vector.h>

namespace xEngine {

class Graphics;

class WindowInterface;

class NuklearGUI {
 public:
  static NuklearGUI &GetInstance() {
    static NuklearGUI instance;
    return instance;
  }

 public:
  void Initialize(NuklearConfig config = NuklearConfig());

  void Finalize();

  bool Available() const { return graphics_ != nullptr; }

  void BeginFrame();

  void EndFrame();

  nk_context *context() { return &context_; }

 private:
  NuklearConfig gui_config_;
  WindowInterface *window_{nullptr};
  Graphics *graphics_{nullptr};
  nk_convert_config config_;
  nk_context context_;
  nk_font_atlas font_atlas_;
  nk_buffer command;
  void *vertex_data{nullptr};
  void *index_data{nullptr};
  nk_buffer vertex_buffer;
  nk_buffer index_buffer;
  eastl::vector<ResourceID> image_;
  ResourceID shader_{kInvalidResourceID};
  ResourceID mesh_{kInvalidResourceID};
  ResourceID pipeline_{kInvalidResourceID};
  ResourceID sampler_{kInvalidResourceID};
};

} // namespace xEngine

#endif // XENGINE_PLUGIN_NUKLEAR_NUKLEAR_H
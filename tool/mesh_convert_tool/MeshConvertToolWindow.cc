#include "MeshConvertToolWindow.h"
#include "MeshConverter.h"

#include <application/Application.h>
#include <window/Window.h>

namespace xEngine {

static const auto window_width = 800;

static const auto window_height = 600;

MeshConvertToolWindow::MeshConvertToolWindow() {
  x_assert(Window::GetInstance().Available());
  nuklear_ = NuklearGUI::CreateUnique();
  window_id_ = Window::GetInstance().Create(WindowConfig::ForWindow(this, window_width, window_height, "MeshConvertTool"));
  Window::GetInstance().GetGraphics(window_id_)->Initialize(GraphicsConfig::ForWindow(window_id_));
  Window::GetInstance().GetGraphics(window_id_)->renderer()->MakeCurrent();
  nuklear_->Initialize(NuklearGUIConfig::ForWindow(window_id_));
}

MeshConvertToolWindow::~MeshConvertToolWindow() {
  if (window_id_ != kInvalidResourceID) {
    MeshConvertToolWindow::OnWindowClose();
  }
}

void MeshConvertToolWindow::OnWindowUpdate() {
  Draw();
}

void MeshConvertToolWindow::OnWindowClose() {
  nuklear_->Finalize();
  Window::GetInstance().GetGraphics(window_id_)->Finalize();
  Window::GetInstance().Destroy(window_id_);
  window_id_ = kInvalidResourceID;
#if X_WINDOWS
  Application::GetInstance().Quit();
#endif
}

void MeshConvertToolWindow::OnWindowDropFile(eastl::vector<eastl::string> files) {
  for (auto file : files) {
    auto out_file = file;
    auto position = out_file.find_last_of('.');
    if (position == eastl::string::npos) {
      Log::GetInstance().Error("cannot resolve file extension name of %s\n", file.c_str());
      continue;
    }
    out_file.replace(position, out_file.length() - position, ".xmesh");
    MeshConverter::Convert(file, out_file);
  }
}

void MeshConvertToolWindow::Draw() {
  auto &renderer = Window::GetInstance().GetGraphics(window_id_)->renderer();
  renderer->MakeCurrent();
  renderer->ApplyTarget();
  DrawGUI();
  renderer->Render();
}

void MeshConvertToolWindow::DrawGUI() {
  auto &window_config = Window::GetInstance().Get(window_id_)->config();
  nuklear_->BeginFrame();
  auto ctx = nuklear_->context();
  nk_begin(ctx, "", nk_rect(-1, -1, window_config.width + 2, window_config.height + 2), NK_WINDOW_BACKGROUND);

  if (nk_tree_push(ctx, NK_TREE_NODE, "PostProcessFlag", NK_MAXIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "triangulate", &MeshConverter::post_process_flag.triangulate);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MAXIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "texcoord 0", &MeshConverter::remove_component_flag.texcoord0);
    nk_checkbox_label(ctx, "texcoord 1", &MeshConverter::remove_component_flag.texcoord1);
    nk_checkbox_label(ctx, "texcoord 2", &MeshConverter::remove_component_flag.texcoord2);
    nk_checkbox_label(ctx, "texcoord 3", &MeshConverter::remove_component_flag.texcoord3);
    nk_checkbox_label(ctx, "color 0", &MeshConverter::remove_component_flag.color0);
    nk_checkbox_label(ctx, "color 1", &MeshConverter::remove_component_flag.color1);
    nk_checkbox_label(ctx, "normal", &MeshConverter::remove_component_flag.normal);
    nk_checkbox_label(ctx, "tangent", &MeshConverter::remove_component_flag.tangent);
    nk_checkbox_label(ctx, "binormal", &MeshConverter::remove_component_flag.binormal);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MAXIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "point", &MeshConverter::remove_topology_flag.point);
    nk_checkbox_label(ctx, "line", &MeshConverter::remove_topology_flag.line);
    nk_checkbox_label(ctx, "triangle", &MeshConverter::remove_topology_flag.triangle);
    nk_checkbox_label(ctx, "polygon", &MeshConverter::remove_topology_flag.polygon);
    nk_tree_pop(ctx);
  }

  nk_end(ctx);
  nuklear_->EndFrame();
}

} // namespace xEngine

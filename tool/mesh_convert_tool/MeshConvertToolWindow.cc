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
    nk_checkbox_label(ctx, "calculate tangent space", &MeshConverter::PostProcessFlag::calc_tangent_space);
    nk_checkbox_label(ctx, "join identical vertices", &MeshConverter::PostProcessFlag::join_identical_vertices);
    nk_checkbox_label(ctx, "make left handed", &MeshConverter::PostProcessFlag::make_left_handed);
    nk_checkbox_label(ctx, "triangulate", &MeshConverter::PostProcessFlag::triangulate);
    nk_checkbox_label(ctx, "remove component", &MeshConverter::PostProcessFlag::remove_component);
    nk_checkbox_label(ctx, "generate normals", &MeshConverter::PostProcessFlag::gen_normals);
    nk_checkbox_label(ctx, "generate smooth normals", &MeshConverter::PostProcessFlag::gen_smooth_normals);
    nk_checkbox_label(ctx, "split large meshes", &MeshConverter::PostProcessFlag::split_large_meshes);
    nk_checkbox_label(ctx, "transform vertices", &MeshConverter::PostProcessFlag::pre_transform_vertices);
    nk_checkbox_label(ctx, "limit bone weights", &MeshConverter::PostProcessFlag::limit_bone_weights);
    nk_checkbox_label(ctx, "validate data structure", &MeshConverter::PostProcessFlag::validate_data_structure);
    nk_checkbox_label(ctx, "improve cache locality", &MeshConverter::PostProcessFlag::improve_cache_locality);
    nk_checkbox_label(ctx, "remove redundant materials", &MeshConverter::PostProcessFlag::remove_redundant_materials);
    nk_checkbox_label(ctx, "fix infacing normals", &MeshConverter::PostProcessFlag::fix_infacing_normals);
    nk_checkbox_label(ctx, "sort by topology type", &MeshConverter::PostProcessFlag::sort_by_ptype);
    nk_checkbox_label(ctx, "find degenerates", &MeshConverter::PostProcessFlag::find_degenerates);
    nk_checkbox_label(ctx, "find invalid data", &MeshConverter::PostProcessFlag::find_invalid_data);
    nk_checkbox_label(ctx, "gen uv coords", &MeshConverter::PostProcessFlag::gen_uv_coords);
    nk_checkbox_label(ctx, "transform uv coords", &MeshConverter::PostProcessFlag::transform_uv_coords);
    nk_checkbox_label(ctx, "find instances", &MeshConverter::PostProcessFlag::find_instances);
    nk_checkbox_label(ctx, "optimize meshes", &MeshConverter::PostProcessFlag::optimize_meshes);
    nk_checkbox_label(ctx, "optimize graph", &MeshConverter::PostProcessFlag::optimize_graph);
    nk_checkbox_label(ctx, "flip uvs", &MeshConverter::PostProcessFlag::flip_uvs);
    nk_checkbox_label(ctx, "flip winding order", &MeshConverter::PostProcessFlag::flip_winding_order);
    nk_checkbox_label(ctx, "split by bone count", &MeshConverter::PostProcessFlag::split_by_bone_count);
    nk_checkbox_label(ctx, "debone", &MeshConverter::PostProcessFlag::debone);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MINIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "normals", &MeshConverter::RemoveComponentFlag::normals);
    nk_checkbox_label(ctx, "tangents & bitangents", &MeshConverter::RemoveComponentFlag::tangents_and_bitangents);
    nk_checkbox_label(ctx, "colors", &MeshConverter::RemoveComponentFlag::colors);
    nk_checkbox_label(ctx, "colors 0", &MeshConverter::RemoveComponentFlag::colors0);
    nk_checkbox_label(ctx, "colors 1", &MeshConverter::RemoveComponentFlag::colors1);
    nk_checkbox_label(ctx, "colors 2", &MeshConverter::RemoveComponentFlag::colors2);
    nk_checkbox_label(ctx, "colors 3", &MeshConverter::RemoveComponentFlag::colors3);
    nk_checkbox_label(ctx, "texcoords", &MeshConverter::RemoveComponentFlag::texcoords);
    nk_checkbox_label(ctx, "texcoords 0", &MeshConverter::RemoveComponentFlag::texcoords0);
    nk_checkbox_label(ctx, "texcoords 1", &MeshConverter::RemoveComponentFlag::texcoords1);
    nk_checkbox_label(ctx, "texcoords 2", &MeshConverter::RemoveComponentFlag::texcoords2);
    nk_checkbox_label(ctx, "texcoords 3", &MeshConverter::RemoveComponentFlag::texcoords3);
    nk_checkbox_label(ctx, "boneweights", &MeshConverter::RemoveComponentFlag::boneweights);
    nk_checkbox_label(ctx, "animations", &MeshConverter::RemoveComponentFlag::animations);
    nk_checkbox_label(ctx, "textures", &MeshConverter::RemoveComponentFlag::textures);
    nk_checkbox_label(ctx, "lights", &MeshConverter::RemoveComponentFlag::lights);
    nk_checkbox_label(ctx, "cameras", &MeshConverter::RemoveComponentFlag::cameras);
    nk_checkbox_label(ctx, "meshes", &MeshConverter::RemoveComponentFlag::meshes);
    nk_checkbox_label(ctx, "materials", &MeshConverter::RemoveComponentFlag::materials);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MINIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "point", &MeshConverter::RemoveTopologyFlag::point);
    nk_checkbox_label(ctx, "line", &MeshConverter::RemoveTopologyFlag::line);
    nk_checkbox_label(ctx, "triangle", &MeshConverter::RemoveTopologyFlag::triangle);
    nk_checkbox_label(ctx, "polygon", &MeshConverter::RemoveTopologyFlag::polygon);
    nk_tree_pop(ctx);
  }

  nk_end(ctx);
  nuklear_->EndFrame();
}

} // namespace xEngine

#include "MeshConvertToolWindow.h"
#include "MeshConvertor.h"

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
    MeshConvertor::Convert(file, out_file);
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
    nk_checkbox_label(ctx, "calculate tangent space", &MeshConvertor::PostProcessFlag::calc_tangent_space);
    nk_checkbox_label(ctx, "join identical vertices", &MeshConvertor::PostProcessFlag::join_identical_vertices);
    nk_checkbox_label(ctx, "make left handed", &MeshConvertor::PostProcessFlag::make_left_handed);
    nk_checkbox_label(ctx, "triangulate", &MeshConvertor::PostProcessFlag::triangulate);
    nk_checkbox_label(ctx, "remove component", &MeshConvertor::PostProcessFlag::remove_component);
    nk_checkbox_label(ctx, "generate normals", &MeshConvertor::PostProcessFlag::gen_normals);
    nk_checkbox_label(ctx, "generate smooth normals", &MeshConvertor::PostProcessFlag::gen_smooth_normals);
    nk_checkbox_label(ctx, "split large meshes", &MeshConvertor::PostProcessFlag::split_large_meshes);
    nk_checkbox_label(ctx, "transform vertices", &MeshConvertor::PostProcessFlag::pre_transform_vertices);
    nk_checkbox_label(ctx, "limit bone weights", &MeshConvertor::PostProcessFlag::limit_bone_weights);
    nk_checkbox_label(ctx, "validate data structure", &MeshConvertor::PostProcessFlag::validate_data_structure);
    nk_checkbox_label(ctx, "improve cache locality", &MeshConvertor::PostProcessFlag::improve_cache_locality);
    nk_checkbox_label(ctx, "remove redundant materials", &MeshConvertor::PostProcessFlag::remove_redundant_materials);
    nk_checkbox_label(ctx, "fix infacing normals", &MeshConvertor::PostProcessFlag::fix_infacing_normals);
    nk_checkbox_label(ctx, "sort by topology type", &MeshConvertor::PostProcessFlag::sort_by_ptype);
    nk_checkbox_label(ctx, "find degenerates", &MeshConvertor::PostProcessFlag::find_degenerates);
    nk_checkbox_label(ctx, "find invalid data", &MeshConvertor::PostProcessFlag::find_invalid_data);
    nk_checkbox_label(ctx, "gen uv coords", &MeshConvertor::PostProcessFlag::gen_uv_coords);
    nk_checkbox_label(ctx, "transform uv coords", &MeshConvertor::PostProcessFlag::transform_uv_coords);
    nk_checkbox_label(ctx, "find instances", &MeshConvertor::PostProcessFlag::find_instances);
    nk_checkbox_label(ctx, "optimize meshes", &MeshConvertor::PostProcessFlag::optimize_meshes);
    nk_checkbox_label(ctx, "optimize graph", &MeshConvertor::PostProcessFlag::optimize_graph);
    nk_checkbox_label(ctx, "flip uvs", &MeshConvertor::PostProcessFlag::flip_uvs);
    nk_checkbox_label(ctx, "flip winding order", &MeshConvertor::PostProcessFlag::flip_winding_order);
    nk_checkbox_label(ctx, "split by bone count", &MeshConvertor::PostProcessFlag::split_by_bone_count);
    nk_checkbox_label(ctx, "debone", &MeshConvertor::PostProcessFlag::debone);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MINIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "normals", &MeshConvertor::RemoveComponentFlag::normals);
    nk_checkbox_label(ctx, "tangents & bitangents", &MeshConvertor::RemoveComponentFlag::tangents_and_bitangents);
    nk_checkbox_label(ctx, "colors", &MeshConvertor::RemoveComponentFlag::colors);
    nk_checkbox_label(ctx, "colors 0", &MeshConvertor::RemoveComponentFlag::colors0);
    nk_checkbox_label(ctx, "colors 1", &MeshConvertor::RemoveComponentFlag::colors1);
    nk_checkbox_label(ctx, "colors 2", &MeshConvertor::RemoveComponentFlag::colors2);
    nk_checkbox_label(ctx, "colors 3", &MeshConvertor::RemoveComponentFlag::colors3);
    nk_checkbox_label(ctx, "texcoords", &MeshConvertor::RemoveComponentFlag::texcoords);
    nk_checkbox_label(ctx, "texcoords 0", &MeshConvertor::RemoveComponentFlag::texcoords0);
    nk_checkbox_label(ctx, "texcoords 1", &MeshConvertor::RemoveComponentFlag::texcoords1);
    nk_checkbox_label(ctx, "texcoords 2", &MeshConvertor::RemoveComponentFlag::texcoords2);
    nk_checkbox_label(ctx, "texcoords 3", &MeshConvertor::RemoveComponentFlag::texcoords3);
    nk_checkbox_label(ctx, "boneweights", &MeshConvertor::RemoveComponentFlag::boneweights);
    nk_checkbox_label(ctx, "animations", &MeshConvertor::RemoveComponentFlag::animations);
    nk_checkbox_label(ctx, "textures", &MeshConvertor::RemoveComponentFlag::textures);
    nk_checkbox_label(ctx, "lights", &MeshConvertor::RemoveComponentFlag::lights);
    nk_checkbox_label(ctx, "cameras", &MeshConvertor::RemoveComponentFlag::cameras);
    nk_checkbox_label(ctx, "meshes", &MeshConvertor::RemoveComponentFlag::meshes);
    nk_checkbox_label(ctx, "materials", &MeshConvertor::RemoveComponentFlag::materials);
    nk_tree_pop(ctx);
  }

  if (nk_tree_push(ctx, NK_TREE_NODE, "RemoveComponentFlag", NK_MINIMIZED)) {
    nk_layout_row_dynamic(ctx, 30, 3);
    nk_checkbox_label(ctx, "point", &MeshConvertor::RemoveTopologyFlag::point);
    nk_checkbox_label(ctx, "line", &MeshConvertor::RemoveTopologyFlag::line);
    nk_checkbox_label(ctx, "triangle", &MeshConvertor::RemoveTopologyFlag::triangle);
    nk_checkbox_label(ctx, "polygon", &MeshConvertor::RemoveTopologyFlag::polygon);
    nk_tree_pop(ctx);
  }

  nk_end(ctx);
  nuklear_->EndFrame();
}

} // namespace xEngine

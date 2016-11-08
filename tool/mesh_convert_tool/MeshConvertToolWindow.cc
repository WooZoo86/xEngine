#include "MeshConvertToolWindow.h"

#include <application/Application.h>
#include <window/Window.h>

namespace xEngine {

static const auto window_width = 600;

static const auto window_height = 400;

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
  nk_begin(ctx, "", nk_rect(0, 0, window_config.width, window_config.height), NK_WINDOW_NO_SCROLLBAR);
  nk_layout_row_static(ctx, 200, 50, 1);
  nk_button_label(ctx, "click");
  nk_end(ctx);
  nuklear_->EndFrame();
}

} // namespace xEngine

// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#include <rex/rex_app.h>
#include <imgui.h>
#include <algorithm>

namespace soulcalibur {
// The dialog owns itself through ReXGlue's ImGuiDialog lifecycle.
// Runtime initialization resumes only after the user chooses Play.
class LauncherDialog final : public rex::ui::ImGuiDialog {
  rex::PathConfig paths_;
  std::function<void(rex::PathConfig)> resume_;
  rex::ui::Window* window_;
  bool fullscreen_=false;
  bool ready_=false;
  bool launched_=false;
 public:
  LauncherDialog(rex::ui::ImGuiDrawer* drawer, rex::ui::Window* window,
                 rex::PathConfig paths, std::function<void(rex::PathConfig)> resume)
      : ImGuiDialog(drawer), paths_(std::move(paths)), resume_(std::move(resume)), window_(window) {
    std::error_code error;
    ready_=std::filesystem::is_regular_file(paths_.game_data_root / "default.xex",error);
    window_->SetTitle("SOULCALIBUR HD | Launcher");
    fullscreen_=window_->IsFullscreen();
  }
 protected:
  void OnDraw(ImGuiIO& io) override {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    auto* viewport=ImGui::GetMainViewport();
    const float width=std::min(840.0f,std::max(320.0f,viewport->WorkSize.x-32.0f));
    ImGui::SetNextWindowPos(viewport->GetWorkCenter(),ImGuiCond_Always,ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(width,std::min(600.0f,viewport->WorkSize.y-32.0f)),ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(32,28));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,14);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,6);
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(.055f,.065f,.09f,1));
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.38f,.28f,.10f,1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.56f,.42f,.17f,1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(.67f,.50f,.20f,1));
    ImGui::PushStyleColor(ImGuiCol_Border,ImVec4(.45f,.35f,.18f,1));
    ImGui::PushStyleColor(ImGuiCol_Tab,ImVec4(.14f,.16f,.20f,1));
    ImGui::PushStyleColor(ImGuiCol_TabHovered,ImVec4(.40f,.31f,.14f,1));
    ImGui::PushStyleColor(ImGuiCol_TabSelected,ImVec4(.38f,.28f,.10f,1));
    ImGui::PushStyleColor(ImGuiCol_Separator,ImVec4(.27f,.24f,.18f,1));
    ImGui::PushFont(nullptr,18.0f);
    ImGui::Begin("Soulcalibur launcher",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
    ImGui::PushFont(nullptr,32.0f);
    ImGui::TextColored(ImVec4(.83f,.68f,.37f,1),"SOULCALIBUR HD");
    ImGui::PopFont();
    ImGui::Text("The legend returns.");
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    bool play=false,quit=false;
    if(ImGui::BeginTabBar("Launcher sections")) {
      if(ImGui::BeginTabItem("Play")) {
        ImGui::Spacing();
        ImGui::TextWrapped("Choose your fighter and step into the arena. Full-game mode is enabled for your local copy.");
        ImGui::Spacing();
        ImGui::TextColored(ready_?ImVec4(.45f,.85f,.61f,1):ImVec4(1,.48f,.38f,1),ready_?"Game files found":"Game files missing");
        if(!ready_) ImGui::TextWrapped("Place your legally dumped game in the SoulCalibur folder, then reopen the launcher.");
        ImGui::Spacing();
        ImGui::BeginDisabled(!ready_ || launched_);
        play=ImGui::Button("PLAY GAME",ImVec2(-1,56));
        ImGui::SetItemDefaultFocus();
        ImGui::EndDisabled();
        ImGui::Spacing();
        ImGui::TextDisabled("Offline prototype  |  Local versus supported");
        ImGui::TextWrapped("Audio and save/load are still being verified. Online play is planned for a later version.");
        ImGui::EndTabItem();
      }
      if(ImGui::BeginTabItem("Settings")) {
        ImGui::Spacing();
        ImGui::Checkbox("Start game in fullscreen",&fullscreen_);
        ImGui::Spacing(); ImGui::Text("Game folder");
        ImGui::TextWrapped("%s",paths_.game_data_root.string().c_str());
        ImGui::Spacing(); ImGui::TextWrapped("Settings apply when you press Play. Your original game files remain unchanged.");
        ImGui::EndTabItem();
      }
      if(ImGui::BeginTabItem("Controls")) {
        ImGui::Spacing(); ImGui::TextWrapped("Controllers use the game's standard layout. Two players can also share a keyboard.");
        if(ImGui::BeginTable("Keyboard",3,ImGuiTableFlags_RowBg|ImGuiTableFlags_BordersInnerH)) {
          ImGui::TableSetupColumn("Action"); ImGui::TableSetupColumn("Player 1"); ImGui::TableSetupColumn("Player 2"); ImGui::TableHeadersRow();
          const char* rows[][3]={{"Move","W A S D","I J K L"},{"Confirm / guard","Space","U"},{"Kick / back","Shift","H"},{"Weapon attacks","R / E","O / P"},{"Start / pause","Escape","Enter"}};
          for(const auto& row:rows) {ImGui::TableNextRow(); for(int column=0;column<3;++column) {ImGui::TableSetColumnIndex(column); ImGui::TextUnformatted(row[column]);}}
          ImGui::EndTable();
        }
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    quit=ImGui::Button("Exit",ImVec2(90,32));
    ImGui::End(); ImGui::PopFont(); ImGui::PopStyleColor(9); ImGui::PopStyleVar(3);
    if(play) {
      launched_=true;
      window_->SetFullscreen(fullscreen_);
      window_->SetTitle("SOULCALIBUR HD");
      Close();
      auto resume=std::move(resume_); resume(paths_);
    } else if(quit) { Close(); window_->RequestClose(); }
  }
};
}

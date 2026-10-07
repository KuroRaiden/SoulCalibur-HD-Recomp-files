// SPDX-License-Identifier: BSD-3-Clause
// soulcalibur_hd - ReXGlue Recompiled Project
//
// Customize your app by overriding virtual hooks from rex::ReXApp.

#pragma once

#include <rex/rex_app.h>
#include "menu/application_hooks.h"
#include "keyboard_controller.h"
#include "launcher_dialog.h"
#include <rex/input/input_system.h>
#include <rex/audio/audio_system.h>
#include "render/scene_renderer.h"

class SoulcaliburHdApp : public rex::ReXApp {
 public:
  using rex::ReXApp::ReXApp;
  soulcalibur::MenuApplicationHooks menu_hooks;
  soulcalibur::RendererPath renderer_path = soulcalibur::RendererPath::XenosReference;

  static std::unique_ptr<rex::ui::WindowedApp> Create(
      rex::ui::WindowedAppContext& ctx) {
    return std::unique_ptr<SoulcaliburHdApp>(new SoulcaliburHdApp(ctx, "soulcalibur_hd",
        PPCImageConfig));
  }

  void OnConfigurePaths(rex::PathConfig& paths) override {
    const auto project = std::filesystem::path(__FILE__).parent_path().parent_path();
    if (paths.game_data_root.empty()) paths.game_data_root = project / "SoulCalibur";
    paths.user_data_root = project / "out" / "userdata";
    paths.cache_root = project / "out" / "cache";
  }

  std::optional<rex::PathConfig> OnFinalizePaths(
      const rex::PathConfig& defaults,
      std::function<void(rex::PathConfig)> resume) override {
    new soulcalibur::LauncherDialog(imgui_drawer(), window(), defaults, std::move(resume));
    return std::nullopt;
  }

  void OnPostSetup() override {
    auto* input=dynamic_cast<rex::input::InputSystem*>(runtime()->input_system());
    if (!input || !window()) return;
    auto keyboard=std::make_unique<soulcalibur::KeyboardController>(window());
    keyboard->Setup(); keyboard->OnWindowAvailable(window());
    input->AddDriver(std::move(keyboard));
  }

  void OnWindowFocusChanged(bool focused) override {
    auto* audio=runtime() ? dynamic_cast<rex::audio::AudioSystem*>(runtime()->audio_system()) : nullptr;
    REXLOG_INFO("Game window focus: focused={} audio_paused={}",focused,audio ? audio->is_paused() : false);
  }

  // Override virtual hooks for customization:
  // void OnPostInitLogging() override {}
  // void OnPreSetup(rex::RuntimeConfig& config) override {}
  // void OnLoadXexImage(std::string& xex_image) override {}
  // void OnPostLoadXexImage() override {}
  // void OnPostSetup() override {}
  // void OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) override {}
  // std::unique_ptr<rex::ui::ImGuiDialog> CreateAchievementsOverlay() override;
  // std::unique_ptr<rex::ui::AchievementNotificationDialog>
  // CreateAchievementNotificationDialog() override;
  // void OnShutdown() override {}
  // void OnConfigurePaths(rex::PathConfig& paths) override {}
};

/*
 * Copyright (C) 2026 CrowdWare
 *
 * This file is part of SMLUI.
 *
 *  SMLUI is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SMLUI is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SMLUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SML_UI_H
#define SML_UI_H

#include <string>
#include <vector>

#include "imgui.h"
#include "sml_parser.h"

namespace smlui {

struct UiLabel {
    sml::Vec2i position = {0, 0};
    std::string text;
    int font_size = 13;
};

struct UiMenuItem {
    std::string label;
    std::string clicked;
    std::string action;
    int action_id = 0;
    bool use_on_mac = true;
    bool is_separator = false;
};

struct UiMenu {
    std::string label;
    std::vector<UiMenuItem> items;
};

struct UiMainMenu {
    bool enabled = false;
    std::vector<UiMenu> menus;
};

struct UiWindow {
    std::string title;
    sml::Vec2i position = {0, 0};
    sml::Vec2i size = {640, 480};
    UiLabel label;
    UiMainMenu main_menu;
    struct DockLayout {
        bool enabled = false;
        int top_height = 0;
        int bottom_height = 0;
        int left_width = 0;
        int right_width = 0;
        std::string top_label;
        std::string bottom_label;
        std::string left_label;
        std::string right_label;
        std::string center_label;
        bool show_menubar = false;
        bool show_toolbar = false;
        bool show_statusbar = false;
        bool show_property_panel = false;
        bool show_viewport = false;
        bool show_overlay = false;
        std::vector<std::string> toolbar_tools;
        std::vector<std::string> left_tools;
    } dock;
    struct UiState {
        std::string persist = "user";
        bool pos = false;
        bool size = false;
        bool maximized = false;
        bool last_file_path = false;
        bool docking = false;
        std::string last_file_path_value;
        std::string theme;
    } state;
};

struct UiTheme {
    ImVec4 toolbar_bg = ImVec4(0.14f, 0.14f, 0.15f, 1.0f);
    ImVec4 status_bg = ImVec4(0.19f, 0.39f, 0.72f, 1.0f);
    ImVec4 status_text = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 left_bg = ImVec4(0.16f, 0.16f, 0.17f, 1.0f);
    ImVec4 right_bg = ImVec4(0.15f, 0.15f, 0.16f, 1.0f);
    ImVec4 center_bg = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class UiDocument {
public:
    bool parseFromString(const std::string& text, std::string* error_message);
    const UiWindow& window() const { return window_; }
    void setTheme(const UiTheme& theme) { theme_ = theme; }
    using PropertyPanelCallback = void(*)(const ImVec2& panel_pos, const ImVec2& panel_size, void* user_data);
    void setPropertyPanelCallback(PropertyPanelCallback callback, void* user_data) {
        property_panel_callback_ = callback;
        property_panel_user_data_ = user_data;
    }
    using MenuActionCallback = void(*)(int action_id, void* user_data);
    void setMenuActionCallback(MenuActionCallback callback, void* user_data) {
        menu_action_callback_ = callback;
        menu_action_user_data_ = user_data;
    }
    void render(const ImGuiViewport* viewport, ImFont* font_15, bool* out_play_clicked) const;

private:
    UiWindow window_;
    UiTheme theme_;
    PropertyPanelCallback property_panel_callback_ = nullptr;
    void* property_panel_user_data_ = nullptr;
    MenuActionCallback menu_action_callback_ = nullptr;
    void* menu_action_user_data_ = nullptr;
};

} // namespace smlui

#endif

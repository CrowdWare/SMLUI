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

struct UiWindow {
    std::string title;
    sml::Vec2i position = {0, 0};
    sml::Vec2i size = {640, 480};
    UiLabel label;
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
    } state;
};

class UiDocument {
public:
    bool parseFromString(const std::string& text, std::string* error_message);
    const UiWindow& window() const { return window_; }
    void render(const ImGuiViewport* viewport, ImFont* font_15, bool* out_play_clicked) const;

private:
    UiWindow window_;
};

} // namespace smlui

#endif

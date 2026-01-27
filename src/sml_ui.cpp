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

#include "sml_ui.h"

#include <vector>
#include <cctype>
#include <algorithm>

namespace smlui {

static bool DrawPlayButton(const char* id) {
    ImVec2 size(28.0f, 28.0f);
    bool clicked = ImGui::InvisibleButton(id, size);
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
    ImU32 border = ImGui::GetColorU32(ImGuiCol_Border);
    ImU32 arrow = IM_COL32(40, 220, 90, 255);
    draw->AddRectFilled(min, max, bg, 4.0f);
    draw->AddRect(min, max, border, 4.0f);
    float pad = 7.0f;
    ImVec2 a(min.x + pad, min.y + pad);
    ImVec2 b(min.x + pad, max.y - pad);
    ImVec2 c(max.x - pad + 2.0f, (min.y + max.y) * 0.5f);
    draw->AddTriangleFilled(a, b, c, arrow);
    return clicked;
}

static bool DrawToolButton(const char* label, bool play_button) {
    if (play_button) {
        return DrawPlayButton(label);
    }
    ImVec2 size(28.0f, 28.0f);
    return ImGui::Button(label, size);
}

static std::string IconToLabel(const std::string& icon) {
    if (icon == "select") return "S";
    if (icon == "move") return "M";
    if (icon == "paint") return "P";
    if (icon == "play") return "Play";
    if (icon.empty()) return "?";
    return std::string(1, (char)std::toupper(static_cast<unsigned char>(icon[0])));
}

class UiSmlHandler : public sml::SmlHandler {
public:
    explicit UiSmlHandler(UiWindow* model) : model_(model) {}

    void startElement(const std::string& name) override {
        stack_.push_back(name);
        if (!model_)
            return;
        if (name == "MainMenu") {
            model_->main_menu.enabled = true;
            in_main_menu_ = true;
        } else if (name == "Menu" && in_main_menu_) {
            model_->main_menu.menus.push_back(UiMenu());
            current_menu_ = &model_->main_menu.menus.back();
        } else if (name == "MenuItem" && current_menu_) {
            current_menu_->items.push_back(UiMenuItem());
            current_item_ = &current_menu_->items.back();
        } else if (name == "Separator" && current_menu_) {
            UiMenuItem item;
            item.is_separator = true;
            current_menu_->items.push_back(item);
            current_item_ = nullptr;
        } else if (name == "DockLayout") {
            model_->dock.enabled = true;
        } else if (name == "MenuBar") {
            model_->dock.show_menubar = true;
        } else if (name == "ToolBar") {
            model_->dock.show_toolbar = true;
        } else if (name == "StatusBar") {
            model_->dock.show_statusbar = true;
        } else if (name == "PropertyPanel") {
            model_->dock.show_property_panel = true;
        } else if (name == "Viewport3D") {
            model_->dock.show_viewport = true;
        } else if (name == "Overlay") {
            model_->dock.show_overlay = true;
        }
    }

    void onProperty(const std::string& name, const sml::PropertyValue& value) override {
        if (!model_ || stack_.empty())
            return;
        const std::string& element = stack_.back();
        if (element == "Window") {
            if (name == "title" && value.type == sml::PropertyValue::String)
                model_->title = value.string_value;
            else if (name == "position" && value.type == sml::PropertyValue::Vec2iType)
                model_->position = value.vec2i_value;
            else if (name == "size" && value.type == sml::PropertyValue::Vec2iType)
                model_->size = value.vec2i_value;
        } else if (element == "state") {
            if (name == "persist" && value.type == sml::PropertyValue::EnumType)
                model_->state.persist = value.string_value;
            else if (name == "theme" && value.type == sml::PropertyValue::String)
                model_->state.theme = value.string_value;
            else if (name == "pos" && value.type == sml::PropertyValue::Boolean)
                model_->state.pos = value.bool_value;
            else if (name == "size" && value.type == sml::PropertyValue::Boolean)
                model_->state.size = value.bool_value;
            else if (name == "maximized" && value.type == sml::PropertyValue::Boolean)
                model_->state.maximized = value.bool_value;
            else if (name == "lastFilePath" && value.type == sml::PropertyValue::Boolean)
                model_->state.last_file_path = value.bool_value;
            else if (name == "docking" && value.type == sml::PropertyValue::Boolean)
                model_->state.docking = value.bool_value;
        } else if (element == "Label") {
            if (name == "position" && value.type == sml::PropertyValue::Vec2iType)
                model_->label.position = value.vec2i_value;
            else if (name == "text" && value.type == sml::PropertyValue::String)
                model_->label.text = value.string_value;
            else if (name == "fontSize" && value.type == sml::PropertyValue::Int)
                model_->label.font_size = value.int_value;
        } else if (element == "Menu" && current_menu_) {
            if (name == "label" && value.type == sml::PropertyValue::String)
                current_menu_->label = value.string_value;
        } else if (element == "MenuItem" && current_item_) {
            if (name == "label" && value.type == sml::PropertyValue::String)
                current_item_->label = value.string_value;
            else if (name == "clicked" && (value.type == sml::PropertyValue::String || value.type == sml::PropertyValue::EnumType))
                current_item_->clicked = value.string_value;
            else if (name == "useOnMac" && value.type == sml::PropertyValue::Boolean)
                current_item_->use_on_mac = value.bool_value;
        } else if (name == "height" && value.type == sml::PropertyValue::Int) {
            if (element == "Top" || element == "ToolBar")
                model_->dock.top_height = value.int_value;
            else if (element == "Bottom" || element == "StatusBar")
                model_->dock.bottom_height = value.int_value;
        } else if ((element == "Left" || element == "Right") && name == "width" && value.type == sml::PropertyValue::Int) {
            if (element == "Left")
                model_->dock.left_width = value.int_value;
            else
                model_->dock.right_width = value.int_value;
        } else if (name == "label" && value.type == sml::PropertyValue::String) {
            if (element == "Left")
                model_->dock.left_label = value.string_value;
            else if (element == "Right")
                model_->dock.right_label = value.string_value;
            else if (element == "Center")
                model_->dock.center_label = value.string_value;
            else if (element == "Top")
                model_->dock.top_label = value.string_value;
            else if (element == "Bottom")
                model_->dock.bottom_label = value.string_value;
        } else if (element == "ToolButton" && name == "icon") {
            std::string icon = value.string_value;
            if (value.type == sml::PropertyValue::EnumType)
                icon = value.string_value;
            if (stack_.size() >= 2 && stack_[stack_.size() - 2] == "ToolBar")
                model_->dock.toolbar_tools.push_back(icon);
            else if (std::find(stack_.begin(), stack_.end(), "Left") != stack_.end())
                model_->dock.left_tools.push_back(icon);
        }
    }

    void endElement(const std::string& name) override {
        (void)name;
        if (name == "MenuItem")
            current_item_ = nullptr;
        else if (name == "Menu")
            current_menu_ = nullptr;
        else if (name == "MainMenu")
            in_main_menu_ = false;
        if (!stack_.empty())
            stack_.pop_back();
    }

private:
    UiWindow* model_;
    std::vector<std::string> stack_;
    UiMenu* current_menu_ = nullptr;
    UiMenuItem* current_item_ = nullptr;
    bool in_main_menu_ = false;
};

bool UiDocument::parseFromString(const std::string& text, std::string* error_message) {
    UiSmlHandler handler(&window_);
    try {
        sml::SmlSaxParser parser(text);
        parser.registerEnumValue("icon", "play");
        parser.registerEnumValue("icon", "select");
        parser.registerEnumValue("icon", "move");
        parser.registerEnumValue("icon", "paint");
        parser.registerEnumValue("persist", "user");
        parser.registerEnumValue("persist", "project");
        parser.registerEnumValue("persist", "session");
        parser.parse(handler);
    } catch (const sml::SmlParseException& e) {
        if (error_message)
            *error_message = e.what();
        return false;
    }
    return true;
}

void UiDocument::render(const ImGuiViewport* viewport, ImFont* font_15, bool* out_play_clicked) const {
    if (!viewport)
        return;
    if (out_play_clicked)
        *out_play_clicked = false;
    if (window_.main_menu.enabled && !window_.main_menu.menus.empty()) {
        auto render_menu = [&](bool is_mac) {
            if (!ImGui::BeginMainMenuBar())
                return;
            for (size_t i = 0; i < window_.main_menu.menus.size(); ++i) {
                const UiMenu& menu = window_.main_menu.menus[i];
                const char* menu_label = menu.label.empty() ? "Menu" : menu.label.c_str();
                if (ImGui::BeginMenu(menu_label)) {
                    for (size_t j = 0; j < menu.items.size(); ++j) {
                        const UiMenuItem& item = menu.items[j];
                        if (item.is_separator) {
                            ImGui::Separator();
                            continue;
                        }
                        if (is_mac && (item.clicked == "exit" || item.label == "Exit"))
                            continue;
                        const char* item_label = item.label.empty() ? "Item" : item.label.c_str();
                        ImGui::MenuItem(item_label);
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        };
#if defined(__APPLE__)
        // TODO: macOS should integrate into the native MacMenu (GLFW Cocoa).
        // Keep ImGui menu disabled on macOS to avoid double menu bars.
#else
#if defined(_WIN32)
        // TODO: Windows branch should integrate a native menu bar; ImGui is used for now.
#endif
        render_menu(false);
#endif
    }
    if (!window_.dock.enabled) {
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);
        ImGuiWindowFlags root_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                      ImGuiWindowFlags_NoBackground;
        ImGui::SetNextWindowBgAlpha(0.0f);
        const char* title = window_.title.empty() ? "Window" : window_.title.c_str();
        ImGui::Begin(title, nullptr, root_flags);
        ImGui::SetCursorScreenPos(ImVec2(viewport->WorkPos.x + (float)window_.label.position.x,
                                         viewport->WorkPos.y + (float)window_.label.position.y));
        if (window_.label.font_size == 15 && font_15)
            ImGui::PushFont(font_15);
        const char* label_text = window_.label.text.empty() ? "Label" : window_.label.text.c_str();
        ImGui::TextUnformatted(label_text);
        if (window_.label.font_size == 15 && font_15)
            ImGui::PopFont();
        ImGui::End();
        return;
    }

    ImVec2 origin = viewport->WorkPos;
    ImVec2 size = viewport->WorkSize;
    float top_h = (float)window_.dock.top_height;
    float bottom_h = (float)window_.dock.bottom_height;
    float left_w = (float)window_.dock.left_width;
    float right_w = (float)window_.dock.right_width;

    auto begin_panel = [&](const char* name, ImVec2 pos, ImVec2 sz, ImU32 bg_color, float bg_alpha, bool show_titlebar) {
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(sz, ImGuiCond_Always);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if (!show_titlebar)
            flags |= ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowBgAlpha(bg_alpha);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, bg_color);
        ImGui::Begin(name, nullptr, flags);
        ImGui::PopStyleColor();
    };

    if (top_h > 0.0f) {
        const char* top_title = window_.dock.top_label.empty() ? "##Toolbar" : window_.dock.top_label.c_str();
        ImVec4 tb = theme_.toolbar_bg;
        begin_panel(top_title, origin, ImVec2(size.x, top_h), IM_COL32((int)(tb.x * 255.0f), (int)(tb.y * 255.0f), (int)(tb.z * 255.0f), (int)(tb.w * 255.0f)), 1.0f, false);
        if (window_.dock.show_toolbar) {
            ImGui::SameLine();
            for (size_t i = 0; i < window_.dock.toolbar_tools.size(); ++i) {
                const std::string& icon = window_.dock.toolbar_tools[i];
                bool is_play = (icon == "play");
                if (i > 0)
                    ImGui::SameLine();
                std::string label = std::string("##tool_") + std::to_string(i);
                bool clicked = DrawToolButton(label.c_str(), is_play);
                if (clicked && is_play && out_play_clicked)
                    *out_play_clicked = true;
            }
        }
        ImGui::End();
    }

    if (bottom_h > 0.0f) {
        const char* bottom_title = window_.dock.bottom_label.empty() ? "##Status" : window_.dock.bottom_label.c_str();
        ImVec4 sb = theme_.status_bg;
        begin_panel(bottom_title, ImVec2(origin.x, origin.y + size.y - bottom_h), ImVec2(size.x, bottom_h), IM_COL32((int)(sb.x * 255.0f), (int)(sb.y * 255.0f), (int)(sb.z * 255.0f), (int)(sb.w * 255.0f)), 1.0f, false);
        ImVec4 st = theme_.status_text;
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32((int)(st.x * 255.0f), (int)(st.y * 255.0f), (int)(st.z * 255.0f), (int)(st.w * 255.0f)));
        if (window_.dock.show_statusbar) {
            ImGui::TextUnformatted("dungeon.sml loaded");
        }
        ImGui::PopStyleColor();
        ImGui::End();
    }

    if (left_w > 0.0f) {
        const char* left_title = window_.dock.left_label.empty() ? "Toolbar" : window_.dock.left_label.c_str();
        ImVec4 lb = theme_.left_bg;
        begin_panel(left_title, ImVec2(origin.x, origin.y + top_h), ImVec2(left_w, size.y - top_h - bottom_h), IM_COL32((int)(lb.x * 255.0f), (int)(lb.y * 255.0f), (int)(lb.z * 255.0f), (int)(lb.w * 255.0f)), 1.0f, true);
        for (size_t i = 0; i < window_.dock.left_tools.size(); ++i) {
            const std::string& icon = window_.dock.left_tools[i];
            std::string label = IconToLabel(icon);
            DrawToolButton(label.c_str(), false);
        }
        ImGui::End();
    }

    if (right_w > 0.0f) {
        const char* right_title = window_.dock.right_label.empty() ? "Properties" : window_.dock.right_label.c_str();
        ImVec4 rb = theme_.right_bg;
        begin_panel(right_title, ImVec2(origin.x + size.x - right_w, origin.y + top_h), ImVec2(right_w, size.y - top_h - bottom_h), IM_COL32((int)(rb.x * 255.0f), (int)(rb.y * 255.0f), (int)(rb.z * 255.0f), (int)(rb.w * 255.0f)), 1.0f, true);
        if (window_.dock.show_property_panel) {
            ImGui::TextUnformatted("Properties");
            if (property_panel_callback_) {
                property_panel_callback_(ImGui::GetWindowPos(), ImGui::GetWindowSize(), property_panel_user_data_);
            }
        }
        ImGui::End();
    }

    const char* center_title = window_.dock.center_label.empty() ? "Viewport" : window_.dock.center_label.c_str();
    ImVec4 cb = theme_.center_bg;
    begin_panel(center_title,
                ImVec2(origin.x + left_w, origin.y + top_h),
                ImVec2(size.x - left_w - right_w, size.y - top_h - bottom_h),
                IM_COL32((int)(cb.x * 255.0f), (int)(cb.y * 255.0f), (int)(cb.z * 255.0f), (int)(cb.w * 255.0f)), cb.w, true);
    if (window_.dock.show_viewport)
        ImGui::TextUnformatted("Viewport");
    ImGui::End();
}

} // namespace smlui

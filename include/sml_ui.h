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

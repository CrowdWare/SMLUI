#ifndef SML_UI_H
#define SML_UI_H

#include <string>

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
};

class UiDocument {
public:
    bool parseFromString(const std::string& text, std::string* error_message);
    const UiWindow& window() const { return window_; }
    void render(const ImGuiViewport* viewport, ImFont* font_15) const;

private:
    UiWindow window_;
};

} // namespace smlui

#endif

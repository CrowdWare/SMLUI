#include "sml_ui.h"

#include <vector>

namespace smlui {

class UiSmlHandler : public sml::SmlHandler {
public:
    explicit UiSmlHandler(UiWindow* model) : model_(model) {}

    void startElement(const std::string& name) override {
        stack_.push_back(name);
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
        } else if (element == "Label") {
            if (name == "position" && value.type == sml::PropertyValue::Vec2iType)
                model_->label.position = value.vec2i_value;
            else if (name == "text" && value.type == sml::PropertyValue::String)
                model_->label.text = value.string_value;
            else if (name == "fontSize" && value.type == sml::PropertyValue::Int)
                model_->label.font_size = value.int_value;
        }
    }

    void endElement(const std::string& name) override {
        (void)name;
        if (!stack_.empty())
            stack_.pop_back();
    }

private:
    UiWindow* model_;
    std::vector<std::string> stack_;
};

bool UiDocument::parseFromString(const std::string& text, std::string* error_message) {
    UiSmlHandler handler(&window_);
    try {
        sml::SmlSaxParser parser(text);
        parser.parse(handler);
    } catch (const sml::SmlParseException& e) {
        if (error_message)
            *error_message = e.what();
        return false;
    }
    return true;
}

void UiDocument::render(const ImGuiViewport* viewport, ImFont* font_15) const {
    if (!viewport)
        return;
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
}

} // namespace smlui

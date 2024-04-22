#pragma once

#include "../imgui.h"
#include "../imgui_internal.h"

namespace ImAdd
{
    // Configs
    inline float fAnimationSpeed = 0.15f;

    // Helpers
    ImVec4  HexToColorVec4(unsigned int hex_color, float alpha);
    float   Lerp(float a, float b, float t);

    // Buttons
    bool    TextButton(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
    bool    Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
    bool    AcentButton(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
    bool    RadioButton(const char* label, int* v, int current_id, const ImVec2& size_arg = ImVec2(0, 0));
    bool    RadioButtonIcon(const char* std_id, const char* icon, int* v, int current_id, const ImVec2& size_arg = ImVec2(0, 0));
    bool    Selectable(const char* label, bool selected = false, const ImVec2& size_arg = ImVec2(0, 0));

    // Toggles
    bool    Checkbox(const char* label, bool* v);
    bool    SmallCheckbox(const char* label, bool* v);
    bool    Togglebutton(const char* label, bool* v);

    // Color Picker
    //bool    ColorEdit3(const char* label, float col[3]);
    bool    ColorEdit4(const char* label, float col[4]);

    // Inputs
    bool    InputText(const char* label, const char* text, char* buf, size_t buf_size, float width = NULL, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

    // Combo
    bool    BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    bool    Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool    Combo(const char* label, int* current_item, const char* items_separated_by_zeros, float width = NULL, int popup_max_height_in_items = -1);
    bool    Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

    // Window Stuff
    bool    OptionBar(const char* str_id, const char* checkbox_label, bool* v, const char* description, const char* button_icon, ImFont* button_icon_font, float width = 0);
    bool    TitleBar(const char* text, float width = 0);
    bool    CloseButton(const char* str_id, const ImVec2& size_arg = ImVec2(0, 0));

    // Sliders
    bool    SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, float width = NULL, const char* format = NULL);
    bool    SliderFloat(const char* label, float* v, float v_min, float v_max, float width = NULL, const char* format = "%.1f");
    bool    SliderInt(const char* label, int* v, int v_min, int v_max, float width = NULL, const char* format = "%d");

    // Renders
    void    RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f, ImDrawFlags flags = 0);
    void    RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f, ImDrawFlags flags = 0);
}

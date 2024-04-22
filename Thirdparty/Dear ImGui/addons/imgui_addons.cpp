#include "imgui_addons.h"
#include <map>
#include <string>

using namespace ImGui;

struct DefaultStyles_State {
    ImVec4 Text;
    ImVec4 TextDisabled;
    ImVec4 WindowBg;
    ImVec4 ChildBg;
    ImVec4 PopupBg;
    ImVec4 Border;
    ImVec4 BorderShadow;
    ImVec4 FrameBg;
    ImVec4 FrameBgHovered;
    ImVec4 FrameBgActive;
    ImVec4 TitleBg;
    ImVec4 TitleBgActive;
    ImVec4 TitleBgCollapsed;
    ImVec4 MenuBarBg;
    ImVec4 ScrollbarBg;
    ImVec4 ScrollbarGrab;
    ImVec4 ScrollbarGrabHovered;
    ImVec4 ScrollbarGrabActive;
    ImVec4 CheckMark;
    ImVec4 SliderGrab;
    ImVec4 SliderGrabActive;
    ImVec4 Button;
    ImVec4 ButtonHovered;
    ImVec4 ButtonActive;
    ImVec4 Header;
    ImVec4 HeaderHovered;
    ImVec4 HeaderActive;
    ImVec4 Separator;
    ImVec4 SeparatorHovered;
    ImVec4 SeparatorActive;
    ImVec4 ResizeGrip;
    ImVec4 ResizeGripHovered;
    ImVec4 ResizeGripActive;
    ImVec4 Tab;
    ImVec4 TabHovered;
    ImVec4 TabActive;
    ImVec4 TabUnfocused;
    ImVec4 TabUnfocusedActive;
    ImVec4 PlotLines;
    ImVec4 PlotLinesHovered;
    ImVec4 PlotHistogram;
    ImVec4 PlotHistogramHovered;
    ImVec4 TableHeaderBg;
    ImVec4 TableBorderStrong;
    ImVec4 TableBorderLight;
    ImVec4 TableRowBg;
    ImVec4 TableRowBgAlt;
    ImVec4 TextSelectedBg;
    ImVec4 DragDropTarget;
    ImVec4 NavHighlight;
    ImVec4 NavWindowingHighlight;
    ImVec4 NavWindowingDimBg;
    ImVec4 ModalWindowDimBg;
    ImVec4 WindowShadow;
};

ImVec4 ImAdd::HexToColorVec4(unsigned int hex_color, float alpha) {
    ImVec4 color;

    color.x = ((hex_color >> 16) & 0xFF) / 255.0f;
    color.y = ((hex_color >> 8) & 0xFF) / 255.0f;
    color.z = (hex_color & 0xFF) / 255.0f;
    color.w = alpha;

    return color;
}

float ImAdd::Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

bool ImAdd::TextButton(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.Text = ImLerp(it_anim->second.Text, GetStyleColorVec4((held && hovered) ? ImGuiCol_SliderGrab : hovered ? ImGuiCol_Text : ImGuiCol_TextDisabled), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 textCol = GetColorU32(it_anim->second.Text);

    // Render
    RenderNavHighlight(bb, id);

    //window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2), textCol, label);

    PushStyleColor(ImGuiCol_Text, textCol);
    RenderText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2), label);
    PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::Button(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, GetStyleColorVec4((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);

    // Render
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, frameCol, true, style.FrameRounding);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::AcentButton(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, GetStyleColorVec4((held && hovered) ? ImGuiCol_SliderGrab : ImGuiCol_SliderGrabActive), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);
    
    it_anim->second.Border = ImLerp(it_anim->second.Border, GetStyleColorVec4(hovered ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);

    // Render
    RenderNavHighlight(bb, id);
    //RenderFrame(bb.Min, bb.Max, frameCol, true, style.FrameRounding);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, frameCol, style.FrameRounding);
    if (style.FrameBorderSize)
        window->DrawList->AddRect(bb.Min, bb.Max, borderCol, style.FrameRounding, 0, style.FrameBorderSize);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::RadioButton(const char* label, int* v, int current_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    bool active = *v == current_id;

    float border_size = style.FrameBorderSize;

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    ImVec4 frameColDefault = GetStyleColorVec4(ImGuiCol_SliderGrabActive);
    ImVec4 frameColNull = frameColDefault; frameColNull.w = 0;
    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, active ? frameColDefault : frameColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);

    ImVec4 borderColDefault = GetStyleColorVec4(ImGuiCol_SliderGrab);
    ImVec4 borderColNull = borderColDefault; borderColNull.w = 0;
    it_anim->second.Border = ImLerp(it_anim->second.Border, active ? borderColDefault : borderColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);

    it_anim->second.Text = ImLerp(it_anim->second.Text, GetStyleColorVec4(active ? ImGuiCol_Text : (held || hovered) ? ImGuiCol_SliderGrab : ImGuiCol_TextDisabled), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 textCol = GetColorU32(it_anim->second.Text);

    // Render
    RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilled(pos, pos + size, frameCol, style.FrameRounding);
    if (border_size > 0)
        window->DrawList->AddRect(pos, pos + size, borderCol, style.FrameRounding, 0, border_size);

    window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1), textCol, label);

    if (pressed)
        *v = current_id;

    return pressed;
}

bool ImAdd::RadioButtonIcon(const char* std_id, const char* icon, int* v, int current_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(std_id);
    const ImVec2 icon_size = CalcTextSize(icon, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, icon_size.x + style.FramePadding.x * 2.0f, icon_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    bool active = *v == current_id;

    float border_size = style.FrameBorderSize;

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    ImVec4 frameColDefault = GetStyleColorVec4(ImGuiCol_FrameBg);
    ImVec4 frameColNull = frameColDefault; frameColNull.w = 0;
    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, active ? frameColDefault : frameColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);
    
    ImVec4 borderColDefault = GetStyleColorVec4(ImGuiCol_Border);
    ImVec4 borderColNull = borderColDefault; borderColNull.w = 0;
    it_anim->second.Border = ImLerp(it_anim->second.Border, active ? borderColDefault : borderColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);
    
    it_anim->second.Text = ImLerp(it_anim->second.Text, GetStyleColorVec4(active ? ImGuiCol_SliderGrab : (held || hovered) ? ImGuiCol_Text : ImGuiCol_TextDisabled), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 textCol = GetColorU32(it_anim->second.Text);

    // Render
    RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilled(pos, pos + size, frameCol, size.y / 2);
    if (border_size > 0)
        window->DrawList->AddRect(pos, pos + size, borderCol, size.y / 2, 0, border_size);

    window->DrawList->AddText(pos + ImVec2(size.x / 2 - icon_size.x / 2, size.y / 2 - icon_size.y / 2), textCol, icon);

    if (pressed)
        *v = current_id;

    return pressed;
}

bool ImAdd::Selectable(const char* label, bool selected, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    float borderSize = style.FrameBorderSize;

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }


    ImVec4 frameColDefault = GetStyleColorVec4(ImGuiCol_Header);
    ImVec4 frameHoveredCol = GetStyleColorVec4(ImGuiCol_HeaderHovered);
    ImVec4 frameColNull = frameColDefault; frameColNull.w = 0;
    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, selected ? frameColDefault : hovered ? frameHoveredCol : frameColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);

    ImVec4 borderColDefault = GetStyleColorVec4(ImGuiCol_Border);
    ImVec4 borderColNull = borderColDefault; borderColNull.w = 0;
    it_anim->second.Border = ImLerp(it_anim->second.Border, selected ? borderColDefault : borderColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);

    // Render
    RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, frameCol, style.FrameRounding);

    if (borderSize > 0)
        window->DrawList->AddRect(bb.Min, bb.Max, borderCol, style.FrameRounding, 0, borderSize);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }
    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    ImVec4 frameColDefault = GetStyleColorVec4(ImGuiCol_SliderGrab);
    ImVec4 frameColNull = frameColDefault; frameColNull.w = 0;
    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, *v ? frameColDefault : frameColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);
    
    ImVec4 frameActiveColDefault = GetStyleColorVec4(ImGuiCol_FrameBg);
    ImVec4 frameActiveColNull = frameActiveColDefault; frameActiveColNull.w = 0;
    it_anim->second.FrameBgActive = ImLerp(it_anim->second.FrameBgActive, !*v ? frameActiveColDefault : frameActiveColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameActiveCol = GetColorU32(it_anim->second.FrameBgActive);

    ImVec4 borderColDefault = GetStyleColorVec4(ImGuiCol_Border);
    ImVec4 borderColNull = borderColDefault; borderColNull.w = 0;
    it_anim->second.Border = ImLerp(it_anim->second.Border, !*v ? borderColDefault : borderColNull, 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);

    // Render
    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    RenderNavHighlight(total_bb, id);

    RenderFrame(check_bb.Min, check_bb.Max, frameCol, false, style.FrameRounding);

    ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
    bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
    if (mixed_value)
    {
        // Undocumented tristate/mixed/indeterminate checkbox (#2644)
        // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
        ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    }
    //else if (*v)
    {
        const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 4.0f));
        RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, frameActiveCol, style.FrameRounding);
    if (style.FrameBorderSize > 0)
        window->DrawList->AddRect(check_bb.Min, check_bb.Max, borderCol, style.FrameRounding, 0, style.FrameBorderSize);

    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);

    return pressed;
}

bool ImAdd::SmallCheckbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    bool result = Checkbox(label, v);
    PopStyleVar();

    return result;
}

bool ImAdd::Togglebutton(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    float height = ImGui::GetFontSize();
    const ImVec2 pos = window->DC.CursorPos;

    float width = height * 2.f;
    float radius = height * 0.50f;
    float edge_rounding = height * 0.5f;

    const ImRect frame_bb(pos, ImVec2(pos.x + width, pos.y + height));
    const ImRect total_bb(pos, ImVec2(pos.x + width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y));

    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id))
        return false;

    float last_active_id_timer = g.LastActiveIdTimer;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
        g.LastActiveIdTimer = 0.f;
    }

    if (g.LastActiveIdTimer == 0.f && g.LastActiveId == id && !pressed)
        g.LastActiveIdTimer = last_active_id_timer;

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, GetStyleColorVec4(*v ? (held && hovered) ? ImGuiCol_SliderGrab : ImGuiCol_SliderGrabActive : (held && hovered) ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);

    it_anim->second.Border = ImLerp(it_anim->second.Border, GetStyleColorVec4(*v ? ImGuiCol_SliderGrab : ImGuiCol_Border), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 borderCol = GetColorU32(it_anim->second.Border);
    
    it_anim->second.CheckMark = ImLerp(it_anim->second.CheckMark, GetStyleColorVec4(*v ? ImGuiCol_Text : ImGuiCol_TextDisabled), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 checkMarkCol = GetColorU32(it_anim->second.CheckMark);

    // Anime slide
    float t = *v ? 1.0f : 0.0f;

    if (g.LastActiveId == id)
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / fAnimationSpeed);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, frameCol, edge_rounding);
    if (style.FrameBorderSize > 0.0f)
    {
        window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, borderCol, edge_rounding, 0, style.FrameBorderSize);
    }

    RenderNavHighlight(total_bb, id);

    ImVec2 label_pos = ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y);
    RenderText(label_pos, label);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + radius + t * (width - radius * 2.0f), pos.y + radius), radius - 4, checkMarkCol);

    return pressed;
}

bool ImAdd::ColorEdit4(const char* label, float col[4])
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec4 col_v4(col[0], col[1], col[2], col[3]);

    ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview;

    BeginGroup();

    bool result = ColorButton(label, col_v4, flags, ImVec2(g.FontSize * 2, g.FontSize));
    if (result)
    {
        OpenPopup("picker");
    }
    if (BeginPopup("picker"))
    {
        ColorPicker4(label, col, flags);
        EndPopup();
    }

    SameLine(style.ItemInnerSpacing.x + g.FontSize * 2);
    Text(label);

    EndGroup();

    return result;
}

bool ImAdd::InputText(const char* label, const char* text, char* buf, size_t buf_size, float width, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 text_size  = CalcTextSize(text , NULL, true);

    IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()

    bool result = false;
    bool has_label = label_size.x > 0;

    const float w = CalcItemSize(ImVec2(width, 0), CalcItemWidth(), 0).x;

    BeginGroup();
    {
        if (has_label) {
            PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.ItemInnerSpacing.y));
            ImGui::Text(label);
        }
        {
            ImVec2 pos = GetCursorScreenPos();
            PushItemWidth(w);
            {
                result |= InputTextEx(std::string("##" + std::string(label)).c_str(), NULL, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
            }
            PopItemWidth();

            if (text_size.x > 0)
            {
                if (!ImGui::IsItemActive() && !strlen(buf)) {
                    //ImGui::SameLine(pos.x);
                    ImGui::SetCursorScreenPos(pos + style.FramePadding);
                    ImGui::TextDisabled(text);
                }
            }
        }
        if (has_label) {
            PopStyleVar();
        }
    }
    EndGroup();

    return result;
}

bool ImAdd::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
    if (flags & ImGuiComboFlags_WidthFitPreview)
        IM_ASSERT((flags & (ImGuiComboFlags_NoPreview | ImGuiComboFlags_CustomPreview)) == 0);

    const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    
    const float preview_width = ((flags & ImGuiComboFlags_WidthFitPreview) && (preview_value != NULL)) ? CalcTextSize(preview_value, NULL, true).x : 0.0f;
    const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : ((flags & ImGuiComboFlags_WidthFitPreview) ? (arrow_size + preview_width + style.FramePadding.x * 2.0f) : CalcItemWidth());
    
    const ImRect bb(window->DC.CursorPos + ImVec2(0.0f, label_size.x > 0 ? label_size.y + style.ItemInnerSpacing.y : 0.0f), window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f) + ImVec2(0.0f, label_size.x > 0 ? label_size.y + style.ItemInnerSpacing.y : 0.0f));
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f) + ImVec2(0.0f, label_size.x > 0 ? label_size.y + style.ItemInnerSpacing.y : 0.0f));
    
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, GetStyleColorVec4(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);
    
    it_anim->second.Text = ImLerp(it_anim->second.Text, GetStyleColorVec4((popup_open || hovered) ? ImGuiCol_Text : ImGuiCol_TextDisabled), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 textCol = GetColorU32(it_anim->second.Text);

    // Render shape
    const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
    RenderNavHighlight(bb, id);
    if (!(flags & ImGuiComboFlags_NoPreview))
        window->DrawList->AddRectFilled(bb.Min, bb.Max, frameCol, style.FrameRounding);
    if (!(flags & ImGuiComboFlags_NoArrowButton))
    {
        if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
            RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, bb.Min.y + style.FramePadding.y), textCol, ImGuiDir_Down, 1.0f);
    }
    RenderFrameBorder(bb.Min, bb.Max, style.FrameRounding);

    // Custom preview
    if (flags & ImGuiComboFlags_CustomPreview)
    {
        g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
        IM_ASSERT(preview_value == NULL || preview_value[0] == 0);
        preview_value = NULL;
    }

    // Render preview and label
    if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
    {
        if (g.LogEnabled)
            LogSetNextTextDecoration("{", "}");
        PushStyleColor(ImGuiCol_Text, GetStyleColorVec4(ImGuiCol_TextDisabled));
        RenderTextClipped(bb.Min + style.FramePadding, ImVec2(value_x2, bb.Max.y), preview_value, NULL, NULL);
        PopStyleColor();
    }
    if (label_size.x > 0)
        RenderText(total_bb.Min, label);

    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    return BeginComboPopup(popup_id, bb, flags);
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool ImAdd::Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        preview_value = getter(user_data, *current_item);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!BeginCombo(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        const char* item_text = getter(user_data, i);
        if (item_text == NULL)
            item_text = "*Unknown item*";

        PushID(i);
        const bool item_selected = (i == *current_item);
        if (Selectable(item_text, item_selected, ImVec2(-0.1f, 0)) && *current_item != i)
        {
            value_changed = true;
            *current_item = i;
            //CloseCurrentPopup();
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

// Getter for the old Combo() API: const char*[]
static const char* Items_ArrayGetter(void* data, int idx)
{
    const char* const* items = (const char* const*)data;
    return items[idx];
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static const char* Items_SingleStringGetter(void* data, int idx)
{
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    return *p ? p : NULL;
}

bool ImAdd::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

bool ImAdd::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, float width, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }

    const float w = CalcItemSize(ImVec2(width, 0), CalcItemWidth(), 0).x;
    PushItemWidth(w);
    bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    PopItemWidth();

    return value_changed;
}

bool ImAdd::OptionBar(const char* str_id, const char* checkbox_label, bool* v, const char* description, const char* button_icon, ImFont* button_icon_font, float width)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    bool pressed = false;

    ImGui::BeginChild(str_id, ImVec2(width, ImGui::GetFrameHeight() + ImGui::GetFontSize() + style.WindowPadding.y * 2 + style.ItemSpacing.y), ImGuiChildFlags_Border);
    {
        ImGui::BeginGroup();
        {
            ImAdd::Checkbox(checkbox_label, v);
            ImGui::TextDisabled(description);
        }
        ImGui::EndGroup();
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - ImGui::GetWindowHeight(), 0) + style.WindowPadding);
        ImGui::PushFont(button_icon_font);
        {
            pressed |= TextButton(button_icon, ImVec2(-0.1f, -0.1f));
        }
        ImGui::PopFont();
    }
    ImGui::EndChild();

    return pressed;
}

bool ImAdd::TitleBar(const char* text, float width)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    bool pressed = false;

    ImGui::BeginChild(text, ImVec2(width, ImGui::GetFrameHeight() + style.FramePadding.y * 2));
    {
        ImGui::SetCursorPos(style.FramePadding * 2);
        ImGui::Text(text);
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - ImGui::GetWindowHeight(), 0));
        pressed |= CloseButton(text, ImVec2(-0.1f, -0.1f));
    }
    ImGui::EndChild();

    return pressed;
}

bool ImAdd::CloseButton(const char* str_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, g.FontSize + style.FramePadding.x * 2.0f, g.FontSize + style.FramePadding.y * 2.0f);

    // Tweak 1: Shrink hit-testing area if button covers an abnormally large proportion of the visible region. That's in order to facilitate moving the window away. (#3825)
    // This may better be applied as a general hit-rect reduction mechanism for all widgets to ensure the area to move window is always accessible?
    const ImRect bb(pos, pos + size);
    ImRect bb_interact = bb;
    const float area_to_visible_ratio = window->OuterRectClipped.GetArea() / bb.GetArea();
    if (area_to_visible_ratio < 1.5f)
        bb_interact.Expand(ImTrunc(bb_interact.GetSize() * -0.25f));

    // Tweak 2: We intentionally allow interaction when clipped so that a mechanical Alt,Right,Activate sequence can always close a window.
    // (this isn't the common behavior of buttons, but it doesn't affect the user because navigation tends to keep items visible in scrolling layer).
    bool is_clipped = !ItemAdd(bb_interact, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb_interact, id, &hovered, &held);
    if (is_clipped)
        return pressed;

    // Render
    // FIXME: Clarify this mess
    ImU32 col = GetColorU32(held ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered);
    ImVec2 center = bb.GetCenter();
    if (hovered)
        window->DrawList->AddCircleFilled(center, ImMax(2.0f, g.FontSize * 0.5f + 1.0f), col);

    float cross_extent = g.FontSize * 0.5f * 0.7071f - 1.0f;
    ImU32 cross_col = GetColorU32(ImGuiCol_Text);
    center -= ImVec2(0.5f, 0.5f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent), center + ImVec2(-cross_extent, -cross_extent), cross_col, 1.0f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent), center + ImVec2(-cross_extent, +cross_extent), cross_col, 1.0f);

    return pressed;
}

bool ImAdd::SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, float width, const char* format)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    float w = CalcItemSize(ImVec2(width, 0), CalcItemWidth(), 0).x;
    w -= label_size.x > 0 ? 0 : CalcTextSize(value_buf).x + style.ItemInnerSpacing.x;

    ImVec2 pos = window->DC.CursorPos;

    const float label_height = label_size.x > 0 ? g.FontSize + style.ItemInnerSpacing.y : 0.0f;
    const ImRect frame_bb(pos + ImVec2(0, label_height), pos + ImVec2(w, g.FontSize + label_height));
    const ImRect total_bb(pos, pos + ImVec2(w, g.FontSize + label_height));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    const bool clicked = hovered && IsMouseClicked(0, id);
    const bool make_active = (clicked || g.NavActivateId == id);

    if (make_active)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    // Animate Colors
    static std::map<ImGuiID, DefaultStyles_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, DefaultStyles_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.FrameBg = ImLerp(it_anim->second.FrameBg, GetStyleColorVec4(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 frameCol = GetColorU32(it_anim->second.FrameBg);

    it_anim->second.SliderGrab = ImLerp(it_anim->second.SliderGrab, GetStyleColorVec4(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), 1.0f / fAnimationSpeed * ImGui::GetIO().DeltaTime);
    ImU32 sliderGrabCol = GetColorU32(it_anim->second.SliderGrab);

    // Draw frame
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frameCol, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, 0, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    ImVec2 grab_pad = ImVec2(grab_bb.Min.y - frame_bb.Min.y, grab_bb.Min.y - frame_bb.Min.y);

    grab_bb.Min = frame_bb.Min + grab_pad;

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, sliderGrabCol, style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    float extra_width;
    if (label_size.x > 0) {
        extra_width = -ImGui::CalcTextSize(value_buf).x;
    }
    else {
        extra_width = style.ItemInnerSpacing.x;
    }

    window->DrawList->AddText(pos + ImVec2(w + extra_width, 0), GetColorU32(ImGuiCol_TextDisabled), value_buf);

    if (label_size.x > 0.0f)
        window->DrawList->AddText(pos, GetColorU32(ImGuiCol_Text), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}

bool ImAdd::SliderFloat(const char* label, float* v, float v_min, float v_max, float width, const char* format)
{
    return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, width, format);
}

bool ImAdd::SliderInt(const char* label, int* v, int v_min, int v_max, float width, const char* format)
{
    return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, width, format);
}

void ImAdd::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding, ImDrawFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding, flags);
    const float border_size = g.Style.FrameBorderSize;
    if (border && border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, flags, border_size);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, flags, border_size);
    }
}

void ImAdd::RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding, ImDrawFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const float border_size = g.Style.FrameBorderSize;
    if (border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, flags, border_size);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, flags, border_size);
    }
}
#include "StdAfx.h"
#include "Gui.h"
#include "clicker/clicker.hpp"

#include "Resources/Fonts/IconsFontAwesome6.h"
#include "Resources/Fonts/IconsFontAwesome6Brands.h"
#include "Resources/Fonts/Poppins_Medium.h"

std::string clicker::formatCPS(int cps, int mincps, int maxcps) {
    return std::to_string(cps) + " (" + std::to_string(cps - mincps) + "-" + std::to_string(cps + maxcps) + ")";
}

void Gui::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL; // Remove the imgui.ini
    GImGui->NavDisableHighlight = true;

    ImGui::GetDefaultFont();
    Customize();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, device_context);
}

void Gui::Customize(void)
{
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    style.WindowRounding = 4;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 3;
    style.GrabRounding = 3;
    style.TabRounding = 3;
    style.ScrollbarRounding = 1;

    style.ButtonTextAlign = { 0.5f, 0.5f };
    style.WindowTitleAlign = { 0.5f, 0.5f };
    style.FramePadding = { 6.0f, 6.0f };
    style.ItemSpacing = { 9.0f, 9.0f };
    style.WindowPadding = { 9.0f, 9.0f };
    style.ItemInnerSpacing = { 8.0f, 4.0f };

    style.WindowBorderSize = 1;
    style.FrameBorderSize = 1;

    style.ScrollbarSize = 12.f;
    style.GrabMinSize = 8.f;
    style.WindowShadowSize = 0.f;

    //style.Colors[ImGuiCol_WindowShadow]         = ImAdd::HexToColorVec4(0x06A76D, 1.0f);

    style.Colors[ImGuiCol_WindowBg] = ImAdd::HexToColorVec4(0x111111, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImAdd::HexToColorVec4(0x151515, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImAdd::HexToColorVec4(0x111111, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImAdd::HexToColorVec4(0x191919, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImAdd::HexToColorVec4(0x191919, 1.0f);

    style.Colors[ImGuiCol_Border] = ImAdd::HexToColorVec4(0x1F1F1F, 1.0f);
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];

    style.Colors[ImGuiCol_CheckMark] = ImAdd::HexToColorVec4(0x151515, 1.0f);
    style.Colors[ImGuiCol_Text] = ImAdd::HexToColorVec4(0xFFFFFF, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImAdd::HexToColorVec4(0x616161, 1.0f);

    style.Colors[ImGuiCol_SliderGrab] = ImAdd::HexToColorVec4(0x06A76D, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImAdd::HexToColorVec4(0x06A76D, 0.7f);

    style.Colors[ImGuiCol_Button] = ImAdd::HexToColorVec4(0x191919, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImAdd::HexToColorVec4(0x191919, 0.7f);
    style.Colors[ImGuiCol_ButtonActive] = ImAdd::HexToColorVec4(0x191919, 0.5f);

    style.Colors[ImGuiCol_FrameBg] = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_ButtonHovered];
    style.Colors[ImGuiCol_FrameBgActive] = style.Colors[ImGuiCol_ButtonActive];

    style.Colors[ImGuiCol_Header] = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_ButtonHovered];
    style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_ButtonActive];

    ImFontConfig cfg;
    cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    io.Fonts->AddFontFromMemoryCompressedTTF(Poppins_Medium_compressed_data, Poppins_Medium_compressed_size, 16, &cfg, io.Fonts->GetGlyphRangesDefault());

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };

    ImFontConfig fa_config; fa_config.MergeMode = true; fa_config.PixelSnapH = true;
    fa_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;

    ImFont* fontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14, &fa_config, icons_ranges);
    ImFont* fontAwesomeBrands = io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14, &fa_config, icons_ranges_brands);

    ImFontConfig icons_config; icons_config.PixelSnapH = true;
    icons_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    IconFont = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 18, &icons_config, icons_ranges);

    LogoFont = io.Fonts->AddFontFromMemoryCompressedTTF(Poppins_Medium_compressed_data, Poppins_Medium_compressed_size, IconFont->FontSize + style.WindowPadding.y * 2 + style.FramePadding.y * 2, &cfg, io.Fonts->GetGlyphRangesDefault());

    ImNotify.Initialize(IconFont);
}

void Gui::Render(void)
{
    // Defs
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    float HeaderHeight = IconFont->FontSize + style.WindowPadding.y * 4 + style.FramePadding.y * 2;
    //float MenuBarHeight = ImGui::GetFrameHeight();
    float MenuBarHeight = ImGui::GetFontSize() * 2 + style.WindowPadding.y * 2;
    //float FooterHeight  = ImGui::GetFrameHeight();
    float FooterHeight = ImGui::GetFontSize() + style.WindowPadding.y * 2;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        ImNotify.Render();

        ImGui::SetNextWindowPos(io.DisplaySize / 2 - Size / 2, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ Size });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin(GUI_TITLE, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
        ImGui::PopStyleVar(2);
        {
            ImVec2 pos = ImGui::GetWindowPos();
            ImVec2 size = ImGui::GetWindowSize();
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            float windowRounding = style.WindowRounding;
            float borderSize = style.WindowBorderSize;

            ImU32 colBorder = ImGui::GetColorU32(ImGuiCol_Border);

            // Header
            ImAdd::RenderFrame(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_WindowBg), false, windowRounding, ImDrawFlags_RoundCornersTop);
            drawList->AddText(LogoFont, LogoFont->FontSize, pos + style.WindowPadding * 2, ImGui::GetColorU32(ImGuiCol_Text), "Motion");
            drawList->AddText(LogoFont, LogoFont->FontSize, pos + style.WindowPadding * 2 + ImVec2(ImGui::CalcTextSize("Motion").x * (LogoFont->FontSize / ImGui::GetFontSize()), 0), ImGui::GetColorU32(ImGuiCol_SliderGrab), "");

            // MenuBar
            ImAdd::RenderFrame(pos + ImVec2(0, HeaderHeight), pos + ImVec2(size.x, HeaderHeight + MenuBarHeight), ImGui::GetColorU32(ImGuiCol_ChildBg), false, windowRounding, ImDrawFlags_RoundCornersNone);
            if (borderSize > 0)
            {
                drawList->AddLine(pos + ImVec2(borderSize, HeaderHeight), pos + ImVec2(size.x - borderSize, HeaderHeight), colBorder, borderSize);
                drawList->AddLine(pos + ImVec2(borderSize, HeaderHeight + MenuBarHeight - borderSize), pos + ImVec2(size.x - borderSize, HeaderHeight + MenuBarHeight - borderSize), colBorder, borderSize);
            }

            // Main
            ImAdd::RenderFrame(pos + ImVec2(0, HeaderHeight + MenuBarHeight), pos + ImVec2(size.x, size.y - FooterHeight), ImGui::GetColorU32(ImGuiCol_WindowBg), false, windowRounding, ImDrawFlags_RoundCornersNone);

            // Footer
            ImAdd::RenderFrame(pos + ImVec2(0, size.y - FooterHeight), pos + size, ImGui::GetColorU32(ImGuiCol_ChildBg), false, windowRounding, ImDrawFlags_RoundCornersBottom);
            if (borderSize > 0)
            {
                //drawList->AddLine(pos + ImVec2(borderSize, size.y - FooterHeight - borderSize), pos + ImVec2(size.x - borderSize, size.y - FooterHeight - borderSize), colBorder, borderSize);
            }
            //drawList->AddText(pos + ImVec2(style.FramePadding.x, size.y - FooterHeight + style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_TextDisabled), "By: charlotte");
            drawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("v1.0").x - style.WindowPadding.x, size.y - FooterHeight + style.WindowPadding.y), ImGui::GetColorU32(ImGuiCol_SliderGrab), "v1.0");

            // Window Border
            if (borderSize > 0)
            {
                ImAdd::RenderFrameBorder(pos, pos + size, windowRounding);
            }
        }
        // Clips
        {
            ImGui::SameLine(style.WindowPadding.x * 4 + ImGui::CalcTextSize("Motion Clicker").x * (LogoFont->FontSize / ImGui::GetFontSize()));
            ImGui::BeginChild("Header", ImVec2(0, HeaderHeight), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
            {
                ImVec2 radioSize = ImVec2(HeaderHeight - style.WindowPadding.x * 2, HeaderHeight - style.WindowPadding.y * 2);

                ImGui::SameLine(ImGui::GetWindowWidth() - radioSize.x * 1.5 - style.ItemSpacing.x * 4 - style.WindowPadding.x);
                ImGui::BeginGroup();
                {
                    ImGui::PushFont(IconFont);
                    {
                        //ImAdd::RadioButtonIcon("player", ICON_FA_USER, &PageID, 2, radioSize);
                        //ImGui::SameLine();
                        //ImAdd::RadioButtonIcon("settings", ICON_FA_CROSSHAIRS, &PageID, 5, radioSize);
                        //ImGui::SameLine();
                        //ImAdd::RadioButtonIcon("visuals", ICON_FA_EYE, &PageID, 3, radioSize);
                        //ImGui::SameLine();
                        //ImAdd::RadioButtonIcon("vehicles", ICON_FA_CAR, &PageID, 4, radioSize);
                        //ImGui::SameLine();
                        //ImAdd::RadioButtonIcon("aim", ICON_FA_GEAR, &PageID, 1, radioSize);
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();
            }
            ImGui::EndChild();
            ImGui::SetCursorPosY(HeaderHeight);
            ImGui::BeginChild("MenuBar", ImVec2(0, MenuBarHeight), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
            {
                std::vector radioList = { "Auto Clicker" };
                ImGui::BeginGroup();
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                    {
                        ImGui::Text("    Auto Clicker");
                        ImGui::TextDisabled("    Automatically clicks for you, obviously..");
                    }
                    ImGui::PopStyleVar();
                }
                ImGui::EndGroup();

                if (!radioList.empty())
                {
                    float totalWidth = 0.0f;

                    for (size_t i = 0; i < radioList.size(); i++) {
                        totalWidth += ImGui::CalcTextSize(radioList[i]).x + style.FramePadding.x;
                    }

                    totalWidth += style.ItemSpacing.x * radioList.size();

                    ImGui::SameLine(ImGui::GetWindowWidth() - totalWidth - style.WindowPadding.x * 2);

                    // Radio Buttons Group
                    ImGui::BeginGroup();
                    {
                        for (size_t i = 0; i < radioList.size(); i++)
                        {
                            ImAdd::RadioButton(radioList[i], &MenuID, i, ImVec2(0, -0.1f));

                            ImGui::SameLine();
                        }
                        ImGui::NewLine();
                    }
                    ImGui::EndGroup();
                }
            }
            ImGui::EndChild();
            ImGui::SetCursorPosY(HeaderHeight + MenuBarHeight);

            ImGui::BeginChild("Main", ImVec2(0, ImGui::GetWindowHeight() - HeaderHeight - MenuBarHeight - FooterHeight), 0, ImGuiWindowFlags_NoBackground);
            {
                // add Sidebar
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
                ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                ImGui::PopStyleVar();
                {
                    float GroupWidth = (ImGui::GetWindowWidth() - style.ItemSpacing.x - style.WindowPadding.x * 2) / 2;
                    float GroupHeight = (ImGui::GetWindowHeight() - style.ItemSpacing.y - style.WindowPadding.y * 2) / 2;

                    if (PageID == 1 && MenuID == 0)
                    {
                        ImGui::BeginGroup();
                        {
                            ImGui::BeginChild("group1", ImVec2(GroupWidth, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);
                            {
                                if (ImGui::BeginMenuBar())
                                {
                                    ImGui::TextDisabled("Conditions");
                                    ImGui::EndMenuBar();
                                }
                                ImAdd::Togglebutton("Toggle", &clicker::toggle);

                                ImAdd::SliderInt("Click Rate", &clicker::cps, 5, 20, -0.1, (clicker::formatCPS(clicker::cps, clicker::mincps, clicker::maxcps) + " CPS").c_str());
                                ImAdd::SmallCheckbox("Inventory", &clicker::inventory);
                                ImAdd::SmallCheckbox("Prioritize Higher CPS", &clicker::prioritize_higher_cps);
                                ImAdd::SmallCheckbox("RMB Lock", &clicker::rmb_lock);
                            
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndGroup();
                        ImGui::SameLine();
                        ImGui::BeginChild("group2", ImVec2(GroupWidth, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);
                        {
                            if (ImGui::BeginMenuBar())
                            {
                                ImGui::TextDisabled("Randomization");
                                ImGui::EndMenuBar();
                            }
                            ImAdd::SliderInt("Min Click Range", &clicker::mincps, 1, 5, -0.1, "-%d CPS");
                            ImAdd::SliderInt("Max Click Range", &clicker::maxcps, 1, 5, -0.1, "+%d CPS");
                            ImGui::Text(""); // gives a bigger space than ImGui::Spacing();
                            ImAdd::SliderInt("Min Drop Delay", &clicker::mindrop, 25, 300, -0.1, "%d ms");
                            ImAdd::SliderInt("Max Drop Delay", &clicker::maxdrop, 25, 300, -0.1, "%d ms");
                            ImAdd::SliderInt("Drop Loop Grace Period", &clicker::dropgraceperiod, 1, 10, -0.1, "%d p/l");
                            ImAdd::SliderInt("Drop Chance", &clicker::dropchance, 0, 100, -0.1, "%d%%");
                            if (clicker::mindrop >= clicker::maxdrop) {
                                clicker::mindrop = clicker::maxdrop;
                            }
                            ImGui::Text("");
                            ImAdd::SliderInt("Min Double Click Delay", &clicker::mindoubleclickdelay, 5, 20, -0.1, "%d ms");
                            ImAdd::SliderInt("Max Double Click Delay", &clicker::maxdoubleclickdelay, 6, 20, -0.1, "%d ms");
                            ImAdd::SliderInt("Double Click Chance", &clicker::doubleclickchance, 0, 100, -0.1, "%d%%");
                            if (clicker::mindoubleclickdelay >= clicker::maxdoubleclickdelay) {
                                clicker::mindoubleclickdelay = clicker::maxdoubleclickdelay - 1;
                            }
                        }
                        ImGui::EndChild();
                    }

                    if (PageID == 1 && MenuID == 1)
                    {
                        ImGui::Text("Coming Soon");
                    }
                }
            }
            ImGui::EndChild();
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - FooterHeight);
            ImGui::BeginChild("Footer", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
            {
            //    ImAdd::TextButton(ICON_FA_YOUTUBE, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize() + style.WindowBorderSize * 2));
            //    ImGui::SameLine();
            //    ImAdd::TextButton(ICON_FA_DISCORD, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize() + style.WindowBorderSize * 2));
            //    ImGui::SameLine();
            //    ImAdd::TextButton(ICON_FA_GLOBE, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize() + style.WindowBorderSize * 2));
            }
            ImGui::EndChild();


        }
        ImGui::End();
    }

    ImGui::Render();
}

void Gui::Unload(void)
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

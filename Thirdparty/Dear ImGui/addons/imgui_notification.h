#pragma once

#include "../imgui.h"
#include "../../../Resources/Fonts/IconsFontAwesome6.h"
#include <vector>

using namespace ImGui;

enum ImNotifyLevel_
{
	ImNotifyLevel_None,
	ImNotifyLevel_Success,
	ImNotifyLevel_Warning,
	ImNotifyLevel_Error,
	ImNotifyLevel_Info,
	ImNotifyLevel_COUNT
};

struct NotifyData
{
public:
	ImVec4 color;
	const char* icon;
	const char* title;
	const char* text;
};

struct stImNotify
{
protected:
	const char* GetFlagText(ImNotifyLevel_ flag)
	{
		switch (flag)
		{
		case ImNotifyLevel_None:
			return "None";
			break;
		case ImNotifyLevel_Success:
			return "Success";
			break;
		case ImNotifyLevel_Warning:
			return "Warning";
			break;
		case ImNotifyLevel_Error:
			return "Error";
			break;
		case ImNotifyLevel_Info:
			return "Info";
			break;
		default:
			return "Unknow";
			break;
		}
	}
	
	const char* GetFlagIcon(ImNotifyLevel_ flag)
	{
		switch (flag)
		{
		case ImNotifyLevel_None:
			return " ";
			break;
		case ImNotifyLevel_Success:
			return ICON_FA_CHECK;
			break;
		case ImNotifyLevel_Warning:
			return ICON_FA_TRIANGLE_EXCLAMATION;
			break;
		case ImNotifyLevel_Error:
			return ICON_FA_XMARK;
			break;
		case ImNotifyLevel_Info:
			return ICON_FA_INFO;
			break;
		default:
			return ICON_FA_QUESTION;
			break;
		}
	}

	ImVec4 GetMainColor(ImNotifyLevel_ flag)
	{
		switch (flag)
		{
		case ImNotifyLevel_None:
			return ImVec4(0.702f, 0.702f, 0.702f, 1.000f);
			break;
		case ImNotifyLevel_Success:
			return ImVec4(0.039f, 0.482f, 0.325f, 1.000f);
			break;
		case ImNotifyLevel_Warning:
			return ImVec4(0.749f, 0.498f, 0.09f, 1.000f);
			break;
		case ImNotifyLevel_Error:
			return ImVec4(0.631f, 0.106f, 0.106f, 1.000f);
			break;
		case ImNotifyLevel_Info:
			return ImVec4(0.102f, 0.588f, 0.722f, 1.000f);
			break;
		default:
			return ImVec4(0, 0, 0, 1.000f);
			break;
		}
	}
private:
	std::vector<NotifyData> notifications;

	ImFont* IconFont;

	void PrintEx(ImVec4 color, const char* icon, const char* title, const char* text)
	{
		notifications.push_back({color, icon, title, text});
	}

public:
	void	Initialize(ImFont* icon_font)
	{
		IconFont = icon_font;
		notifications.clear();
	}

	void	RenderNotify(int id, ImVec4 color, const char* icon, const char* title, const char* text)
	{
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		float Height = IconFont->FontSize + style.WindowPadding.y * 4 + style.FramePadding.y * 2;
		float Width  = Height * 5;

		PushStyleColor(ImGuiCol_ChildBg, GetStyleColorVec4(ImGuiCol_WindowBg));
		BeginChild(std::string("Notify[" + std::to_string(id) + "]").c_str(), ImVec2(Width, Height), true, ImGuiWindowFlags_NoDecoration);
		PopStyleColor();
		{
			ImVec2 pos = ImGui::GetWindowPos();
			ImVec2 size = ImGui::GetWindowSize();
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			RenderFrame(pos + style.WindowPadding, pos + ImVec2(Height, Height) - style.WindowPadding, GetColorU32(color), true, Height / 2);

			PushFont(IconFont);
			{
				drawList->AddText(pos + ImVec2(size.y, size.y) / 2 - CalcTextSize(icon) / 2, GetColorU32(ImGuiCol_WindowBg), icon);
			}
			PopFont();

			drawList->AddText(pos + ImVec2(Height, style.WindowPadding.y + style.FramePadding.y), GetColorU32(ImGuiCol_Text), title);
			drawList->AddText(pos + ImVec2(Height, Height - GetFontSize() - style.WindowPadding.y - style.FramePadding.y), GetColorU32(ImGuiCol_TextDisabled), text);
		}
		EndChild();
	}

	void Render()
	{
		if (notifications.empty())
			return;

		Begin("notification", nullptr);
		{
			//for (size_t i = 0; i < notifications.size(); i++)
			for (int i = notifications.size() - 1; i >= 0; i--)
			{
				RenderNotify(i, notifications[i].color, notifications[i].icon, notifications[i].title, notifications[i].text);
			}
		}
		End();
	}

	template <ImNotifyLevel_ level = ImNotifyLevel_None>
	void	Print(const char* text)
	{
		PrintEx(GetMainColor(level), GetFlagIcon(level), GetFlagText(level), text);
	}

	void	PrintCustom(ImVec4 color, const char* icon, const char* title, const char* text)
	{
		PrintEx(color, icon, title, text);
	}
};
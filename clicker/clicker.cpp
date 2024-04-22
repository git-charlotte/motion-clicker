#include <deque>
#include <thread>
#include <iostream>
#include <memory>
#include "StdAfx.h"

#include "clicker.hpp"

#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP   0x0202

auto clicker::menucheck() -> bool {
	struct CursorInformation {
		CURSORINFO info;
		CursorInformation() noexcept : info{ .cbSize = sizeof(CURSORINFO) } {}
	} cursor_info;

	GetCursorInfo(&cursor_info.info);

	constexpr auto cursor_threshold = 0x1FFFFull;

	const auto cursor_handle = reinterpret_cast<uintptr_t>(cursor_info.info.hCursor);

	return !(cursor_handle > cursor_threshold);
}

void clicker::click(HWND window) {

	auto single_click = [window = std::move(window)]() mutable {
		SendMessage(window, WM_LBUTTONDOWN, 0, 0);
		if (clicker::prioritize_higher_cps) { wait(random(420, 470) / random(clicker::cps - clicker::mincps, clicker::cps + clicker::maxcps)); }
		else { wait(random(420, 510) / random(clicker::cps - clicker::mincps, clicker::cps + clicker::maxcps));
		}
		

		SendMessage(window, WM_LBUTTONUP, 0, 0);
		if (clicker::prioritize_higher_cps) { wait(random(430, 480) / random(clicker::cps - clicker::mincps, clicker::cps + clicker::maxcps)); }
		else { wait(random(440, 510) / random(clicker::cps - clicker::mincps, clicker::cps + clicker::maxcps));
		}
		
	};

	auto double_click = [window = std::move(window)]() mutable {
		clicker::roll = random(1, 100);
		if (clicker::roll <= clicker::doubleclickchance) {

			SendMessage(window, WM_LBUTTONDOWN, 0, 0);
			wait(random(clicker::mindoubleclickdelay, clicker::maxdoubleclickdelay));

			SendMessage(window, WM_LBUTTONUP, 0, 0);
			wait(random(clicker::mindoubleclickdelay, clicker::maxdoubleclickdelay));
		}
	};

	auto drop = []() {
		clicker::roll = random(1, 100);
		if (clicker::roll <= clicker::dropchance) {
			clicker::dropgraceperiodcounter++;
			if (clicker::dropgraceperiodcounter < clicker::dropgraceperiod) { return; }
			clicker::dropgraceperiodcounter = 0;

			wait(random(clicker::mindrop, clicker::maxdrop));
		}
	};

	single_click();
	double_click();
	drop();
}
void clicker::thread::main() {
	for (;; std::this_thread::sleep_for(std::chrono::milliseconds(1))) {

		auto mcwindow = [&]() -> std::optional<HWND> {
			if (auto window = FindWindow(clicker::game, nullptr); window != nullptr) {
				if (const auto LeftClick = GetAsyncKeyState(VK_LBUTTON); LeftClick && GetForegroundWindow() == window) {
					return window;
				}
			}
			return std::nullopt;
		};

		if (auto window = mcwindow(); window.has_value()) {
			if (clicker::menucheck()) {
				if (const auto shiftKeyState = GetAsyncKeyState(VK_SHIFT); shiftKeyState) {
					if (const auto [inventory, toggle] = std::tuple{ clicker::inventory, clicker::toggle }; inventory && toggle) { click(*window); }
				}
			}
			else {
				if (const auto [rmb_lock] = std::tuple{ clicker::rmb_lock }; rmb_lock && GetAsyncKeyState(VK_RBUTTON) & 0x8000) { continue; }
				if (const auto [toggle] = std::tuple{ clicker::toggle }; toggle) { click(*window); }
				else {
					wait(15);
				}
			}
		}
	}
}
#include <Windows.h>

namespace clicker {
	namespace thread {
		extern void main();
	}

	std::string formatCPS(int cps, int mincps, int maxcps);

	inline int mincps = 3; 
	inline int maxcps = 4;
	inline int cps = 12;
	inline int mindoubleclickdelay = 6;
	inline int maxdoubleclickdelay = 11;
	inline int doubleclickchance = 25;
	inline int dropchance = 10;
	inline int mindrop = 75;
	inline int maxdrop = 125;
	inline int dropgraceperiod = 9;
	inline int roll;
	inline int dropgraceperiodcounter = 0;

	//inline char bind = 0;

	inline bool inventory = false;
	inline bool toggle = false;
	inline bool prioritize_higher_cps = false;
	inline bool rmb_lock = false;

	void click(HWND window);

	constexpr auto game = "LWJGL";

	auto menucheck() -> bool;
}
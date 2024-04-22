#include <thread>
#include <random>
#include <optional>
#include <mutex>
#include <Windows.h>
#include <variant>

#include "StdAfx.h"

#pragma comment(lib, "winmm.lib")

NTSYSAPI NTSTATUS	NTAPI NtDelayExecution(_In_ BOOLEAN Alertable, _In_opt_ PLARGE_INTEGER DelayInterval);

inline decltype(&NtDelayExecution) pNtDelayExecution{};

__forceinline static void wait(std::uint64_t delay_interval)
{
	static auto grab_nt_delay_execution = [&]() -> bool
		{
			pNtDelayExecution = reinterpret_cast<decltype(pNtDelayExecution)>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtDelayExecution"));

			return true;
		};

	static auto _ = grab_nt_delay_execution();

	timeBeginPeriod(1);

	LARGE_INTEGER _delay_interval{};
	_delay_interval.QuadPart = -static_cast<LONGLONG>(delay_interval * static_cast<std::uint64_t>(10'000));

	pNtDelayExecution(false, &_delay_interval);

	timeEndPeriod(1);
}

inline float random(float min, float max)
{
	using MaybeFloat = std::variant<float, bool>;

	auto generateRandom = [&](float min, float max, MaybeFloat last_generated) -> float {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min, max);

		float result;
		do {
			result = dis(gen);
		} while (std::holds_alternative<float>(last_generated) && result == std::get<float>(last_generated));

		return result;
		};

	static MaybeFloat last_generated;
	float result = generateRandom(min, max, last_generated);

	last_generated = result;

	return result;
}
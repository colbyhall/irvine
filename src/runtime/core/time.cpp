// Copyright Colby Hall. All Rights Reserved.

#include <core/time.h>
#include <core/containers/option.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif

CORE_NAMESPACE_BEGIN

f32 Duration::as_secs_f32() const {
	return (f32)m_secs + ((f32)m_nanos / (f32)NANOS_PER_SEC);
}

f64 Duration::as_secs_f64() const {
	return (f64)m_secs + ((f64)m_nanos / (f64)NANOS_PER_SEC);
}

#ifdef PLATFORM_WINDOWS

// Thread local so we don't have to lock
static thread_local Option<u64> g_timer_frequency;

Instant Instant::now() {
	LARGE_INTEGER ticks;
	const auto result = QueryPerformanceCounter(&ticks);
	ASSERT(result);
	return { static_cast<u64>(ticks.QuadPart) };
}

Duration Instant::duration_since(Instant earlier) const {
	if (!g_timer_frequency.is_set()) {
		LARGE_INTEGER freq;
		const auto result = QueryPerformanceFrequency(&freq);
		ASSERT(result);
		g_timer_frequency = freq.QuadPart;
	}
	const auto& freq = g_timer_frequency.as_ref().unwrap();

	auto duration =  m_tick - earlier.m_tick;
	const auto secs = duration / freq;
	duration -= secs * freq;
	const auto nanos = (u32)(duration * (NANOS_PER_SEC / freq));

	return Duration(secs, nanos);
}

#endif

CORE_NAMESPACE_END

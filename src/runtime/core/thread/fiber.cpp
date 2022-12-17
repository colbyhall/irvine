// Copyright Colby Hall. All Rights Reserved.

#include <core/thread/fiber.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif

CORE_NAMESPACE_BEGIN
MSVC_SUPPRESS_WARNING(5039);

Fiber::~Fiber() {
	if (!m_handle) return;

	if (!m_thread) DeleteFiber(m_handle);
	m_handle = nullptr;
}

void Fiber::switch_to() {
	SwitchToFiber(m_handle);
}

static void WINAPI FiberProc(_In_ LPVOID lpParameter) {
	auto& info = *reinterpret_cast<Function<void()>*>(lpParameter);
	info();
	info.~Function();
	mem::free(&info);
}

Fiber Fiber::spawn(Fiber::Function&& spawn) {
	auto memory = mem::alloc(mem::Layout::single<Fiber::Function>);
	Fiber::Function* param = new(memory) Fiber::Function(forward<Fiber::Function>(spawn));

	auto* handle = CreateFiber(0, &FiberProc, param);
	return Fiber{ false, handle };
}

Fiber Fiber::convert() {
	auto* handle = ConvertThreadToFiber(nullptr);
	return Fiber{ true, handle };
}

CORE_NAMESPACE_END

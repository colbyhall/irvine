// Copyright Colby Hall. All Rights Reserved.

#include <core/thread/thread.h>

#ifdef PLATFORM_WINDOWS
	#include <core/platform/windows.h>
#endif

CORE_NAMESPACE_BEGIN
MSVC_SUPPRESS_WARNING(5039);

bool JoinHandle::join() {
	// TODO: Check the result so we can tell the user if the thread had already ended
	WaitForSingleObject(
			(HANDLE)m_thread.handle(),
			INFINITE
	);

	m_joined = true;
	return true;
}

static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {
	auto& info = *reinterpret_cast<Thread::Function*>(lpParameter);
	info();
	info.~Function();
	mem::free(&info);
	return 0;
}

JoinHandle Thread::spawn(bool start, Thread::Function&& spawn) {
	auto memory = mem::alloc(mem::Layout::single<Thread::Function>);
	Thread::Function* param = new(memory) Thread::Function(forward<Thread::Function>(spawn));

	auto flags = 0;
	if (!start) {
		flags = CREATE_SUSPENDED;
	}

	DWORD id;
	HANDLE handle = CreateThread(
		nullptr,
		0,
		&ThreadProc,
		(LPVOID)param,
		flags,
		&id
	);
	ASSERT(handle != nullptr);
	return JoinHandle { handle, id };
}

Thread Thread::current() {
	return Thread{ GetCurrentThread(), GetCurrentThreadId() };
}

void Thread::set_affinity(u32 core) const {
	auto result = SetThreadAffinityMask((HANDLE)m_handle, (DWORD_PTR)((u64)1 << (u64)(core + 1)));
	ASSERT(result != 0);
}

void Thread::resume() const {
	ResumeThread(m_handle);
}

void sleep(const Duration& duration) {
	Sleep((DWORD)duration.as_millis());
}

u32 logical_core_count() {
	SYSTEM_INFO info = {};
	GetSystemInfo(&info);
	return (u32)info.dwNumberOfProcessors;
}

CORE_NAMESPACE_END

// Copyright Colby Hall. All Rights Reserved.

#include <gui/window.hpp>

#ifdef PLATFORM_WINDOWS

#include <core/containers/wstring.hpp>
#include <core/platform/library.hpp>

#include <core/platform/windows.hpp>

// NOTE: Could #include <hidusage.h> for these defines
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

using gui::Window;

static LRESULT CALLBACK window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	auto* window = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);

	if (window == nullptr) {
		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	}

	switch (Msg) {
	case WM_DESTROY:
	case WM_CLOSE:
		ExitProcess(0);
		break;
	// case WM_PAINT:
		// window->on_paint();
		// OutputDebugStringA("Painting Screen\n");
		// return 0;
	case WM_SIZE:
	case WM_SIZING:
		// window->on_resize();
		return 0;
	default:
		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	}
}

GUI_NAMESPACE_BEGIN

Shared<Window> Window::make(const WindowConfig& config) {
	HINSTANCE hInstance = GetModuleHandleA(nullptr);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	WNDCLASSEXW window_class = {};
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = &window_proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = hInstance;
	window_class.hIcon = ::LoadIconW(hInstance, nullptr);
	window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = L"window_class";
	window_class.hIconSm = ::LoadIconW(hInstance, nullptr);

	static bool registered = true;
	if (registered) {
		registered = false;
		const ATOM atom = RegisterClassExW(&window_class);
		ASSERT(atom != 0);
	}

	const auto dpi = (f32)GetDpiForSystem() / 96.f;

	RECT adjusted_rect = {
		.left = 0,
		.top = 0,
		.right = (LONG)((f32)config.size.width * dpi),
		.bottom = (LONG)((f32)config.size.height * dpi)
	};
	AdjustWindowRect(&adjusted_rect, dwStyle, 0);

	const int width = adjusted_rect.right - adjusted_rect.left;
	const int height = adjusted_rect.bottom - adjusted_rect.top;

	const int monitor_width = GetSystemMetrics(SM_CXSCREEN);
	const int monitor_height = GetSystemMetrics(SM_CYSCREEN);

	const int x = monitor_width / 2 - width / 2;
	const int y = monitor_height / 2 - height / 2;

	WString title;
	title.push(config.title);

	HWND handle = CreateWindowExW(
		0,
		window_class.lpszClassName,
		title.ptr(),
		dwStyle,
		x, y, width, height,
		nullptr, nullptr,
		hInstance,
		nullptr
	);
	ASSERT(handle != INVALID_HANDLE_VALUE);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = handle;
	const auto rid_success = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	ASSERT(rid_success);

	int show = SW_HIDE;
	if (config.visible) {
		if (config.maximized) {
			show = SW_SHOWMAXIMIZED;
		} else if (config.minimized) {
			show = SW_SHOWMINIMIZED;
		} else {
			show = SW_SHOWDEFAULT;
		}
	}
	ShowWindow(handle, show);

	auto swapchain = gpu::Swapchain::make(handle);
	auto result = make_shared<Window, SMode::NonAtomic>((void*)handle, core::move(swapchain));
	SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR)&*result);
	return result;
}

bool Window::set_visible(bool visible) {
	return ShowWindow((HWND)m_handle, visible ? SW_SHOW : SW_HIDE) == 1;
}

bool Window::set_cursor_lock(bool locked) {
	if (locked) {
		POINT cursor_pos;
		GetCursorPos(&cursor_pos);
		RECT cursor_rect;
		cursor_rect.left = cursor_pos.x;
		cursor_rect.right = cursor_pos.x;
		cursor_rect.bottom = cursor_pos.y;
		cursor_rect.top = cursor_pos.y;
		return ClipCursor(&cursor_rect) > 0;
	}
	return ClipCursor(nullptr) > 0;
}

void Window::set_cursor_visbility(bool visible) {
	ShowCursor(visible);
}

AABB2u32 Window::client() const {
	RECT rect;
	GetClientRect((HWND)m_handle, &rect);
	const auto inv_dpi = 1.f / dpi();
	return AABB2u32::from_min_max(
		{ (u32)((f32)rect.left * inv_dpi), (u32)((f32)rect.top * inv_dpi) },
		{ (u32)((f32)rect.right * inv_dpi), (u32)((f32)rect.bottom * inv_dpi) }
	);
}

f32 Window::dpi() const {
	return (f32)GetDpiForWindow((HWND)m_handle) / 96.f;
}

void pump_events() {
	MSG msg;
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

GUI_NAMESPACE_END

#endif

// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/gui.h>

#include <core/non_copyable.h>
#include <core/containers/shared.h>
#include <core/containers/string_view.h>
#include <core/math/aabb2.h>

#include <gpu/swapchain.h>

GUI_NAMESPACE_BEGIN

struct WindowConfig {
    StringView title;

    Vec2u32 size = { 1280, 720 };

    bool visible = true;
    bool maximized = false;
    bool minimized = false;
};

class Window final : private NonCopyable {
public:
    static Shared<Window> make(const WindowConfig& config);

    // FIXME: This should be private
    FORCE_INLINE explicit Window(void* handle, gpu::Swapchain&& swapchain)
        : m_handle(handle), m_swapchain(core::forward<gpu::Swapchain>(swapchain)) {}

    bool set_visible(bool visible);
    bool set_cursor_lock(bool locked);
    void set_cursor_visbility(bool visible);

    AABB2u32 client() const;
    f32 dpi() const;
    FORCE_INLINE void* handle() const { return m_handle; }
    FORCE_INLINE gpu::Swapchain& swapchain() { return m_swapchain; }
    FORCE_INLINE const gpu::Swapchain& swapchain() const { return m_swapchain; }

private:
    void* m_handle;
    gpu::Swapchain m_swapchain;
};

void pump_events();

GUI_NAMESPACE_END

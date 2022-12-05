// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/gui.hpp>

#include <core/non_copyable.hpp>
#include <core/containers/shared.hpp>
#include <core/containers/string_view.hpp>
#include <core/math/aabb2.hpp>

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
    FORCE_INLINE explicit Window(void* handle) : m_handle(handle) {}

    bool set_cursor_lock(bool locked);
    void set_cursor_visbility(bool visible);

    AABB2u32 client() const;
    f32 dpi() const;
    FORCE_INLINE void* handle() const { return m_handle; }

private:
    void* m_handle;
};

Shared<Window, SMode::NonAtomic> make_window(const WindowConfig& config);

void pump_events();

GUI_NAMESPACE_END
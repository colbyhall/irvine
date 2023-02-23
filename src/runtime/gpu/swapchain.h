// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/unique.h>
#include <core/math/vec2.h>

#include <gpu/gpu.h>

GPU_NAMESPACE_BEGIN

class Texture;

class SwapchainInterface : private NonCopyable {
public:
    virtual const Texture& back_buffer() const = 0;
    virtual void present() = 0;
    virtual void wait_for_previous() = 0;
	virtual void resize() = 0;
    virtual ~SwapchainInterface() {}
};

class Swapchain {
public:
    static Swapchain make(void* window_handle);

    FORCE_INLINE const Texture& back_buffer() const { return m_interface->back_buffer(); }
    FORCE_INLINE void present() { m_interface->present(); }
    FORCE_INLINE void wait_for_previous() { m_interface->wait_for_previous(); }
	FORCE_INLINE void resize() { m_interface->resize(); }

    template <typename T = SwapchainInterface>
    T const& interface() const {
        static_assert(core::is_base_of<SwapchainInterface, T>, "T is not derived of SwapchainInterface");
        return static_cast<const T&>(*m_interface);
    }

private:
    Swapchain(Unique<SwapchainInterface>&& interface) : m_interface(core::move(interface)) { }

    Unique<SwapchainInterface> m_interface;
};

GPU_NAMESPACE_END

// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/unique.h>

#include <gpu/graphics_command_list.h>

GPU_NAMESPACE_BEGIN

enum class Backend {
	D3D12,
};

class ContextInterface : private NonCopyable {
public:
	virtual Backend backend() const = 0;
	virtual void post_init() = 0;
	virtual void flush_queue() const = 0;
	virtual ~ContextInterface() {}
};

class Context {
public:
	static const Context& the();

	inline Backend backend() const { return m_interface->backend(); }
	inline void flush_queue() const { return m_interface->flush_queue(); }

	template <typename T = ContextInterface>
	T const& interface() const {
		static_assert(core::is_base_of<ContextInterface, T>, "T is not derived of ContextInterface");
		return static_cast<const T&>(*m_interface);
	}

private:
	inline Context(Unique<ContextInterface>&& interface)
		: m_interface(core::forward<Unique<ContextInterface>>(interface)) {}

	friend void init();

	Unique<ContextInterface> m_interface;
};

void init();

GPU_NAMESPACE_END

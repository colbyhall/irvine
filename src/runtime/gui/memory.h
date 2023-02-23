// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gui/id.h>
#include <core/containers/unique.h>
#include <core/containers/function.h>
#include <core/containers/hash_map.h>

GUI_NAMESPACE_BEGIN

struct Memory {
	virtual ~Memory() {}
};

struct MemorySlot {
	u32 size;
	u32 frame;
	Unique<Memory> memory;
};

class MemoryManager {
public:
	MemoryManager() = default;

	void step();

	template <typename T>
	Option<T const&> get(Id id) const;

	template <typename T>
	Option<T&> get_mut(Id id);

	template <typename T>
	T& get_mut_or_else(Id id, FunctionRef<T()> create);
	
private:
	HashMap<Id, MemorySlot> m_table;
	u32 m_frame = 0;
	u32 m_total_used = 0;
};

template <typename T>
Option<T const&> MemoryManager::get(Id id) const {
	auto opt_slot = m_table.find(id);
	if (opt_slot) {
		MemorySlot& slot = const_cast<MemorySlot>(opt_slot.unwrap());
		slot.frame = m_frame;
		return (T const&)*slot.memory;
	}
	return nullptr;
}

template <typename T>
Option<T&> MemoryManager::get_mut(Id id) {
	auto opt_slot = m_table.find_mut(id);
	if (opt_slot) {
		MemorySlot& slot = opt_slot.unwrap();
		slot.frame = m_frame;
		return (T&)*slot.memory;
	}
	return nullptr;
}

template <typename T>
T& MemoryManager::get_mut_or_else(Id id, FunctionRef<T()> create) {
	auto opt_slot = m_table.find_mut(id);
	if (opt_slot) {
		MemorySlot& slot = opt_slot.unwrap();
		slot.frame = m_frame;
		return (T&)*slot.memory;
	}

	auto value = create();
	auto memory = make_unique<T>(core::move(value));
	m_table.insert(id, MemorySlot{
		.size = sizeof(T),
		.frame = m_frame,
		.memory = core::move(memory),
	});
	m_total_used += sizeof(T);

	return (T&)*m_table.find_mut(id).unwrap().memory;
}

GUI_NAMESPACE_END

// Copyright Colby Hall. All Rights Reserved.

#include <gui/memory.h>

GUI_NAMESPACE_BEGIN

void MemoryManager::step() {
	// Free memory that was not used last frame
	m_table.retain([this](const Id& id, const MemorySlot& slot) {
		return slot.frame == m_frame;
	});

	m_frame += 1;
}

GUI_NAMESPACE_END

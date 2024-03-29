// Copyright Colby Hall. All Rights Reserved.

#include <core/memory.h>

SUPPRESS_WARNINGS_STD_BEGIN

#include <cstdlib>
#include <cstring>
#include <cstdio>

SUPPRESS_WARNINGS_STD_END

CORE_NAMESPACE_BEGIN

NonNull<void> alloc(Layout layout) {
	void* result = std::malloc(layout.size);
	return result;
}

NonNull<void> realloc(NonNull<void> old_ptr, Layout old_layout, Layout new_layout) {
	(void)old_layout; // Unused currently

	void* result = std::realloc(old_ptr, new_layout.size);
	return result;
}

void free(NonNull<void> ptr) {
	std::free(ptr);
}

NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count) {
	return std::memcpy(dst, src, count);
}

NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count) {
	return std::memmove(dst, src, count);
}

NonNull<void> set(NonNull<void> ptr, u8 value, usize count) {
	return std::memset(ptr, value, count);
}

#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define COUNT_BITS B6(0), B6(1), B6(1), B6(2)

static u8 count_set_bits_table[256] = { COUNT_BITS };

u8 count_ones(u8 b) {
	return count_set_bits_table[b];
}

CORE_NAMESPACE_END

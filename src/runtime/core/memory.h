// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/non_null.h>

SUPPRESS_WARNINGS_STD_BEGIN
#include <new>
SUPPRESS_WARNINGS_STD_END

#define CORE_MEM_NAMESPACE_BEGIN                                                \
    SUPPRESS_WARNING_PUSH                                                       \
    MSVC_SUPPRESS_WARNING(4268)                                                 \
    namespace mem {

#define CORE_MEM_NAMESPACE_END                                                  \
    SUPPRESS_WARNING_POP                                                        \
    }

CORE_NAMESPACE_BEGIN
CORE_MEM_NAMESPACE_BEGIN

struct Layout {
    usize size;
    usize alignment;

    template <typename T>
    static inline constexpr Layout single = { sizeof(T), alignof(T) };

    template <typename T>
    static constexpr Layout array(usize len) {
        return Layout{ sizeof(T) * len, alignof(T) };
    }
};

NonNull<void> alloc(Layout layout);
NonNull<void> realloc(NonNull<void> old_ptr, Layout old_layout, Layout new_layout);
void free(NonNull<void> ptr);

NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> set(NonNull<void> ptr, u8 value, usize count);

u8 count_ones(u8 b);

template <typename T>
inline u32 count_ones(T t) {
	const usize size = sizeof(T);
	void* ptr = &t;
	u8 const* u8_casted = (u8 const*)ptr;

	u32 result = 0;
	for (usize i = 0; i < size; ++i) {
		result += count_ones(u8_casted[i]);
	}

	return result;
}

CORE_MEM_NAMESPACE_END
CORE_NAMESPACE_END

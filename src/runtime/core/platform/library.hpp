// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.hpp>
#include <core/containers/option.hpp>
#include <core/platform/fs.hpp>

CORE_NAMESPACE_BEGIN

class Library : private NonCopyable {
public:
    Library() : m_handle(nullptr) { }
    static Option<Library> open(PathView path);
    ~Library();

    FORCE_INLINE Library(Library&& move) noexcept : m_handle(move.m_handle) {
        move.m_handle = nullptr;
    }
    FORCE_INLINE Library& operator=(Library&& move) noexcept {
        Library to_destroy = core::move(*this);
        m_handle = move.m_handle;
        move.m_handle = nullptr;
        return *this;
    }

    void* find(StringView name);

private:
    FORCE_INLINE Library(void* handle) : m_handle(handle) { }

    void* m_handle;
};

CORE_NAMESPACE_END

using core::Library;
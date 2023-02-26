// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/non_copyable.h>
#include <core/containers/option.h>
#include <core/fs/path_view.h>

CORE_NAMESPACE_BEGIN

class Library : private NonCopyable {
public:
    Library() : m_handle(nullptr) { }
    static Option<Library> open(PathView path);
    ~Library();

    inline Library(Library&& move) noexcept : m_handle(move.m_handle) {
        move.m_handle = nullptr;
    }
    inline Library& operator=(Library&& move) noexcept {
        Library to_destroy = core::move(*this);
        m_handle = move.m_handle;
        move.m_handle = nullptr;
        return *this;
    }

    void* find(StringView name);

private:
    inline Library(void* handle) : m_handle(handle) { }

    void* m_handle;
};

CORE_NAMESPACE_END

using core::Library;

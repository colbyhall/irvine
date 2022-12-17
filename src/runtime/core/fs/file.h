// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/result.h>
#include <core/containers/string.h>
#include <core/fs/path_view.h>

CORE_NAMESPACE_BEGIN

BITFLAG(FileFlags) {
    FF_Read = (1 << 0),
    FF_Write = (1 << 1),
    FF_Create = (1 << 2)
};

enum class FileOpenError : u8 {
    NotFound,
    InUse
};

enum class Seek : u8 {
    Begin,
    Current,
    End
};

class File : private NonCopyable {
public:
    static Result<File, FileOpenError> open(PathView path, FileFlags flags);

    FORCE_INLINE File(File&& move) noexcept : m_handle(move.m_handle), m_flags(move.m_flags), m_cursor(0) {
        move.m_handle = nullptr;
    }
    FORCE_INLINE File& operator=(File&& move) noexcept {
        File to_destroy = core::move(*this);
        m_handle = move.m_handle;
        m_flags = move.m_flags;
        move.m_handle = nullptr;
        return *this;
    }

    usize size() const;
    usize cursor() const { return m_cursor; }
    usize seek(Seek method, isize distance);
    FORCE_INLINE void rewind() { seek(Seek::Begin, 0); }
    void set_eof();

    usize read(Slice<u8> buffer);
    void write(Slice<const u8> buffer);

    ~File();

private:
    FORCE_INLINE File(void* handle, FileFlags flags) : m_handle(handle), m_flags(flags), m_cursor(0) {}

    void* m_handle;
    FileFlags m_flags;
    usize m_cursor;
};

struct MetaData {
    u64 creation_time;
    u64 last_access_time;
    u64 last_write_time;

    usize size;
    bool read_only;
};

Result<String, FileOpenError> read_to_string(PathView path);

CORE_NAMESPACE_END

using core::FF_Read;
using core::FF_Write;
using core::FF_Create;
using core::File;

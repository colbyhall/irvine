// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.hpp>
#include <core/containers/result.hpp>
#include <core/containers/string.hpp>

CORE_NAMESPACE_BEGIN

class PathView {
public:
    FORCE_INLINE constexpr PathView() : m_string() {}
    FORCE_INLINE constexpr PathView(Slice<char const> bytes) : m_string(bytes) {}
    FORCE_INLINE constexpr PathView(const char* ptr) : m_string({ ptr, core::strlen(ptr) }) {}

    FORCE_INLINE operator StringView() const { return m_string; }
    FORCE_INLINE const char* operator*() const { return *m_string; }
    
    FORCE_INLINE usize len() const { return m_string.len(); }
    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_string); }

private:
    StringView m_string;
};

class Path {
public:
    FORCE_INLINE constexpr Path() : m_string() {}
    explicit inline Path(PathView view) : m_string(String::from(view)) {}
    FORCE_INLINE Path(String&& string) noexcept : m_string(core::forward<String>(string)) {}

    FORCE_INLINE static Path from(StringView string) { return Path(String::from(string)); }
    FORCE_INLINE static Path from(WStringView wstring) { return Path(String::from(wstring)); }

    FORCE_INLINE operator StringView() const { return m_string; }
    FORCE_INLINE operator PathView() const { return PathView(m_string); }
    FORCE_INLINE const char* operator*() const { return *m_string; }

    FORCE_INLINE usize len() const { return m_string.len(); }
    FORCE_INLINE CodepointsIterator codepoints() const { return CodepointsIterator(m_string); }

private:
    String m_string;
};

Path cwd();

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

enum class DirectoryItemType : u8 {
    File,
    Directory,
    Unknown,
};

struct DirectoryItem {
    Path path;
    DirectoryItemType type;
    MetaData meta_data;
};

Array<DirectoryItem> read_directory(PathView path, bool recursive = true);

CORE_NAMESPACE_END

using core::PathView;
using core::Path;
using core::FF_Read;
using core::FF_Write;
using core::FF_Create;
using core::File;
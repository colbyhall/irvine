// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/function.h>
#include <core/fs/file.h>
#include <core/fs/path.h>

CORE_NAMESPACE_BEGIN

enum class DirectoryItemType : u8 {
    File,
    Directory,
    Unknown,
};

struct DirectoryItem {
    PathView path;
    DirectoryItemType type;
    MetaData meta_data;
};

using ReadDirFunction = FunctionRef<bool(const DirectoryItem&)>;

void read_dir(PathView path, ReadDirFunction function);
void read_dir_recursive(PathView path, ReadDirFunction function);

Path cwd();

CORE_NAMESPACE_END

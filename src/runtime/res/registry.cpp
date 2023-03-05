// Copyright Colby Hall. All Rights Reserved.

#include <core/fs/dir.h>

#include <res/context.h>

RES_NAMESPACE_BEGIN

void init() {
	core::read_dir_recursive("../../res", [](auto& item) {
		const auto ext = item.path.extension();
		item.path;
		return true;
	});
}

RES_NAMESPACE_END

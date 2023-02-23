// Copyright Colby Hall. All Rights Reserved.

#include <gui/id.h>

void hash(Hasher& hasher, const gui::Id& value) {
	::hash(hasher, (u64)value);
}

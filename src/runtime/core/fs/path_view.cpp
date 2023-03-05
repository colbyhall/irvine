// Copyright Colby Hall. All Rights Reserved.

#include <core/fs/path_view.h>

CORE_NAMESPACE_BEGIN

Option<StringView> PathView::extension() const {
	auto copy = m_string;
	return copy.rsplit('.');
}

CORE_NAMESPACE_END

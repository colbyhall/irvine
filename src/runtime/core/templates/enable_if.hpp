// Copyright Colby Hall. All Rights Reserved.

#pragma once

namespace core {
	// https://en.cppreference.com/w/cpp/types/enable_if
	template <bool B, typename T = void>
	class EnableIf;

	template <typename T>
	struct EnableIf<true, T> {
		using Type = T;
	};

	template <typename T>
	class EnableIf<false, T> { };

	template <bool B, typename T = void>
	using EnabledIf = typename EnableIf<B, T>::Type;
}
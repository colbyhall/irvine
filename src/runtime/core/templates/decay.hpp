// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.hpp>
#include <core/templates/remove.hpp>
#include <core/templates/type_traits.hpp>

CORE_NAMESPACE_BEGIN

namespace hidden {
	template <typename T>
	struct DecayNonReference {
		using Type = CVRemoved<T>;
	};

	template <typename T>
	struct DecayNonReference<T[]> {
		using Type = T*;
	};

	template <typename T, u32 N>
	struct DecayNonReference<T[N]> {
		using Type = T*;
	};

	template <typename R, typename... P>
	struct DecayNonReference<R(P...)> {
		typedef R (*Type)(P...);
	};

	template <typename T>
	struct Decay {
		using Type = typename hidden::DecayNonReference<ReferenceRemoved<T>>::Type;
	};
}
	 
// http://en.cppreference.com/w/cpp/types/decay
template <typename T>
using Decayed = typename hidden::Decay<T>::Type;

CORE_NAMESPACE_END
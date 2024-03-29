// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/containers/array.h>
#include <core/containers/unique.h>

CORE_NAMESPACE_BEGIN

template <usize size, typename T>
class BucketArray {
public:
	struct Bucket {
		Array<T> values;
	};

	inline BucketArray() = default;

	usize len() const;
	usize cap() const;

	inline bool is_empty() const { return len() == 0; }
	inline explicit operator bool() const { return !is_empty(); }
	inline bool is_valid_index(usize index) const { return index < len(); }

	T& operator[](usize index);
	const T& operator[](usize index) const;

	void reserve(usize amount);
	usize push(T&& item);
	Option<T> pop();

private:
	Array<Bucket> m_buckets;
};

CORE_NAMESPACE_END

#include <core/containers/bucket_array.inl>

using core::BucketArray;

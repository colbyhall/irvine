// Copyright Colby Hall. All Rights Reserved.

#include <core/hash.hpp>

CORE_NAMESPACE_BEGIN

const u64 FNV1Hasher::offset_basic = 0xcbf29ce484222325;
const u64 FNV1Hasher::prime = 0x100000001b3;

u64 FNV1Hasher::finish() {
	return m_result;
}

void FNV1Hasher::write(Slice<u8 const> bytes) {
	u64 hash = FNV1Hasher::offset_basic;
	for (usize i = 0; i < bytes.len(); ++i) {
		hash *= FNV1Hasher::prime;
		hash = hash ^ bytes[i];
	}
	m_result |= hash;
}

CORE_NAMESPACE_END

void hash(Hasher& hasher, const u8& value) {
	hasher.write(Slice<u8 const>({ &value, sizeof(u8) }));
}

void hash(Hasher& hasher, const u16& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u16)));
}

void hash(Hasher& hasher, const u32& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u32)));
}

void hash(Hasher& hasher, const u64& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u64)));
}

void hash(Hasher& hasher, const i8& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i8)));
}

void hash(Hasher& hasher, const i16& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i16)));
}

void hash(Hasher& hasher, const i32& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i32)));
}

void hash(Hasher& hasher, const i64& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i64)));
}

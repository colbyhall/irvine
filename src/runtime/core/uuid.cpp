// Copyright Colby Hall. All Rights Reserved.

#include <core/uuid.h>
#include <core/time.h>
#include <core/memory.h>

#include <core/platform/windows.h>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

CORE_NAMESPACE_BEGIN

const Uuid Uuid::nil(0, 0, 0, 0);

Uuid Uuid::generate() {
	// https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom
	UCHAR buffer[16] = {};
	auto gen_result = ::BCryptGenRandom(
		nullptr,
		buffer,
		16,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);
	ASSERT(NT_SUCCESS(gen_result));

	auto result = Uuid(0, 0, 0, 0);
	core::copy(&result, &buffer, 16);
	return result;
}

bool Uuid::operator==(const Uuid& right) const {
	return
		m_a == right.m_a &&
		m_b == right.m_b &&
		m_c == right.m_c &&
		m_d == right.m_d;
}

CORE_NAMESPACE_END

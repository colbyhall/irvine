// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <res/res.h>

RES_NAMESPACE_BEGIN

class Resource {
public:
	virtual ~Resource() = default
};

#define DECLARE_RESOURCE(n)																			\
	class n : public Resource

DECLARE_RESOURCE(Test) {
	
};

RES_NAMESPACE_END

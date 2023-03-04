// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <draw/draw.h>

DRAW_NAMESPACE_BEGIN

class Canvas;

class Shape {
public:
	virtual void triangulate(Canvas& canvas) const = 0;
	virtual ~Shape() {}
};

DRAW_NAMESPACE_END

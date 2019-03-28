#pragma once
#include "Rect.h"

rect::rect(const rect &a)
{
	pos = a.pos;
	size = a.size;
}

rect::rect()
{
	x = 0.0f;
	y = 0.0f;
	width = 0.0f;
	height = 0.0f;
}

void rect::operator=(const rect &rhs)
{
	pos = rhs.pos;
	size = rhs.size;
}

void rect::setMax(const vec2 &maxPos)
{
	size = maxPos - pos;
}

void rect::setMin(const vec2 &minPos)
{
	pos = minPos;
}

void rect::setMaxX(float maxPosX)
{
	size.x = maxPosX - pos.x;
}

void rect::setMinX(float minPosX)
{
	pos.x = minPosX;
}

void rect::setMaxY(float maxPosY)
{
	size.y = maxPosY - pos.y;
}

void rect::setMinY(float minPosY)
{
	pos.y = minPosY;
}

vec2 rect::getMax() const
{
	return pos + size;
}

vec2 rect::getMin() const
{
	return pos;
}

vec2 rect::getCenter()
{
	return pos + size * 0.5f;
}

void rect::setCenter(const vec2 &center)
{
	pos = center - size * 0.5f;
}

// Checks whether a point is inside the rect
bool rect::contains(const vec2 & point) const
{
	vec2 thisMax = this->getMax();
	return (point.x > this->x && point.x < thisMax.x &&
			point.y > this->y && point.y < thisMax.y);
}

//TODO: Reorganize function?
// Checks whether the rect is inside another rect
bool rect::contains(const rect & other) const
{
	vec2 otherMax = other.getMax();
	vec2 thisMax = this->getMax();
	return (other.x < this->x && otherMax.x > thisMax.x &&
		other.y < this->y && otherMax.y > thisMax.y);
}

// Checks whether two rects are colliding
bool rect::overlaps(const rect & other) const
{
	vec2 otherMax = other.getMax();
	vec2 thisMax = this->getMax();
	return (other.x < thisMax.x && otherMax.x > this->x &&
			other.y < thisMax.y && otherMax.y > this->y);
}

float rect::getAspect() const
{
	return width / height;
}

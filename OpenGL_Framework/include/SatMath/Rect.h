#pragma once
#include <gmtk/gmtk.h>
#include <SatMath/sys/IgnoreWarning.h>

IGNORE_WARNING_PUSH
IGNORE_WARNING_NAMELESS_UNION_STRUCT

struct rect
{
	union
	{
		struct
		{
			float x, y;
			float width, height;
		}; 
		struct
		{
			float x, y;
			float w, h;
		}; 
		struct
		{
			vec2 pos;
			vec2 size;
		};
	};

	rect(const rect &);
	rect();
	
	void operator=(const rect &rhs);

	void setMax(const vec2 &maxPos);
	void setMin(const vec2 &minPos);
	void setMaxX(float maxPos);
	void setMinX(float minPos);
	void setMaxY(float maxPos);
	void setMinY(float minPos);

	vec2 getMax() const;
	vec2 getMin() const;
	vec2 getCenter();
	void setCenter(const vec2 &center);
	bool contains(const vec2 &point) const;
	bool contains(const rect &other) const;
	bool overlaps(const rect &other) const;
	float getAspect() const;
};

IGNORE_WARNING_POP
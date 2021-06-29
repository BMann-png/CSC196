#pragma once

#ifdef DYNAMIC_EXPORTS
#define DYNAMIC_API _declspec(dllexport)
#else
#define DYNAMIC_API _declspec(dllimport)
#endif

namespace nc
{
	struct DYNAMIC_API point
	{
		float x;
		float y;

		point(float x, float y) : x{ x }, y{ y } {}
		point operator + (point p) { return point{ p.x + x, p.y + y }; }
		//can be:
		//point operator + (point p) { return { p.x + x, p.y + y }; }
	};
}
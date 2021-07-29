#pragma once
#include "core.h"
#include <fstream>

namespace pbls
{
	struct Color
	{
		float r, g, b;

		Color() : r{ 0 }, g{ 0 }, b{ 0 } {}
		Color(float r, float g, float b) : r{ r }, g{ g }, b{ b } {}
		Color(DWORD rgb) //DWORD( x | b | g | r)
		{
			r = (rgb & 0xff) / 255.0f;
			g = ((rgb >> 8) & 0xff) / 255.0f;
			b = ((rgb >> 16) & 0xff) / 255.0f;
		}

		Color operator - (const Color& color) { return { r - color.r, g - color.g, b - color.b }; }
		Color operator + (const Color& color) { return { r + color.r, g + color.g, b + color.b }; }
		Color operator * (float s) const { return { r * s, g * s, b * s }; }
		operator DWORD() const { return ToRGB(); }

		DWORD ToRGB() const
		{
			BYTE red = static_cast<BYTE>(r * 255);
			BYTE green = static_cast<BYTE>(g * 255);
			BYTE blue = static_cast<BYTE>(b * 255);

			return (red | green << 8 | blue << 16);
		}

		friend std::istream& operator >> (std::istream& stream, Color& c);

		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color purple;
		static const Color cyan;
		static const Color orange;
		static const Color yellow;
		static const Color black;
	};
}
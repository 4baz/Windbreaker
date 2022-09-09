#pragma once

namespace hbase
{
#pragma pack(push, 1)
	struct Color
	{
		std::uint8_t r{ 255 };
		std::uint8_t g{ 255 };
		std::uint8_t b{ 255 };
		std::uint8_t a{ 255 };
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct RGBAF
	{
		int r, g, b, a, f;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct RGBA
	{
		int r, g, b, a;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct RGBAD
	{
		int R, G, B, A;
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct RGB
	{
		int r, g, b;
	};
#pragma pack(pop)
}

#pragma once

namespace Constant
{
	// Window Property
	const float width = 1024.0f;
	const float height = 768.0f;

	// Grid Property
	const int gridXCount = 1000;
	const int gridYCount = 1000;
	const float gridCellSize = 0.5f;
}

namespace State
{
	const float AIR	= 0.0f;
	const float FLUID = 1.0f;
	const float BOUNDARY = 2.0f;
}
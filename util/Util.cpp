#include <Windows.h>
#include "../Common/typedef.h"
#include "Util.h"


BOOL CalcClipArea(INT_VECTOR2* pivOutSrcStart, INT_VECTOR2* pivOutDestStart, INT_VECTOR2* pivOutDestSize, const INT_VECTOR2* pivPos, const INT_VECTOR2* pivImageSize, const INT_VECTOR2* pivBufferSize)
{
	int destStartX = max(pivPos->x, 0);
	int destStartY = max(pivPos->y, 0);
	destStartX = min(destStartX, pivBufferSize->x);
	destStartY = min(destStartY, pivBufferSize->y);

	int destEndX = max(pivPos->x + pivImageSize->x, 0);
	int destEndY = max(pivPos->y + pivImageSize->y, 0);
	destEndX = min(destEndX, pivBufferSize->x);
	destEndY = min(destEndY, pivBufferSize->y);

	int	width = destEndX - destStartX;
	int	height = destEndY - destStartY;

	if (width <= 0 || height <= 0)
	{
		return FALSE;
	}	

	int src_start_x = destStartX - pivPos->x;
	int src_start_y = destStartY - pivPos->y;

	pivOutSrcStart->x = src_start_x;
	pivOutSrcStart->y = src_start_y;
	pivOutDestStart->x = destStartX;
	pivOutDestStart->y = destStartY;
	pivOutDestSize->x = width;
	pivOutDestSize->y = height;

	return TRUE;
}

BOOL IsCollisionRectVsRect(const INT_VECTOR2* pv3MinA, const INT_VECTOR2* pv3MaxA, const INT_VECTOR2* pv3MinB, const INT_VECTOR2* pv3MaxB)
{
	const int*	a_min = &pv3MinA->x;
	const int*	a_max = &pv3MaxA->x;
	const int*	b_min = &pv3MinB->x;
	const int*	b_max = &pv3MaxB->x;

	for (DWORD i = 0; i < 2; i++)
	{
		if (a_min[i] > b_max[i] || a_max[i] < b_min[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}
#include "stdafx.h"

//产生随机数

int random()
{
	int i;
	i = 1 + rand() % Max;
	if (i <= 5)
	{
		return 1;
	}
	else if (i >= 95)
	{
		return 2;
	}
	else return -1;
	//return i;
}

#pragma once
#ifndef __COST__
#define __COST__

namespace onlabor
{
	class Cost
	{
		static float* Quadratic(float* out, float* expected);
		static float* Quadratic_D(float * result, float* out, float* expected, int size);
	};
}
#endif
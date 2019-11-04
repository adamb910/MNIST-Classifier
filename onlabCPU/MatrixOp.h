#pragma once

#ifndef __MATOP__
#define __MATOP__

namespace onlabor
{
	class MatrixOperations
	{
	public:
		static void Transponse(float* result, float * matrix, int m_h, int m_w);
	};
}
#endif
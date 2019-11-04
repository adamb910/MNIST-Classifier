#pragma once
#ifndef __MDATA__
#define __MDATA__
#include <Matrices.h>

namespace onlabor
{
	class mnist_matrix
	{
	public:
		Matrix input;
		Matrix output;

		mnist_matrix(Matrix in, Matrix out)
		{
			input = in;
			output = out;

		}
	};
}
#endif
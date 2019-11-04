#include "MatrixOp.h"

void onlabor::MatrixOperations::Transponse(float* result, float * matrix, int m_h, int m_w)
{
	float* helper = new float[m_h*m_w];
	int i = 0;
	int index = 0;
	while (index != (m_h*m_w))
	{

		helper[index] = matrix[(index % (m_h)) * m_w + i];
		index++;

		if (index % m_h == 0)
			i++;
	}

	for (int j = 0; j < m_w * m_h; j++)
		result[j] = helper[j];

	delete[] helper;
}

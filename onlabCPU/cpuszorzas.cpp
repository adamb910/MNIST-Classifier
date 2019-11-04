
#include "cpuszorzas.h"
#include <algorithm>
#include <functional>


std::vector<float> onlabor::CPUSzorzo::Szorzas(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w)
{
	std::vector<float> result = std::vector<float>(a_h*b_w);
	float* resn = new float[a_h*b_w];

	for (int i = 0; i < a_h*b_w; i++)
		resn[i] = 0;

	int outSize = a_h;
	int res_w = b_w;
	//result height = a_h, result width = b_w
	// calculate activation of each neuron
	int i = 0;
	int k = 0;
	for (k = 0; k < b_w; k++)
	{
		for (int a_l = 0; a_l < a_h; a_l++)
		{

			int j = 0;
			for (j = 0; j < b_h; j++)
			{
				i = a_l * res_w + k;
				//i = (j%(a_w-1)) + k;
				resn[i] += a[((a_w) * a_l) + j] * b[(j * (b_w)) + k];
				int s = (((j%b_h) * (b_w)) + k);
			}
		}
	}

	for (int i = 0; i < a_h*b_w; i++)
		result[i] = resn[i];

	delete[] resn;

	
	return result;

}

std::vector<float> onlabor::CPUSzorzo::Dot(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w)
{
	std::vector<float> result = std::vector<float>(a.size()*b.size());

	for (int i = 0; i < a_h; i++)
	{
		for (int j = 0; j < b_w; j++)
		{
			result[i*b_w + j] = a[i] * b[j];
		}
	}
	return result;
}

/*	int outSize = a_h;

	//result height = a_h, result width = b_w
	// calculate activation of each neuron
	int k = 0;
	for (k = 0; k < b_w; k++)	
	{
		int i = 0;
		for (i = 0; i < a_h; i++)
		{			
			int j = 0;
			for (j = 0; j < a_w; j++)
			{
				result[i] = a[(i*a_w) + j] * b[(j * b_w) + k];
			}
		}
	}

}*/

std::vector<float> onlabor::CPUSzorzo::AddMtx(std::vector<float> a, std::vector<float> b, int height, int width)
{
	
	std::vector<float> result = std::vector<float>(height * width);
	for (int i = 0; i < height * width; i++)
		result[i] = a[i] + b[i];

	/*std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(result), std::plus<float>());
	*/
	return result;
}

std::vector<float> onlabor::CPUSzorzo::SubtractMtx(std::vector<float> a, std::vector<float> b, int height, int width)
{
	std::vector<float> result = std::vector<float>(height * width);
	for (int i = 0; i < height * width; i++)
		result[i] = a[i] - b[i];

	return result;
}

std::vector<float> onlabor::CPUSzorzo::Hadamard(std::vector<float> a, std::vector<float> b, int height, int width)
{
	std::vector<float> result = std::vector<float>(height * width);
	for (int i = 0; i < height * width; i++)
		result[i] = a[i] * b[i];

	return result;
}

std::vector<float> onlabor::CPUSzorzo::Convolution2D(std::vector<float> a, int ah, int aw, std::vector<float> b, int bh, int bw)
{
	return std::vector<float>();
}

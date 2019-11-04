#pragma once
#ifndef __GPUSZOR__
#define __GPUSZOR__
#include <vector>

namespace onlabor
{
	class GPUSzorzo
	{
	

	public:
		static std::vector<float> Szorzas(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w);
		static std::vector<float> Dot(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w);
		static std::vector<float> AddMtx(std::vector<float> a, std::vector<float> b, int height, int width);
		static std::vector<float> SubtractMtx(std::vector<float> a, std::vector<float> b, int height, int width);
		static std::vector<float> Hadamard(std::vector<float> a, std::vector<float> b, int height, int width);
	};
}








#endif
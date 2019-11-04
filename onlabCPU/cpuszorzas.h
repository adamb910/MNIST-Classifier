#pragma once
#ifndef __CPUSZOR__
#define __CPUSZOR__

#include <iostream>
#include <vector>
#include <chrono>
#include <vector>
#include <ctime>
#include <ratio>

namespace onlabor
{
	class CPUSzorzo
	{

	public:
		static std::vector<float> Szorzas(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w);
		static std::vector<float> Dot(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w);
		static std::vector<float> AddMtx(std::vector<float> a, std::vector<float> b, int height, int width);
		static std::vector<float> SubtractMtx(std::vector<float> a, std::vector<float> b, int height, int width);
		static std::vector<float> Hadamard(std::vector<float> a, std::vector<float> b, int height, int width);
		static std::vector<float> Convolution2D(std::vector<float> a, int ah, int aw, std::vector<float> b, int bh, int bw);
	};
}
#endif
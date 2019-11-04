#pragma once
#include <vector>
#include <Matrices.h>
#include <iostream>

namespace onlabor
{
//	class Matrix;

	class Transport
	{
	public:

		int Count()
		{
			int c = 0;
			for (int i = 0; i < matrices.size(); i++)
			{
				c += matrices[i].GetHeight() * matrices[i].GetWidth();
			}
			return c;
		}

		std::vector<Matrix> matrices = std::vector<Matrix>();
		Transport()
		{
			matrices = std::vector<Matrix>();
		}
		Transport(int i)
		{
			matrices = std::vector<Matrix>(i);
		}

		void Print()
		{
			for (int i = 0; i < matrices.size(); i++)
			{
				matrices[i].Print();
				std::cout << std::endl;
			}

		}

		float operator[](int idx)
		{
			//std::cout << " operator[] call" << std::endl;
			int endindex = 0;
			int pre = 0;
			for (int i = 0; i < matrices.size(); i++)
			{
				pre = endindex;
				endindex += (matrices[i].GetHeight() * matrices[i].GetWidth());
				//std::cout << "curmatrix size: " << matrices[i].GetHeight() * matrices[i].GetWidth() << std::endl;
				if (endindex - 1 >= idx)
					return matrices[i].content[idx-pre];
			}
			std::cout << "a throwolt index: " << idx << " endindex: " << endindex << " pre: " << pre << " idx-pre= " << (idx-pre) << std::endl;
			throw new exception("hiba a transport operator[]-ben");
			return -1;
		}

		void SetAt(int idx, float value)
		{
			int endindex = 0;
			int pre = 0;
			for (int i = 0; i < matrices.size(); i++)
			{
				pre = endindex;
				endindex += (matrices[i].GetHeight() * matrices[i].GetWidth());
				//std::cout << "curmatrix size: " << matrices[i].GetHeight() * matrices[i].GetWidth() << std::endl;
				if (endindex - 1 >= idx) {
					matrices[i].content[idx - pre] = value;
					return;
				}
			}
			std::cout << "a throwolt index: " << idx << " endindex: " << endindex << " pre: " << pre << " idx-pre= " << (idx - pre) << std::endl;
			throw new exception("hiba a transport setat()-ban");
		}
	};
}
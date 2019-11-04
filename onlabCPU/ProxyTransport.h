#pragma once
#include <Transport.h>
#include <Matrices.h>
namespace onlabor
{
	class ProxyTransport
	{
	public:
		std::vector<Matrix> matrices = std::vector<Matrix>();
		Transport* original = nullptr;
		ProxyTransport(Transport* orig, Transport *minta)
		{
			original = orig;
			matrices = std::vector<Matrix>(minta->matrices.size());
			for (int i = 0; i < matrices.size(); i++)
			{
				matrices[i] = new Matrix(minta->matrices[i].GetHeight(), minta->matrices[i].GetWidth());
			}
		}

		int Count()
		{
			int c = 0;
			for (int i = 0; i < original->matrices.size(); i++)
			{
				c += original->matrices[i].GetHeight() * original->matrices[i].GetWidth();
			}
			return c;
		}

		
		float getAt(int matrixnum, int x, int y)
		{
			int pre = 0;
			for (int i = 0; i < matrixnum; i++)
			{
				pre += (matrices[i].GetHeight() * matrices[i].GetWidth());
			
			}
			pre += y * matrices[matrixnum].GetWidth() + x;
			return original->matrices[0].content[pre];
			//std::cout << "a throwolt index: " << idx << " endindex: " << endindex << " pre: " << pre << " idx-pre= " << (idx - pre) << std::endl;
			//throw new exception("hiba a transport operator[]-ben");
			//return -1;
		}
	};
}
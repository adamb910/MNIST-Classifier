#include <iostream>
#include <exception>
#include "Matrices.h"
#include <algorithm>

#include <chrono>


using namespace std::chrono;

using namespace std;



void onlabor::Matrix::SetAt(int x, int y, float val)
{
	int index = y * (GetWidth()) + x;
	content[index] = val;
}

float onlabor::Matrix::GetAt(int x, int y)
{
	int index = y * (GetWidth()) + x;
	return content[index];
}

onlabor::Matrix onlabor::Matrix::Transpose()
{
	Matrix retVal = Matrix(GetWidth(), GetHeight());

	int size = GetHeight() * GetWidth();

	int i = 0;
	int index = 0;
	while (index != size)
	{

		retVal.content[index] = content[(index % (height)) * width + i];
		index++;

		if (index % height == 0)
			i++;
	}
	return retVal;
}

onlabor::Matrix onlabor::Matrix::Multiply(Matrix b)
{

	Matrix megoldas = Matrix(height, b.width);

		if (width == b.height)
			if (gpu)
			{ }
				//megoldas.content = GPUSzorzo::Szorzas(content, GetHeight(), GetWidth(), b.content, b.GetHeight(), b.GetWidth());
			else
				megoldas.content = CPUSzorzo::Szorzas(content, GetHeight(), GetWidth(), b.content, b.GetHeight(), b.GetWidth());
		else
			std::cout << "\nwrong size matrix for multiplication (" << height << ", " << width << ") vs (" << b.height << ", " << b.width << ");" << std::endl;
	
	//delete result;
	return megoldas;


}

onlabor::Matrix onlabor::Matrix::Subtract(Matrix b)
{
	Matrix result = Matrix(height, width);
	if (height == b.height && width == b.width)
	{
		result.content = CPUSzorzo::SubtractMtx(content, b.content, height, width);
	}
	return result;
}



onlabor::Matrix onlabor::Matrix::Add(Matrix b)
{
	/*using namespace std::chrono;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();*/
	Matrix result = Matrix(height, width);
	
	if (height == b.height && width == b.width)
	{
		for (int i = 0; i < height * width; i++)
		{
			result.content[i] = content[i] + b.content[i];
		}
		//if (!gpu)
			//LASTGOODresult.content = CPUSzorzo::AddMtx( content, b.content, height, width);
		//else
		//	result.content = CPUSzorzo::AddMtx(content, b.content, height, width);
	}
		else
		{
			std::cout << "mas meretu tombok osszeadasa: " << height << "x" << width << " and " << b.height << "x" << b.width << std::endl;
			throw "Mas meretu tombok osszeadasa";
		}
		/*high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(t2 - t1).count();
		std::cout << "\n cpu osszeadas time: " << duration << " ms" << std::endl;*/
		//Print();
		//b.Print();
	return result;

}

onlabor::Matrix onlabor::Matrix::Hadamard(Matrix b)
{
	Matrix result = Matrix(height, width);
	for (int i = 0; i < result.content.size(); i++)
	{
		result.content[i] = content[i] * b.content[i];
	}
	//result.content = CPUSzorzo::Hadamard( content, b.content, height, width);
	return result;
}

onlabor::Matrix onlabor::Matrix::Dot(Matrix b)
{

	Matrix* tall;
	Matrix* wide;

	Matrix result = Matrix(std::max(height, b.height), std::max(width, b.width));
	if (height > b.height)
	{
		tall = this; wide = &b;
	}
	else
	{
		tall = &b; wide = this;
	}

	if (/*!gpu*/true)
	{		
		std::cout << " \nCPU: " << std::endl;
		result.content = CPUSzorzo::Dot(tall->content, tall->height, tall->width, wide->content, wide->height, wide->width);
		result.Print();
		/*for (int i = 0; i < tall->height; i++)
		{
			for (int j = 0; j < wide->width; j++)
			{
				result.content[i*wide->width + j] = tall->content[i] * wide->content[j];
			}
		}*/
	}
	//else
	if (/*!gpu*/true)
	{
		//std::cout << " \nGPU: " << std::endl;
		//result.content = GPUSzorzo::Dot(tall->content, tall->height, tall->width, wide->content, wide->height, wide->width);
		//std::cout << "writing it out vec size: " << result.content.size() << " mat size: " << result.height << "x" << result.width;
		result.Print();
	}
	return result;
}

void onlabor::Matrix::Zeros()
{
	for (int i = 0; i < height*width; i++)
	{
		content[i] = 0;
	}
}

void onlabor::Matrix::Ones()
{
	for (int i = 0; i < height*width; i++)
	{
		content[i] = 1;
	}
}

void onlabor::Matrix::DivideByFloat(float n)
{
	for (int i = 0; i < height*width; i++)
	{
		content[i] = content[i] / n;
	}
}

void onlabor::Matrix::MultiplyByFloat(float n)
{
	for (int i = 0; i < height*width; i++)
	{
		content[i] = content[i] * n;
	}
}

void onlabor::Matrix::AddFloat(float n)
{
	for (int i = 0; i < height*width; i++)
	{
		content[i] = content[i] + n;
	}
}

void onlabor::Matrix::ConvertToSquare()
{
	height = sqrt(height);
	width = height;
}


int onlabor::Matrix::GetHeight()
{
	return height;
}

int onlabor::Matrix::GetWidth()
{
	return width;
}

void onlabor::Matrix::Rotate180()
{
	int hheight = height / 2;
	int hwidth = width / 2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float temp = GetAt(j,i);
			SetAt(j, i, GetAt(width - j - 1, height - i - 1));
			SetAt(width - j - 1, height - i - 1, temp);
			if (j == width / 2 && i == height / 2)
				goto out;
		}
	}
out:
	return;
}

//onlabor::Matrix onlabor::Matrix::Conv2D(Channel filter, int whichFilter, int stride = 1, int padding = 0)
//{
//	stride = 1;	// lecci ne akarj mas strideot hasznalni most thx
//
//	int filterSize = filter.weights.at(whichFilter).GetHeight();
//	
//	Matrix out = Matrix(GetHeight() - filterSize + 1, GetWidth() - filterSize + 1);
//
//	for (int y = 0; y + filterSize - 1 < GetHeight(); y+=stride)
//	{
//		for (int x = 0; x + filterSize - 1 < GetWidth(); x+=stride)
//		{
//			int sum = 0;
//			for (int kx = 0; kx < filterSize; kx++)
//			{
//				for (int ky = 0; ky < filterSize; ky++)
//				{
//					sum += filter.GetAt(kx, ky, whichFilter) * GetAt(x + kx, y + ky);
//				}
//			}
//			out.SetAt(x, y, sum);
//		}
//	}
//	return out;
//}

onlabor::Matrix onlabor::Matrix::Conv2D(Matrix other, int stride, int padding)
{
	//stride = 1;	// lecci ne akarj mas strideot hasznalni most thx
	if (other.GetWidth() == 1)
		other.ConvertToSquare();
	if (GetWidth() == 1)
		ConvertToSquare();
	int filterSize = other.GetHeight();
	//GETHEIGHT OF WHAT??
	//std::cout << "debugging minus matrix size: " << GetHeight() << " " << (filterSize + 1);
	Matrix out = Matrix(GetHeight() - filterSize + 1, GetWidth() - filterSize + 1);

	for (int y = 0; y + filterSize - 1 < GetHeight(); y += stride)
	{
		for (int x = 0; x + filterSize - 1 < GetWidth(); x += stride)
		{
			float sum = 0;
			for (int kx = 0; kx < filterSize; kx++)
			{
				for (int ky = 0; ky < filterSize; ky++)
				{
					sum += other.GetAt(kx, ky) * GetAt(x + kx, y + ky);
				}
			}
			out.SetAt(x, y, sum);
		}
	}
	return out;
}

#pragma once

#ifndef __MAT__
#define __MAT__
#include <iostream>
#include <vector>
#include "cpuszorzas.h"
//#include "gpuszorzas.h"

using namespace std;

namespace onlabor
{

	class Matrix
	{
		int height;
		int width;

		 bool gpu = false;

	public:
		vector<float> content;
		Matrix()
		{
			height = 1;
			width = 1;
			content = vector<float>(height*width);
		}

		Matrix(int h, int w)
		{
			height = h;
			width = w;

			content = vector<float>(h*w);

			for (int i = 0; i < h*w; i++)
				content[i] = 0;
		}

		Matrix(const Matrix &m2)
		{
			height = m2.height; width = m2.width;
			content = vector<float>(m2.height * m2.width);
			if (m2.content.size() == content.size())
				for (int i = 0; i < content.size(); i++)
					content[i] = m2.content[i];

		}

		Matrix(const Matrix *m2)
		{
			height = m2->height; width = m2->width;
			content = vector<float>(m2->height * m2->width);
			if (m2->content.size() == content.size())
				for (int i = 0; i < content.size(); i++)
					content[i] = m2->content[i];

		}

		void Print()
		{

			cout.precision(2);
			for (int i = 0; i < height*width; i++)
			{
				if (i % width == 0)
					std::cout << "\n";
				if (content[i] != 0)
					std::cout << content[i] << ", ";
				else
					std::cout << "0.0" << ", ";
			}
		}

		void PrintNotZero()
		{

			cout.precision(1);
			for (int i = 0; i < height * width; i++)
			{
				if (i % width == 0)
					std::cout << "\n";
				if (content[i] != 0)
					std::cout << content[i] << ", ";
				else
					std::cout << "   " << ", ";
			}
		}

		void PrintLargerThan(float no)
		{

			cout.precision(1);
			for (int i = 0; i < height * width; i++)
			{
				if (i % width == 0)
					std::cout << "\n";
				if (content[i] > no)
					std::cout << content[i] << ", ";
				else
					std::cout << "   " << ", ";
			}
		}

		void PrintShape()
		{

			
			 std::cout << "\n";
			 std::cout << "shape: (" << GetHeight() << ", " << GetWidth() << ")" << std::endl;

		}

		void SetAt(int x, int y, float val);

		float GetAt(int x, int y);

		void Shape(int h, int w)
		{
			height = h;
			width = w;
			this->Resize(h*w);
			this->Zeros();
		}

		bool AllZeros()
		{
			for (int i = 0; i < content.size(); i++)
			{
				if (content[i] == 0)
					return false;
			}
			return true;
		}

		void AddZeroPadding(int num=1)
		{
			Matrix helper = Matrix(GetHeight() + num * 2, GetWidth() + num * 2);
			for (int i = 0; i < helper.content.size(); i++)
				helper.content[i] = 0;

			for (int h = 0; h < GetHeight(); h++)
			{
				for (int w = 0; w < GetWidth(); w++)
				{
					if (w + num > helper.GetWidth() || h + num > helper.GetHeight())
						std::cout << "AddZeroPadding error with sizes" << std::endl;
					helper.SetAt(w + num, h + num, GetAt(w, h));
				}
			}
			
			content = helper.content;
			width = helper.width;
			height = helper.height;

		}

		void Resize(int size)
		{
		

			content = vector<float>(size);

		}

		void SetSize(int h, int w)
		{
			height = h;
			width = w;
		}

		Matrix Transpose();

		Matrix Multiply(Matrix b);

		Matrix Subtract(Matrix b);

		Matrix Add(Matrix b);

		Matrix Hadamard(Matrix b);

		Matrix Dot(Matrix b);

		void DivideByFloat(float n);

		void MultiplyByFloat(float n);

		void AddFloat(float n);

		void ConvertToSquare();

		void Zeros();

		void Ones();
			   
		int GetHeight();

		int GetWidth();

		void Rotate180();

		Matrix& operator+=(const Matrix& right)
		{
			Matrix ret = right;
			for (int i = 0; i < content.size(); i++)
			{
				content[i] += right.content[i];
				ret.content[i] = right.content[i];
			}
			return ret;
		}

		//Matrix Conv2D(Channel filters, int whichFilter, int stride = 1, int padding = 0);

		Matrix Conv2D(Matrix other, int stride = 1, int padding = 0);

		~Matrix()
		{
			/*
				delete[] content;*/
			//std::cout << "Destroying matrix of size: " << height * width << " where pointer null is: " << (content == nullptr) << " this pointer: " << this << std::endl;
	/*		if (content != nullptr)
				delete[] content;*/
		}
	};
}
#endif
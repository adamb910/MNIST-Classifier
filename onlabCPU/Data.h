#pragma once
#ifndef __DATA__
#define __DATA__



#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <readMNIST.h>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <random>
#include <mnist_matrix_data.h>



namespace onlabor
{

	class InputData
	{
		std::vector<mnist_matrix> data;
		mnistdata* d;// = new mnistdata[2];	// most nincs jobb megoldasom, sry
	public:

		InputData()
		{
			d = new mnistdata[2];
		}

		void Shuffle()
		{

			std::vector<int> numbers = std::vector<int>();
			std::vector<mnist_matrix> datahelper = std::vector<mnist_matrix>();
			for (int i = 0; i < data.size(); i++)
			{
				numbers.push_back(i);
			}
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(numbers.begin(), numbers.end(),g);

			for (int i = 0; i < data.size(); i++)
			{
				datahelper.push_back(data.at(numbers.at(i)));
			}

			data = datahelper;
		}

		void print()
		{
			for (int i = 0; i < data.size(); i++)
			{
				std::cout << LargestActivation(data.at(i).output) << std::endl;
			}
			getchar();
		}

		int LargestActivation(Matrix arr)
		{
			int largestIndex = -1;
			float largestVal = -1;
			for (int i = 0; i < arr.content.size(); i++)
			{
				if (arr.content[i] > largestVal)
				{
					largestVal = arr.content[i];
					largestIndex = i;
				}
			}

			return largestIndex;
		}


		void push_back(mnist_matrix pb)
		{
			data.push_back(pb);
		}

		auto begin()
		{
			return data.begin();
		}

		auto end()
		{
			return data.end();
		}

		int size()
		{
			return data.size();
		}

		auto at(int i)
		{
			return data.at(i);
		}

		void Load(const char * image_filename, const char * label_filename, int db)
		{
			unsigned cnt = 0;
			mnistreader reader = mnistreader();
			d = reader.read_mnist_cv(image_filename, label_filename, &cnt, db);

			if (d == nullptr)
				std::cout << " NULL A DATA POINTER" << std::endl;
			else
				std::cout << " DATA POINTER RENDBEN (Y) " << std::endl;


			Matrix inhelper;
			Matrix outhelper;
			for (int i = 0; i < (int)cnt; i++)
			{

				mnistdata p = d[i];
				Matrix in = ToMatrix(p, 784); Matrix out = ToMatrix(p, 10);

				mnist_matrix h = mnist_matrix(in, out);
				data.push_back(h);

			}
		}

		Matrix ToMatrix(mnistdata mnd, int size)
		{
			Matrix mtx = Matrix(size, 1);
			for (int i = 0; i < size; i++)
			{
				if (size == 784)
					mtx.content[i] = mnd.dataf[i];
				else
					mtx.content[i] = mnd.labelf[i];
			}

			return mtx;
		}
	};
}
#endif
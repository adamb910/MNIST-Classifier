#pragma once

#include "Network.h"
#include "Matrices.h"
//#include "../onlabCPU/Layer.h"
//#include "../onlabCPU/Network.h"
//#include "../onlabCPU/Matrices.h"
using namespace System;
using namespace onlabor;



namespace CLIWrapper {

	
}
BUAKNet* network;
extern "C" __declspec(dllexport) void CreateNetwork() { network = new BUAKNet("train-images.idx3-ubyte", "train-labels.idx1-ubyte", "t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte"); }
extern "C" __declspec(dllexport) int Forward(float input[784])
{
	Matrix input_matrix = Matrix(28, 28);
	for (int i = 0; i < 784; i++)
	{
		input_matrix.content[i] = input[i];
	}
	return network->Forward(input_matrix);
}


extern "C" __declspec(dllexport) int ForwardWithNormalization(float input[400])
{
	Matrix input_matrix = Matrix(20, 20);
	for (int i = 0; i < 400; i++)
	{
		input_matrix.content[i] = input[i];
	}
	input_matrix.AddZeroPadding(4);
	return network->Forward(input_matrix);
}

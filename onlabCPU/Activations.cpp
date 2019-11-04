#include "Activations.h"
#include <math.h>
#include <vector>

using namespace onlabor;

float onlabor::Sigmoid::Activate(float z)
{
	//     1  
	// -----------
	// 1 + exp(-z)
	return 1 / (1 + exp(-z));
}


float onlabor::Sigmoid::Prime(float z)
{
	return Sigmoid::Activate(z) * (1 - Sigmoid::Activate(z));
}

std::vector<float> onlabor::Sigmoid::Activate(std::vector<float> z)
{
	std::vector<float> res = std::vector<float>(z.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = Sigmoid::Activate(z[i]);
	}

	return res;
}


Transport onlabor::Sigmoid::Activate(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++) 
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = Sigmoid::Activate(z.matrices[t].content[i]);
		}
	}
	return z;
}

Transport onlabor::Sigmoid::Prime(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++)
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = Sigmoid::Prime(z.matrices[t].content[i]);
		}
	}
	return z;
}

std::vector<float> onlabor::Sigmoid::Prime(std::vector<float> z)
{
	std::vector<float> res = std::vector<float>(z.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = Sigmoid::Prime(z[i]);
	}

	return res;
}


float onlabor::ReLu::Activate(float z)
{
	return fmax(0,z);
}


float onlabor::ReLu::Prime(float z)
{
	return (z > 0 ? 1 : 0);
}

std::vector<float> onlabor::ReLu::Activate(std::vector<float> z)
{
	std::vector<float> res = std::vector<float>(z.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = ReLu::Activate(z[i]);
	}

	return res;
}

std::vector<float> onlabor::ReLu::Prime(std::vector<float> z)
{
	std::vector<float> res = std::vector<float>(z.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = ReLu::Prime(z[i]);
	}

	return res;
}

Transport onlabor::ReLu::Activate(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++)
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = ReLu::Activate(z.matrices[t].content[i]);
		}
	}
	return z;
}

Transport onlabor::ReLu::Prime(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++)
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = ReLu::Prime(z.matrices[t].content[i]);
		}
	}

	return z;
}

Transport onlabor::Softmax::Activate(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++)
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = Softmax::Activate(z.matrices[t].content[i]);
		}
	}

	return z;
}

Transport onlabor::Softmax::Prime(Transport z)
{
	for (int t = 0; t < z.matrices.size(); t++)
	{
		for (int i = 0; i < z.matrices[t].content.size(); i++)
		{
			z.matrices[t].content[i] = Softmax::Prime(z.matrices[t].content[i]);
		}
	}

	return z;
}


std::vector<float> onlabor::Softmax::Activate(std::vector<float> z)
{
	std::vector<float> out = std::vector<float>(z.size());
	float sum = 0;
	for (int i = 0; i < z.size(); i++)
	{
		sum += exp(z[i]);
	}

	for (int i = 0; i < z.size(); i++)
	{
		out[i] = exp(z[i])/sum;
	}
	return out;
}

std::vector<float> onlabor::Softmax::Prime(std::vector<float> z)
{
	std::vector<float> act = Softmax::Activate(z);
	std::vector<float> out = std::vector<float>(z.size());
	for (int i = 0; i < z.size(); i++)
	{
		for (int j = 0; j < z.size(); j++)
		{
			if (i == j)
			{
				out[i] = act[i] * (1 - act[i]);
			}
			else
			{
				out[i] = -act[j] * act[i];
			}
		}
	}
	return std::vector<float>();
}

float onlabor::Softmax::Activate(float z)
{
	return 0.0f;
}

float onlabor::Softmax::Prime(float z)
{
	return 0.0f;
}

float onlabor::NoActivation::Activate(float z)
{
	return z;
}

std::vector<float> onlabor::NoActivation::Activate(std::vector<float> z)
{
	return z;
}

Transport onlabor::NoActivation::Activate(Transport z)
{
	return z;
}

float onlabor::NoActivation::Prime(float z)
{
	return z;
}

std::vector<float> onlabor::NoActivation::Prime(std::vector<float> z)
{
	return z;
}

Transport onlabor::NoActivation::Prime(Transport z)
{
	return z;
}

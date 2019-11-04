#pragma once

#include <Matrices.h>
#include <Layer.h>
#include <Activations.h>
#include <Transport.h>
#include <random>
#include <ProxyTransport.h>

namespace onlabor
{
	class Channel
	{
	public:
		std::vector<Matrix> weights = std::vector<Matrix>();
		float bias = 0;
		int inDepth = 0;
		Matrix out = Matrix();

		Channel()
		{

		}

		Channel(int h, int w, int inDepth_) : inDepth(inDepth_)
		{
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0, 1);
			for (int i = 0; i < inDepth; i++)
			{
				weights.push_back(Matrix(h, w));
				
			}

			InitWeights();

		}

		Channel(int h, int w, int inDepth_, int defaultvalue) : inDepth(inDepth_)
		{
			
			for (int i = 0; i < inDepth; i++)
			{
				weights.push_back(Matrix(h, w));
				for (int j = 0; j < h * w; j++)
				{

					weights.at(i).content[j] = defaultvalue;
				}
			
			}
			bias = 0;

		}

		void InitWeights()
		{
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0, 1);
			generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
			for (int i = 0; i < inDepth; i++)
			{
				for (int j = 0; j < weights[i].content.size(); j++)
				{
					weights[i].content[j] = abs(distribution(generator));

				}
			
			}
			bias = abs(distribution(generator));

		}

		float GetAt(int x, int y, int i)
		{
			return weights[i].GetAt(x, y);
		}

		void SetAt(int x, int y, int i, int val)
		{
			weights[i].SetAt(x, y, val);
		}
	};


	//class Kernel
	//{
	//public:
	//	Matrix weights;
	//	float bias;
	//	Kernel(int h, int w)
	//	{
	//		weights = Matrix(h, w);
	//	}

	//	float GetAt(int x, int y)
	//	{
	//		return weights.GetAt(x, y);
	//	}

	//	void SetAt(int x, int y, int val)
	//	{
	//		weights.SetAt(x, y, val);
	//	}
	//};

	class ConvLayer : public Layer
	{

		int kernelH;
		int kernelW;
		int inchannels;
		int outchannels;
		//this vector contains the ouput channels, the one in the Channel class contains the input channel weights
		std::vector<Channel> channels = std::vector<Channel>();
		std::vector<Channel> n_delta_w = std::vector<Channel>();
		std::vector<float> n_delta_b = std::vector<float>();
		Transport acts = Transport();
		Transport inputs = Transport();
		Transport prevActivations = Transport();
	public:
		Transport z = Transport();
		Activation* activation;

		ConvLayer(Activation* act, int inchannels_ = 1, int outchannels_ = 1, int kH = 5, int kW = 5, int stride = 1) : inchannels(inchannels_), outchannels(outchannels_)
		{
			activation = act;
			prev = nullptr;
			next = nullptr;
			kernelH = kH; kernelW = kW;
			backpropCount = 0;

			n_delta_b = std::vector<float>(outchannels);
			for (int i = 0; i < outchannels; i++)
			{
				//TESTING channels.push_back(Channel(kernelH, kernelW, inchannels));
				channels.push_back(Channel(kernelH, kernelW, inchannels));
				n_delta_w.push_back(Channel(kernelH, kernelW, inchannels,0));

				acts.matrices.push_back(Matrix(15 + 1, 5 + 1));
				//z.matrices.push_back(Matrix(15 + 1, 5 - kernelW + 1));
				z.matrices.push_back(Matrix(1, 1));
			}
		}

		int LargestActivation(std::vector<float> arr);

		// Inherited via Layer
		virtual void Backprop(Transport deltas) override;

		virtual void InitWeights() override;

		virtual void UpdateParameters(float learningRate) override;

		virtual int Feedforward(Transport activations) override;

		virtual void ConnectTo(Layer * l) override;

		// Inherited via Layer
		virtual void Setup(int inp) override;

		virtual Transport GetZs() override;

		void PrintWeights()
		{
			for (int i = 0; i < channels.size(); i++)
			{
				for (int o = 0; o < channels[i].weights.size(); o++)
				{
					
					channels[i].weights[o].Print();
				}
			}
		}

		// Inherited via Layer
		virtual Activation* GetActivation() override;
		


		// Inherited via Layer
		virtual void Load(string path, int layerIndex) override;

		virtual void Save(string path, int layerIndex) override;

	};
}
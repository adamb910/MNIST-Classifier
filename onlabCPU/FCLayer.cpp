#include <iostream>
#include "FCLayer.h"
#include "Activations.h"
#include "cpuszorzas.h"
#include "gpuszorzas.h"
#include "MatrixOp.h"
#include <random>
#include <chrono>
#include <sstream>
#include <fstream>


using namespace std::chrono;



namespace onlabor
{
	FCLayer::FCLayer(int neur, Activation* act) 
	{
		activation = act;
		next = NULL;
		prev = NULL;
		inputs = 0;
		neurons = neur;
		setup = false;
	}

	FCLayer::FCLayer(int inp, int neur, Activation* act) 
	{
		activation = act;
		next = NULL;
		prev = NULL;

		inputs = inp; neurons = neur;
		Setup(inp);
	}

	void FCLayer::Setup(int inp)
	{
		/*std::cout << "FCLayer with neurons: " << neurons << " has " << inp << " number of inputs." << std::endl;
		getchar();
*/
		
		setup = true;
		inputs = inp;
		weights = Matrix(neurons, inputs);//new float[neurons*inputs];
		bias = Matrix(neurons, 1);// new float[neurons];
		deltas = Matrix(neurons, 1);//new float[neurons];
		zs = Matrix(neurons, 1);//new float[neurons];
		zs_prime = Matrix(neurons, 1);//new float[neurons];
		out = Matrix(neurons, 1);

		//initializing backprop variables
		nabla_delta_w = Matrix(neurons, inputs);
		delta_w = Matrix(neurons, inputs);

		nabla_delta_b = Matrix(neurons, 1);
		delta_b = Matrix(neurons, 1);

		backpropCount = 0;

		InitWeights();
	}

	Transport FCLayer::GetZs()
	{
		Transport zsback= Transport(1);
		zsback.matrices[0] = zs;
		return zsback;
	}

	Activation* FCLayer::GetActivation()
	{
		return activation;
	}

	void FCLayer::Load(string path, int layerIndex)
	{
		try
		{
			stringstream fullPath;
			//fullPath << path;
			fullPath << layerIndex;
			fullPath << ".txt";
			path = fullPath.str();
			std::ifstream infile(path, std::ifstream::binary);
			std::string line;
			int i = 0;
			bool inputSwitch = false;
			bool weightsSwitch = false;
			if (infile)
			{
				while (std::getline(infile, line))
				{
					std::istringstream iss(line);
					float n;
					while (iss >> n)
					{

						if (!inputSwitch)
						{
							inputs = (int)n;
							Setup(inputs);
							inputSwitch = true;
						}
						else
						{
							if (!weightsSwitch)
							{
								weights.content[i] = n;
								if (i == weights.content.size() - 1)
								{
									weightsSwitch = true;
									i = 0;
								}
							}
							else if (i < bias.content.size())
							{
								bias.content[i] = n;
							}
							i++;
						}
					}

				}
				setup = true;	// so weights don't get randomly initialized for learning..
				std::cout << "\nDone loading Fully Connected Layer " << layerIndex << "...\n";
				
			}
			else
			{

				std::cout << "\nFAILED loading Fully Connected Layer " << layerIndex << "...\n";
			}
		
		}
		catch (exception e)
		{
			std::cout << "\nLoading has failed..\n";
		}
	}


	void FCLayer::Save(string path, int layerIndex)
	{
		stringstream fullPath;
		//fullPath << path;
		fullPath << layerIndex;
		fullPath << ".txt";
		path = fullPath.str();
		//std::ifstream infile(path, std::ifstream::binary);
		std::ofstream outfile(path, std::ofstream::binary);
		if (outfile)
		{
			outfile << inputs << " ";
			for (int i = 0; i < weights.content.size(); i++)
				outfile << weights.content[i] << " ";

			for (int i = 0; i < bias.content.size(); i++)
				outfile << bias.content[i] << " ";
			outfile.close();
			std::cout << "\nSaved Fully Connected Layer " << (layerIndex + 1) << "\n";
		}
		else
		{
			std::cout << "\nFAILED! Fully Connected Layer " << (layerIndex + 1) << "\n";
		}

		
	}

	void FCLayer::InitWeights()
	{
		if (prev != nullptr)
		{

			// weight initialization, gaussian distr, with deviation of 1/sqrt(inputs)
			std::default_random_engine generator;
			std::normal_distribution<double> distribution(0, 1 / sqrt((float)inputs));
			std::default_random_engine generator2;
			std::normal_distribution<double> distribution2(0, 1 / sqrt((float)neurons));
			for (int i = 0; i < neurons*inputs; i++)
			{
				weights.content[i] = distribution(generator);////1;//
			}
			//TESTING CHANGED
			//bias initialization
			for (int i = 0; i < neurons; i++)
			{
				bias.content[i] = distribution2(generator2);//1;
			}
		}
		else
		{
			for (int i = 0; i < neurons*inputs; i++)
			{
				weights.content[i] = 1;
			}
			for (int i = 0; i < neurons; i++)
			{
				bias.content[i] = 0;
			}
		}
	}

	int FCLayer::Feedforward(Transport act)
	{
	
		//new Transport class may contain any form and number of matrices, so a transperant approach was added, which follows:
		int actSize = act.Count();
		if (!setup)
		{
			Setup(actSize);
		}

		prevActivations = Matrix(actSize, 1);
		Matrix curAct = Matrix(actSize, 1);
		for (int i = 0; i < actSize; i++)
		{
			int helper = act[i];
			prevActivations.content[i] = act[i];
			curAct.content[i] = prevActivations.content[i];

		}
		//std::cout << "\nFC Weights\n";
		//weights.Print();
		// calculate activation of each neuron	
		if (prev != nullptr)
		{
			out = weights.Multiply(curAct);
			zs = out.Add(bias);

			//std::cout << "\nout.print\n";
			//zs.Print();
			zs_prime.content = activation->Prime(zs.content);//Activation::sigmoid_prime(zs.content);

			/*std::cout << "out size " << out.content.size() << std::endl;
			getchar();*/
			//give the outputs to an activation
			out.content = activation->Activate(zs.content);

		}
		else
			out = curAct;


		Transport outT = Transport(); outT.matrices.push_back(out);

		//pass output to next !! GENERAL !! layer, from Layer.h
		if (next != nullptr)
		{
			return next->Feedforward(outT);
		}
		else
		{
			if (debug)
			{
				std::cout << "\nNetwork output: \n";
				//out.Print();
				float max = *max_element(out.content.begin(), out.content.end());
				std::cout << "Largest output activation: " << max << std::endl;
				for (int i = 0; i < out.content.size(); i++)
				{
					std::cout << std::endl;
					if (out.content[i] > 0)
					{
						std::cout << i << "\t";
						for (int j = 0; j < (out.content[i] / max) * 10; j++)
						{
							std::cout << "|";
						}
					}
					else
					{
						for (int j = 0; j < 10; j++)
						{
							if (j > 10 - out.content[i] * 10)
								std::cout << "|";
							else
								std::cout << " ";
						}
					}
				}
				std::cout << std::endl;
				std::cout << "\n Largest activation " << LargestActivation(out.content) << std::endl;
			}
			return LargestActivation(out.content);
		}
	}

	void FCLayer::Backprop(Transport passedDeltas)
	{
		
		std::cout << typeid(this).name() << std::endl;
		std::cout << "neurons: " << neurons << std::endl;
		/*for (int i = 0; i < passedDeltas.matrices.size(); i++)
		{
			std::cout << "\nFC:\n ";
			passedDeltas.matrices[i].Print();
		}*/

		deltas = passedDeltas.matrices.at(0);
		if (prev != nullptr)
		{
			//TESTING
			if (next == nullptr)
			{
				//REMOVE FOR TESTING
				deltas = out.Subtract(passedDeltas.matrices[0]);

				//IF QUADRATIC COST:
				//deltas = deltas.Hadamard(zs_prime);

			}
			

		
			// BP3, setting Bias of backprop
			nabla_delta_b = nabla_delta_b.Add(deltas);

			// BP4, setting weights of backprop
			Matrix prevActHelp = prevActivations.Transpose();

			delta_w = deltas.Multiply(prevActHelp);

			nabla_delta_w = nabla_delta_w.Add(delta_w); //TODO: CREATE ADD IN PLACE TO AVOID COPY
			Transport deltasT = Transport(); Transport weightsT = Transport();
			deltasT.matrices.push_back(deltas); weightsT.matrices.push_back(weights);

			backpropCount++;

			// calculate those goddamn deltas
			//bp2
			Matrix nweightTrans = weights.Transpose();
			
			//Matrix nweightTrans = weights.matrices[0].Transpose();
			deltas = nweightTrans.Multiply(deltas); 
		/*	std::cout << "deltas\n";
			deltas.Print();*/
			//get zs of prev layer
			Transport prevZs = prev->GetZs();
			
			if (prevZs.matrices.size() > 0)
			{
				std::cout << "prevzs.matrices.size()>0\n\n\n\n";
				//Make it prime
				prevZs = prev->GetActivation()->Prime(prevZs);
				//since this is a feedforward layer, we only need the matrix at index 0
				deltas = deltas.Hadamard(prevZs.matrices[0]);
			}


			


			passedDeltas.matrices[0] = deltas;

			//pass em back
			//std::cout << "fcdeltas:::::" << std::endl;
			//passedDeltas.matrices[0].Print();
			prev->Backprop(passedDeltas);

		}

	}

	/*
		Update all parameters after the mini batch is done...
	*/
	void FCLayer::UpdateParameters(const float learningRate)
	{
		if (prev != nullptr)
		{
			std::cout << "MINI BATCH UPDATE " << backpropCount << std::endl;
			if (backpropCount == 0)
				return;
			nabla_delta_w.DivideByFloat(backpropCount);
			nabla_delta_b.DivideByFloat(backpropCount);

			nabla_delta_w.MultiplyByFloat(learningRate);
			nabla_delta_b.MultiplyByFloat(learningRate);

			weights = weights.Subtract(nabla_delta_w);
			bias = bias.Subtract(nabla_delta_b);

			// empty all backprop related variables:
			nabla_delta_w.Zeros();
			delta_w.Zeros();

			nabla_delta_b.Zeros();
			delta_b.Zeros();

			backpropCount = 0;
		}
		if (next != nullptr)
			next->UpdateParameters(learningRate);
	}


	int FCLayer::LargestActivation(std::vector<float> arr)
	{
		int largestIndex = -1;
		float largestVal = -std::numeric_limits<float>::infinity();
		//std::cout << "Largest Act\n";
		for (int i = 0; i < arr.size(); i++)
		{
			//std::cout << arr[i] << "\n";
			if (arr[i] > largestVal)
			{
				largestVal = arr[i];
				largestIndex = i;
			}
		}

		return largestIndex;
	}


	void onlabor::FCLayer::ConnectTo(Layer * l)
	{
		//wire the layers up..
		next = l;
		l->prev = (Layer*)this;

	}

	// (nvm) free allocated weights and bias.
	FCLayer::~FCLayer()
	{

	}

}
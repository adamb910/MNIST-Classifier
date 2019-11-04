#include "ConvLayer.h"
#include <Activations.h>
#include <sstream>
#include <fstream>



int onlabor::ConvLayer::Feedforward(Transport activations)
{

	inputs = activations;

	//std::cout << "conv input\n";
	//for (int i = 0; i < activations.matrices.size(); i++)
	//{
	//	activations.matrices[i].Print();
	//	std::cout << "\n";
	//}
	//std::cout << "conv input done\n";


	// if input is a vector, convert it to a square matrix...
	if (activations.matrices.at(0).GetWidth() == 1)
	{
		activations.matrices.at(0).ConvertToSquare();
		//std::cout << "-----------------------------" << std::endl;
	}
	acts = Transport(outchannels);
	prevActivations = activations;

	//Ciklus, a kimeneti csatorna szerint
	for (int o = 0; o < channels.size(); o++)
	{
		Matrix result = Matrix(activations.matrices.at(0).GetHeight() - channels.at(0).weights.at(0).GetWidth() + 1, activations.matrices.at(0).GetWidth() - channels.at(0).weights.at(0).GetWidth() + 1);
		//minden bemeneti csatornan, activations.matrices.size() e helyett lehetne channels[o].size()-ig is.
		for (int i = 0; i < activations.matrices.size(); i++)
		{
			
			auto conv = activations.matrices.at(i).Conv2D(channels.at(o).weights.at(i));
		/*	std::cout << "conv" << std::endl;
			activations.matrices.at(i).Print();
			channels.at(o).weights.at(i).Print();
			std::cout << std::endl;*/

			result = result.Add(conv);
		}
		//bias hozzaadasa..
		//result.AddFloat(channels.at(o).bias);
		z.matrices.at(o) = result;
		
		//Aktivacio...
		acts.matrices.at(o) = z.matrices.at(o);
		
		for (int j = 0; j < z.matrices.at(o).content.size(); j++)
		{
			//REMOVE FOR TESTING
			acts.matrices.at(o).content[j] += channels[o].bias;
		}
	}
	acts = activation->Activate(acts);//TESTING this was commented out so far???
	/*std::cout << "before activation";
	acts.Print();
	std::cout << "after activation";
	acts.Print();
	*/

	//feed the next layer our channels
	if (next != nullptr)
	{
		/*std::cout << "conv output\n";
		for (int i = 0; i < acts.matrices.size(); i++)
		{
			acts.matrices[0].Print();
			std::cout << "\n";
		}*/
		
	
		return next->Feedforward(acts);
	}
	return -1;
}


onlabor::Transport onlabor::ConvLayer::GetZs()
{
	return z;
}

onlabor::Activation* onlabor::ConvLayer::GetActivation()
{
	return activation;
}

void onlabor::ConvLayer::Load(string path, int layerIndex)
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
		bool weightsSwitch = false;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				std::istringstream iss(line);
				int n;
			
				for (int i = 0; i < channels.size(); i++)
				{
					for (int j = 0; j < channels[i].weights.size(); j++)
					{
						for (int k = 0; k < channels[i].weights[j].content.size(); k++)
						{
							iss >> channels[i].weights[j].content[k];
						}
					}
				}


				for (int i = 0; i < channels.size(); i++)
				{
					iss >> channels[i].bias;
				}

			}
		}

		std::cout << "\nDone loading Convolutional Layer " << layerIndex << "...\n";
	}
	catch (exception e)
	{
		std::cout << "\nLoading has failed..\n";
	}
}

void onlabor::ConvLayer::Save(string path, int layerIndex)
{
	stringstream fullPath;
	//fullPath << path;
	fullPath << layerIndex;
	fullPath << ".txt";
	path = fullPath.str();
	std::cout << "file path: " << path;
	std::ofstream outfile(path, std::ofstream::binary);


	for (int i = 0; i < channels.size(); i++)
	{
		for (int j = 0; j < channels[i].weights.size(); j++)
		{
			for (int k = 0; k < channels[i].weights[j].content.size(); k++)
			{
				outfile << channels[i].weights[j].content[k] << " ";
			}
		}
	}


	for (int i = 0; i < channels.size(); i++)
	{
			outfile << channels[i].bias << " ";
	}

	outfile.close();

	std::cout << "\nSaved Convolutional Layer " << (layerIndex + 1) << "\n";
}

void onlabor::ConvLayer::Backprop(Transport deltas)
{
	/*std::cout << "\n" << typeid(this).name() << std::endl;
	std::cout << " conv recieved deltas" << std::endl;
	deltas.Print();*/
	

	Transport primes = z;

	//update bias:
	for (int i = 0; i < channels.size(); i++)
	{
		float sum = 0;
		for (int m = 0; m < deltas.matrices[i].content.size(); m++)
		{
			sum += deltas.matrices[i].content[m];
		}
		n_delta_b[i] = sum;
	}


	auto dummyInputs = inputs;
	for (int i = 0; i < dummyInputs.matrices.size(); i++)
		dummyInputs.matrices[i].Rotate180();

	//adjusting weights of current layer
	for (int o = 0; o < channels.size(); o++)
	{
		for (int i = 0; i < channels[o].weights.size(); i++)
		{
			//Matrix conv = prevZ.matrices.at(i).Conv2D(deltas.matrices.at(o));
			//std::cout << "\nprevz\n";
			//inputs.matrices.at(i).Print();
			//std::cout << "\nminibatch_delta[b,i]\n";
			//deltas.matrices.at(o).Print();
			Matrix conv = dummyInputs.matrices.at(i).Conv2D(deltas.matrices.at(o));
			/*std::cout << "\ncurrent delta nabla w \n";
			conv.Print();*/
			n_delta_w[o].weights[i] += conv;
			//std::cout << "delta_nabla_w\n";
			//n_delta_w[o].weights[i].Print();
			
		}
	}

	std::vector<Channel> rotatedChannel = channels;
	for (int o = 0; o < rotatedChannel.size(); o++)
	{
		for (int i = 0; i < rotatedChannel[o].weights.size(); i++)
			rotatedChannel[o].weights.at(i).Rotate180();
	}
	Transport backwardDelta = prevActivations;


	//generating deltas, to be passed backwards

	Transport prevZ;
	if (prev != nullptr)
	{
		prevZ = prev->GetZs();
	}
	else
	{
		prevZ = prevActivations;
	}

	prevZ = activation->Prime(prevZ);

	for (int o = 0; o < channels.size(); o++)
	{
		for (int i = 0; i < channels[o].weights.size(); i++)
		{

			//6x6-bol 10x10-et akarunk, akkor a padding;
			//10 = 14 - 5 + 1
			//elvart meret (14) - jelenlegi delta.height() / 2

	
			int x = prevZ.matrices.at(i).GetHeight() + kernelH - 1;
			int kul = x - deltas.matrices.at(o).GetHeight();

			deltas.matrices.at(o).AddZeroPadding(kul/2);
			//deltas.matrices.at(o).PrintShape();
			
			//rotatedChannel[o].weights[i].Print();

			backwardDelta.matrices.at(i) = deltas.matrices.at(o).Conv2D(rotatedChannel[o].weights.at(i));
		
			backwardDelta.matrices.at(i) = backwardDelta.matrices.at(i).Hadamard(prevZ.matrices.at(i));
			backwardDelta.matrices.at(i).DivideByFloat(10);
			//std::cout << "\n\n\n\n\n\nprevlayer_delta\n";
			//backwardDelta.matrices.at(i).Print();

			
			
		}
	}




	backpropCount++;
	if (prev != nullptr)
	{
		prev->Backprop(backwardDelta);
	}

}

void onlabor::ConvLayer::InitWeights()
{
}

void onlabor::ConvLayer::UpdateParameters(float learningRate)
{
	if (backpropCount == 0)
		return;


	/*std::cout << "weights before:\n";
	for (int o = 0; o < channels.size(); o++)
	{
		for (int i = 0; i < channels[o].weights.size(); i++)
		{
			channels[o].weights[i].Print();
			std::cout << "\n";

		}
	}*/
	for (int o = 0; o < channels.size(); o++)
	{
		//BIAS FOR CONVOLUTION
		for (int i = 0; i < n_delta_b.size(); i++)
		{
			n_delta_b[o] *= learningRate;
			n_delta_b[o] /= backpropCount;
		}
		channels[o].bias -= n_delta_b[o];



		for (int i = 0; i < channels[o].weights.size(); i++)
		{
		/*	std::cout << "\ndelta nabla w\n";
			n_delta_w[o].weights[i].Print();

			std::cout << "\nlr " << learningRate << " minbatchsize " << backpropCount << "\n";*/
			n_delta_w[o].weights[i].MultiplyByFloat(learningRate);// / backpropCount);
			n_delta_w[o].weights[i].DivideByFloat(backpropCount);

			/*std::cout << "\nscaled delta nabla w\n";
			n_delta_w[o].weights[i].Print();*/

			channels[o].weights[i] = channels[o].weights[i].Subtract(n_delta_w[o].weights[i]);

		
			//zero out
			n_delta_w[o].weights[i].Zeros();

			

		}
	}
	//getchar();

	/*std::cout << "\nweights after:\n";
	for (int o = 0; o < channels.size(); o++)
	{
		for (int i = 0; i < channels[o].weights.size(); i++)
		{
			channels[o].weights[i].Print();
			std::cout << "\n";

		}
	}*/

	backpropCount = 0;
	if (next != nullptr)
		next->UpdateParameters(learningRate);

}

void onlabor::ConvLayer::ConnectTo(Layer * l)
{
	next = l;
	l->prev = (Layer*)this;

}


void onlabor::ConvLayer::Setup(int inp)
{
}


int onlabor::ConvLayer::LargestActivation(std::vector<float> arr)
{
	int largestIndex = -1;
	float largestVal = -1;
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i] > largestVal)
		{
			largestVal = arr[i];
			largestIndex = i;
		}
	}

	return largestIndex;
}
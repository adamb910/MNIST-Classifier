#pragma once
#ifndef __NETW__
#define __NETW__


#include <Layer.h>
#include <FCLayer.h>
#include <IntermediateLayer.h>
#include <Data.h>
#include <readMNIST.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <mnist_matrix_data.h>
#include <chrono>
#include <ConvLayer.h>


using namespace std::chrono;



namespace onlabor
{
	class BUAKNet
	{
		std::vector<Layer*> layers;
		InputData trainingdata;
		InputData evaldata;

		const char* training_image_filename;
		const char* training_label_filename;
		const char* test_image_filename;
		const char* test_label_filename;

		//.3
		float learningRate = .1; //1 //.6 //.05
		int miniBatchSize = 3; //5
		int epochNum = 3;
		int mennyit = 59999;//1000
		int correct = 0;

	public:
		BUAKNet(const char *image_filename,
			const char *label_filename, const char* test_image_filename_,
			const char* test_label_filename_)
		{
			training_image_filename = image_filename;
			training_label_filename = label_filename;
			test_image_filename = test_image_filename_;
			test_label_filename = test_label_filename_;

			layers = std::vector<Layer*>();
			

			


#define conv 1




#if conv == 2
			learningRate = .0075;
			miniBatchSize = 10;
			//AddLayer(new ConvLayer((Activation&)Sigmoid(), 1, 10/*out*/, 5, 5, 1));
			//AddLayer(new ConvLayer((Activation&)Sigmoid(), 10, 20/*out*/, 3, 3, 1));
			AddLayer(new ConvLayer((Activation&)ReLu(), 1, 5/*out*/, 5, 5, 1));
			AddLayer(new ConvLayer((Activation&)ReLu(), 5, 15/*out*/, 3, 3, 1));
			AddLayer(new IntermediateLayer());
			AddLayer(new FCLayer(100, (Activation&)Sigmoid()));
			AddLayer(new FCLayer(10, (Activation&)Softmax()));
#elif conv == 0
			//learningRate = 0.03;
			std::cout << "using FC only network" << std::endl;
			learningRate = 0.01;
			miniBatchSize = 10;
			AddLayer(new FCLayer(784, 784, (Activation&)ReLu()));
			AddLayer(new FCLayer(100, (Activation&)ReLu()));
			AddLayer(new FCLayer(10, (Activation&)Softmax()));
#else conv == 1
			learningRate = .01;
			miniBatchSize = 10;
			
			AddLayer(new ConvLayer(new ReLu(), 1, 5/*out*/, 5, 5, 1));
			AddLayer(new IntermediateLayer());
			AddLayer(new FCLayer(100, new Sigmoid()));
			AddLayer(new FCLayer(10, new Softmax()));
			//AddLayer(new ConvLayer((Activation*)new ReLu(), 1, 5/*out*/, 5, 5, 1));
			//AddLayer(new IntermediateLayer());
			//AddLayer(new FCLayer(100, (Activation&)ReLu()));
			//AddLayer(new FCLayer(10, (Activation&)Softmax()));
#endif // conv

			// CONNECT LAYERS
			for (int i = 0; i < layers.size(); i++)
			{
				if (i + 1 < layers.size())
				{
					layers[i]->ConnectTo(layers[i + 1]);
					std::cout << i << " connected to " << (i + 1) << std::endl;
				}
			}

			Load();
			// RUN TRAINING, THEN EVALUATION
			//Train(miniBatchSize, epochNum);
			//Evauluate();
			//Test();
		}

		


		void AddLayer(Layer* l)
		{
			layers.push_back(l);
		}

		void Save()
		{
			for (int i = 0; i < layers.size(); i++)
				layers[i]->Save("\\buaknet\\", i);
		}

		void Load()
		{
			for (int i = 0; i < layers.size(); i++)
				layers[i]->Load("\\buaknet\\", i);

		}


		int Forward(Matrix input)
		{
			Transport input_t = Transport(1);
			input_t.matrices[0] = input;
			return layers.at(0)->Feedforward(input_t);
		}

		void Train(int minibatchsize, int epochs = 1)
		{
			std::cout << "\nTraining beginning, length: " << epochs << " epochs\n";
			//LOAD TRAINING DATA
			trainingdata = InputData();
			trainingdata.Load(training_image_filename, training_label_filename, mennyit);

			auto duration = 0;
			int all = 0;
			for (int i = 0; i < epochs; i++)
			{
				int epochCor = 0;

				//SHUFFLE TRAINING DATA:
				trainingdata.Shuffle();

				for (int j = 0; j < trainingdata.size(); j++)
				{
					high_resolution_clock::time_point t1 = high_resolution_clock::now();
					std::cout << "\n Epoch: " << i << std::endl;
					if (j % minibatchsize == 0 && j != 0)
						layers.at(0)->UpdateParameters(learningRate);

					//system("CLS");
					std::cout << "__________________________________________________________" << std::endl;

					Transport trainingfT = Transport(); trainingfT.matrices.push_back(trainingdata.at(j).input);
					int result = layers.at(0)->Feedforward(trainingfT); /////////////////// FEEDFORWARD
					std::cout << "Epoch: " << i << " Training data label: " << trainingdata.LargestActivation(trainingdata.at(j).output) << " correct: " << epochCor << "/" << j + 1 << std::endl;

					std::cout << "RESULT OF FORWARD: " << result << std::endl;

					if (trainingdata.at(j).output.content[result] == 1)
					{

						std::cout << "\nNetwork prediction: " << result << " CORRECT!!!!!!!!!!" << std::endl;;
						correct++;
						epochCor++;
					}
					all++;

					Transport trainingbT = Transport(); trainingbT.matrices.push_back(trainingdata.at(j).output);
					layers.at(layers.size() - 1)->Backprop(trainingbT);		///// BACKPROP

					high_resolution_clock::time_point t2 = high_resolution_clock::now();
					auto duration = duration_cast<milliseconds>(t2 - t1).count();
					std::cout << "\n forward + backprop time: " << duration << " ms" << std::endl;
					auto durationmins = duration_cast<minutes>(t2 - t1).count();
					durationmins *= trainingdata.size() * (epochs - i);
					std::cout << "\n-------------------------------------------------------\n";
					std::cout << "           Estimated remaining training time:          \n";
					std::cout << "                   " << durationmins <<"                         \n";
 					std::cout << "-------------------------------------------------------\n";
					//getchar();

				}
			}

			std::cout << "CORRECT: " << correct << " of " << trainingdata.size() * epochs << " which is: " << ((float)correct / (float)all * 100) << "%" << std::endl;
			Save();
		}


		void Evauluate()
		{
			for (int i = 0; i < layers.size(); i++)
			{
				layers.at(i)->debug = false;
			}

			system("CLS");
			std::cout << "\nWorking on evaluation... Please wait....\n";
			int all = 0;
			int corr = 0;
			//SHUFFLE TRAINING DATA:
			//std::random_shuffle(trainingdata.begin(), trainingdata.end());
			trainingdata.Shuffle();

			for (int j = 0; j < trainingdata.size(); j++)
			{
				Transport trainingfT = Transport(); trainingfT.matrices.push_back(trainingdata.at(j).input);
				int result = layers.at(0)->Feedforward(trainingfT); /////////////////// FEEDFORWARD

				//std::cout << "Evaluation: Training data label: " << /*(int)d[j].label*/ trainingdata.LargestActivation(trainingdata.at(j).output) << std::endl;

				if (trainingdata.at(j).output.content[result] == 1)
				{

					//std::cout << "\n HELYES!!!!!!!!!!" << std::endl;;
					corr++;
					//getchar();
				}
				all++;

				//std::cout << "RESULT OF FORWARD: " << result << std::endl;
			}
			std::cout << "EVALUATION ON TRAINING DATA: CORRECT: " << corr << " of " << (trainingdata.size()) << " which is: " << ((float)corr / (float)all * 100) << "%" << std::endl;
			
				
		}

		void Test()
		{
			//LOAD EVALUATION DATA
			evaldata = InputData();
			evaldata.Load(test_image_filename, test_label_filename, 100);
			for (int i = 0; i < layers.size(); i++)
			{
				//layers.at(i)->debug = false;
			}
			//std::cout << "\n\n\n\n\n REAL TESTING ON UNSEEN DATA: \n\n\n\n\n";
			int all = 0;
			int corr = 0;
			//SHUFFLE TRAINING DATA:
			evaldata.Shuffle();

			for (int j = 0; j < evaldata.size(); j++)
			{
				Transport trainingfT = Transport(); trainingfT.matrices.push_back(evaldata.at(j).input);
				int result = layers.at(0)->Feedforward(trainingfT); /////////////////// FEEDFORWARD
				if (layers.at(0)->debug == true)
					std::cout << "Evaluation: Test data label: " << /*(int)d[j].label*/ evaldata.LargestActivation(evaldata.at(j).output) << std::endl;

				if (evaldata.at(j).output.content[result] == 1)
				{
					if (layers.at(0)->debug == true) {
						std::cout << "\n HELYES!!!!!!!!!!" << std::endl;;
					}
					corr++;
					//getchar();
				}
				all++;

				//std::cout << "RESULT OF FORWARD: " << result << std::endl;
			}
			std::cout << "EVALUATION ON UNSEEN DATA: CORRECT: " << corr << " of " << (evaldata.size()) << " which is: " << ((float)corr / (float)all * 100) << "%" << std::endl;


		}
	};
}


#endif
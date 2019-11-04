#pragma once
#ifndef __FC__
#define __FC__


#include "Layer.h"
#include "Matrices.h"
#include "Activations.h"


namespace onlabor
{
	class FCLayer : public Layer
	{
		int inputs;
		int neurons;

		Matrix nabla_delta_w;
		Matrix delta_w;

		Matrix nabla_delta_b;
		Matrix delta_b;

		Matrix bias;
		Matrix prevActivations;
		Matrix out;		//activations
		Matrix deltas;

	public:
		Activation* activation;
		Matrix weights;
		Matrix errors;
		Matrix zs;
		Matrix zs_prime;

		FCLayer(int neur, Activation* act);
		FCLayer(int inp, int neur, Activation* act);
		virtual int Feedforward(Transport activations);
		void InitWeights();
		virtual void Backprop(Transport deltas);
		virtual void UpdateParameters(float learningRate);
		int LargestActivation(std::vector<float> arr);
		virtual void ConnectTo(Layer* l);

		~FCLayer();


		// Inherited via Layer
		virtual void Setup(int inp) override;


		// Inherited via Layer
		virtual Transport GetZs() override;


		// Inherited via Layer
		virtual Activation* GetActivation() override;


		// Inherited via Layer
		virtual void Load(string path, int layerIndex) override;

		virtual void Save(string path, int layerIndex) override;



	};
}
#endif
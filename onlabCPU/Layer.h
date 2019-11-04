#pragma once
#ifndef __LAY__
#define __LAY__


#include "Matrices.h"
#include "Transport.h"
#include "Activations.h"

namespace onlabor
{
	class Layer
	{
	protected:
	public:
		Layer* next;
		Layer* prev;
		int backpropCount;
		bool setup = false;
		bool debug = true;
		//Activation& activation;

		virtual void Backprop(Transport deltas) = 0;
		virtual void InitWeights() = 0;
		virtual void UpdateParameters(float learningRate) = 0;
		virtual int Feedforward(Transport activations) = 0;
		virtual Transport GetZs() = 0;
		virtual void ConnectTo(Layer* l) = 0;
		virtual void Setup(int inp) = 0;
		virtual Activation* GetActivation() = 0;


		virtual void Load(string path, int layerIndex) = 0;
		virtual void Save(string path, int layerIndex) = 0;

	};
}
#endif
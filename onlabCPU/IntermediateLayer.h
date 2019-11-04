#pragma once


#include <Matrices.h>
#include <Layer.h>
#include <Activations.h>
#include <Transport.h>
#include <random>
#include <ProxyTransport.h>


namespace onlabor
{
	class IntermediateLayer : public Layer
	{
		int mtxheight;
		int mtxwidth;
		int mtxnumber;
		Transport transport;
		Transport inputs;

	public:


		// Inherited via Layer
		virtual void Backprop(Transport deltas) override;

		virtual void InitWeights() override;

		virtual void UpdateParameters(float learningRate) override;

		virtual int Feedforward(Transport activations) override;

		virtual Transport GetZs() override;

		virtual void ConnectTo(Layer* l) override;

		virtual void Setup(int inp) override;


		// Inherited via Layer
		virtual Activation* GetActivation() override;


		// Inherited via Layer
		virtual void Load(string path, int layerIndex) override;

		virtual void Save(string path, int layerIndex) override;

	};
}

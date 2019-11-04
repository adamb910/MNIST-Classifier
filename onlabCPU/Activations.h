#pragma once
#ifndef __ACT__
#define __ACT__

#include <vector>
#include "Transport.h"

namespace onlabor
{
	

	class Activation
	{
	public:
		virtual float Activate(float z) = 0;
		virtual std::vector<float> Activate(std::vector<float> z) = 0;
		virtual Transport Activate(Transport z) = 0;

		virtual float Prime(float z) = 0;
		virtual std::vector<float> Prime(std::vector<float> z) = 0;
		virtual Transport Prime(Transport z) = 0;
	};

	class NoActivation : public Activation
	{
		// Inherited via Activation
		virtual float Activate(float z) override;
		virtual std::vector<float> Activate(std::vector<float> z) override;
		virtual Transport Activate(Transport z) override;
		virtual float Prime(float z) override;
		virtual std::vector<float> Prime(std::vector<float> z) override;
		virtual Transport Prime(Transport z) override;
	};

	class Sigmoid : public Activation
	{
	public:
		virtual float Activate(float z);
		virtual float Prime(float z);

		virtual std::vector<float> Activate(std::vector<float> z);
		virtual std::vector<float> Prime(std::vector<float> z);


		virtual Transport Activate(Transport z);
		virtual Transport Prime(Transport z);
	};

	class ReLu : public Activation
	{
	public:
		virtual float Activate(float z);
		virtual float Prime(float z);
		virtual std::vector<float> Activate(std::vector<float> z);
		virtual std::vector<float> Prime(std::vector<float> z);


		virtual Transport Activate(Transport z);
		virtual Transport Prime(Transport z);
	};

	class Softmax : public Activation
	{
	public:
		virtual std::vector<float> Activate(std::vector<float> z);
		virtual std::vector<float> Prime(std::vector<float> z);

		// Inherited via Activation
		virtual float Activate(float z) override;
		virtual float Prime(float z) override;

		virtual Transport Activate(Transport z) override;
		virtual Transport Prime(Transport z) override;
	};
	

	
}

#endif
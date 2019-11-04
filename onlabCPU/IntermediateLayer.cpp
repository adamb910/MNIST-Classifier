#include "IntermediateLayer.h"
using namespace onlabor;

void onlabor::IntermediateLayer::Backprop(Transport deltas)
{
	if (prev == nullptr)
		throw "Intermediate Layer needs to have some kind of other layer before it.";

	Transport back = transport;

	for (int i = 0; i < deltas.Count(); i++)
	{
		back.SetAt(i, deltas[i]);
	}

	prev->Backprop(back);
}

void onlabor::IntermediateLayer::InitWeights()
{
}

void onlabor::IntermediateLayer::UpdateParameters(float learningRate)
{
	if (next != nullptr)
		next->UpdateParameters(learningRate);
}

int onlabor::IntermediateLayer::Feedforward(Transport activations)
{

	if (next == nullptr)
		throw "Intermediate Layer needs to have some kind of other layer after it.";

	//int number = activations.matrices.size();
	//int onematrix = activations.matrices.at(0).GetHeight() * activations.matrices.at(0).GetWidth();
	transport = activations;

	Transport out = Transport(1);
	int actCount = activations.Count();
	out.matrices.at(0) = Matrix(actCount, 1);
	for (int i = 0; i < actCount; i++)
	{
		out.matrices.at(0).content[i] = activations[i];
	}
	inputs = out;

	return next->Feedforward(out);
}

Transport onlabor::IntermediateLayer::GetZs()
{

	/*Transport out = Transport(1);
	if (prev != nullptr)
	{

		Transport help = prev->GetZs();
		int actCount = help.Count();
		out.matrices.at(0) = Matrix(actCount, 1);
		for (int i = 0; i < actCount; i++)
		{
			out.matrices.at(0).content[i] = help[i];
		}
	}
	else
	{

	}

	return out;*/
	return Transport();
}

void onlabor::IntermediateLayer::ConnectTo(Layer* l)
{
	next = l;
	l->prev = (Layer*)this;
}

void onlabor::IntermediateLayer::Setup(int inp)
{
	//nothing
}

onlabor::Activation* onlabor::IntermediateLayer::GetActivation()
{
	return prev->GetActivation();
}

void onlabor::IntermediateLayer::Load(string path, int layerIndex)
{
}

void onlabor::IntermediateLayer::Save(string path, int layerIndex)
{
}

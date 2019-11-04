//#include "SoftmaxLayer.h"
//
//void onlabor::Softmax::Backprop(Matrix deltas, Matrix weightsOfNext)
//{
//	if (next == nullptr)
//	{
//		deltas = FCLayer::out.Subtract(passedDeltas);
//
//		//IF QUADRATIC COST:
//		//deltas = deltas.Hadamard(zs_prime);
//
//	}
//	else
//	{
//		Matrix nweightTrans = weightsOfNext.Transpose();
//		deltas = nweightTrans.Multiply(passedDeltas);
//		deltas = deltas.Hadamard(zs_prime);
//	}
//
//	if (prev != nullptr)
//	{
//		// BP3, setting Bias of backprop
//		nabla_delta_b = nabla_delta_b.Add(deltas);
//
//		// BP4, setting weights of backprop
//		Matrix prevActHelp = prevActivations.Transpose();
//
//
//		//ehelyett mtx szorzas????? 1el lentebb
//		//delta_w = prevActHelp.Dot(deltas);
//		delta_w = deltas.Multiply(prevActHelp);
//
//		nabla_delta_w = nabla_delta_w.Add(delta_w);
//
//
//		backpropCount++;
//
//		prev->Backprop(deltas, weights);
//	}
//	return;
//}
//
//void onlabor::Softmax::InitWeights()
//{
//}
//
//void onlabor::Softmax::UpdateParameters(float learningRate)
//{
//}
//
//int onlabor::Softmax::Feedforward(Matrix activations)
//{
//	return 0;
//}
//
//void onlabor::Softmax::ConnectTo(Layer * l)
//{
//}

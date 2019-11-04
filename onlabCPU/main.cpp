

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Network.h>
#include <readMNIST.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Network.h>
#include <Layer.h>
#include <vector>
#include <gpuszorzas.h>
#include <FCLayer.h>
#include <IntermediateLayer.h>

using namespace onlabor;



int main(int argc, char* argv[])
{


	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(4);
	std::cout << "Hello World!" << std::endl;

	/*Matrix m = Matrix(5, 5);
	for (int i = 0; i < m.content.size(); i++)
	{
		m.content[i] = 5;
	}

	m.Print();
	m.AddZeroPadding(4);
	std::cout << std::endl << " zero paddingel: " << std::endl;
	m.Print();

*/
#define testing 0

#if testing == 1
	///forward test
	/*ConvLayer prevc = ConvLayer((Activation&)ReLu(), 1, 1, 5, 5, 1);
	
	Transport prevT = Transport(1); 
	Matrix prevm = Matrix(10, 10);
	prevm.Ones();
	prevT.matrices[0] = prevm;
	prevc.z = prevT;*/
	ConvLayer c = ConvLayer((Activation&)Sigmoid(), 1, 2, 5, 5, 1);
	IntermediateLayer flatten = IntermediateLayer();
	FCLayer fc = FCLayer(6*6*1, (Activation&)Sigmoid());

	c.ConnectTo(&flatten);
	flatten.ConnectTo(&fc);

	//prevc.ConnectTo(&c);
//	Transport inp = Transport(1);
//	Matrix m = Matrix(10, 10);
//	m.Ones();
////	m.SetAt(4, 4, .6);
////	m.SetAt(5, 5, .6);
//	inp.matrices[0] = m;
//	m.Print();
//	c.Feedforward(inp);

	///backward test
	/*Transport back = Transport(1);
	Matrix d = Matrix(6, 6);
	d.Zeros();
	d.SetAt(5, 3, 1);
	d.SetAt(2, 3, .7);
	d.SetAt(1, 5, .6);
	d.SetAt(0, 4, .2);
	d.Print();
	back.matrices[0] = d;
	c.Backprop(back);

	c.UpdateParameters(0.5);*/

	Matrix inp1010 = Matrix(10, 10);
	inp1010.Zeros();
	inp1010.SetAt(1, 1, .2);
	inp1010.SetAt(9, 9,.3);
	inp1010.SetAt(4, 2,.4);
	inp1010.SetAt(5, 8,.5);
	inp1010.SetAt(8, 2,.6);
	Transport t1010 = Transport(1);
	t1010.matrices[0] = inp1010;
	c.Feedforward(t1010);


	Transport back = Transport(1);
	Matrix bm = Matrix(36, 1);
	for (int i = 0; i < bm.content.size(); i++)
	{
		if (i % 2 != 0)
		{
			bm.content[i] = 10 * (i + 1);
		}
		else
		{
			bm.content[i] =  0;//i + 1;
		}
	}
	std::cout << "back\n";
	back.Print();
	back.matrices[0] = bm;
	fc.Backprop(back);

	back = Transport(1);
	bm = Matrix(36, 1);
	for (int i = 0; i < bm.content.size(); i++)
	{
		if (i % 2 == 0)
		{
			bm.content[i] = (i + 1);
		}
		else
		{
			bm.content[i] = 0;//i + 1;
		}
	}
	std::cout << "back\n";
	back.Print();
	back.matrices[0] = bm;
	fc.Backprop(back);





	c.UpdateParameters(0.5);
	c.Feedforward(t1010);

#else
using namespace std;
	freopen("output.txt", "w", stdout);
	freopen("error.txt", "w", stderr);

	BUAKNet network = BUAKNet("train-images.idx3-ubyte", "train-labels.idx1-ubyte","t10k-images.idx3-ubyte","t10k-labels.idx1-ubyte");
	//network.Train(10, 10);
	//network.Evauluate();
	network.Test();

#endif

	return 0;
}
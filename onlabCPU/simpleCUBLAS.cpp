//
///* Includes, system */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include <Network.h>
//#include <Layer.h>
//#include <vector>
//#include <gpuszorzas.h>
//#include <FCLayer.h>
//
//
//using namespace onlabor;
//
//
//void matrixSzorzasTeszt()
//{
//	int ah = 5; int aw = 1; 
//	Matrix am = Matrix(ah, aw);
//	float* a = &am.content[0];
//
//	int bh = 1; int bw = 3;
//	Matrix bm = Matrix(bh, bw);
//	float* b = &bm.content[0];
//
//	for (int i = 0; i < am.content.size(); i++)
//		a[i] = 3;
//	for (int i = 0; i < bm.content.size(); i++)
//		b[i] = 1;
//
//	Matrix result = Matrix(); 
//	result = am.Dot(bm);
//	std::cout << "\n result: " << result.content.size() << " \n";
//	/*for (int i = 0; i < result.content.size(); i++)
//	{
//		if (i % bw == 0) std::cout << std::endl;
//		std::cout << " " << result.content[i] << ", ";
//	}*/
//}
//
//void copyalot()
//{
//	Matrix m = Matrix(784, 100);
//	for (int i = 0; i < 784 * 100; i++)
//		m.content[i] = 5;
//
//	high_resolution_clock::time_point t1 = high_resolution_clock::now();
//
//	Matrix p = m;
//	high_resolution_clock::time_point t2 = high_resolution_clock::now();
//	auto duration = duration_cast<milliseconds>(t2 - t1).count();
//	std::cout << "\n ADDITION TIME: " << duration << " ms" << std::endl;
//}
//
//void RotateTest()
//{
//	Matrix m = Matrix(5, 5);
//	for (int i = 0; i < 25; i++)
//	{
//		m.content[i] = i;
//	}
//	m.Print();
//
//	m.Rotate180();
//	std::cout << "\n\n\n";
//
//	m.Print();
//
//}
//
/////* Main */
////int main(int argc, char **argv) {
////	
////	//matrixSzorzasTeszt();
////	//copyalot();
////
////	//RotateTest();
////	BUAKNet network = BUAKNet("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
////
////
////
////}

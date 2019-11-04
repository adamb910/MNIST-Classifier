//#include <iostream>
//#include "gpuszorzas.h"
//#include <cublas_v2.h>
//#include <cuda_runtime.h>
//#include <helper_cuda.h>
//#include <chrono>
//
//
//using namespace std::chrono;
//using namespace onlabor;
//
///* Host implementation of a simple version of sgemm */
//static void simple_sgemm(int n, float alpha, const float *A, const float *B,
//	float beta, float *C) {
//	int i;
//	int j;
//	int k;
//
//	for (i = 0; i < n; ++i) {
//		for (j = 0; j < n; ++j) {
//			float prod = 0;
//
//			for (k = 0; k < n; ++k) {
//				prod += A[k * n + i] * B[j * n + k];
//			}
//
//			C[j * n + i] = alpha * prod + beta * C[j * n + i];
//		}
//	}
//}
//
//
//std::vector<float> onlabor::GPUSzorzo::Szorzas(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w)
//{
//
//
//	std::vector<float> result = std::vector<float>(a_h*b_w);
//	cublasStatus_t status;
//	float *h_A = &a[0];
//	float *h_B = &b[0];
//	float *h_C = 0; &result[0];
//
//	/*for (int i = 0; i < a_h * a_w; i++)
//	{
//		if (i % a_w == 0) std::cout << std::endl;
//		std::cout << " " << a[i] << ", ";
//	}*/
//
//	float *d_A = 0;
//	float *d_B = 0;
//	float *d_C = 0;
//	float alpha = 1.0f;
//	float beta = 0.0f;
//	int n2 = 16 * 16;
//	int i;
//	float error_norm;
//	float ref_norm;
//	float diff;
//	cublasHandle_t handle;
//
//
//	/* Initialize CUBLAS */
//	//printf("simpleCUBLAS test running..\n");
//
//	status = cublasCreate(&handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! CUBLAS initialization error\n");
//	}
//
//	///* Allocate host memory for the matrices */
//	h_A = reinterpret_cast<float *>(malloc(a_w * a_h * sizeof(h_A[0])));
//
//	if (h_A == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (A)\n");
//	}
//
//	h_B = reinterpret_cast<float *>(malloc(a_w * b_w * sizeof(h_B[0])));
//
//	if (h_B == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (B)\n");
//	}
//
//	h_C = reinterpret_cast<float *>(malloc(a_h * b_w * sizeof(h_C[0])));
//
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//	}
//
//	/* Fill the matrices with test data */
//	for (i = 0; i < a_w * a_h; i++) {
//		h_A[i] = a[i];
//	}
//	for (i = 0; i < a_w * b_w; i++) {
//		h_B[i] = b[i];
//	}
//	//for (i = 0; i < a_h * b_w; i++) {
//	//	h_C[i] = 0;
//	//}
//	
//
//
//	/* Allocate device memory for the matrices */
//	if (cudaMalloc(reinterpret_cast<void **>(&d_A), a_w * a_h * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate A)\n");
//		 
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_B), a_w * b_w * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate B)\n");
//		 
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_C), a_h * b_w * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate C)\n");
//		 
//	}
//
//	/* Initialize the device matrices with the host matrices */
//	status = cublasSetVector(a_w * a_h, sizeof(h_A[0]), h_A, 1, d_A, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write A)\n");
//		 
//	}
//
//	status = cublasSetVector(a_w * b_w, sizeof(h_B[0]), h_B, 1, d_B, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write B)\n");
//		 
//	}
//
//	status = cublasSetVector(a_h * b_w, sizeof(h_C[0]), h_C, 1, d_C, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write C)\n");
//		 
//	}
//
//	///* Performs operation using plain C code */
//	//simple_sgemm(N, alpha, h_A, h_B, beta, h_C);
//	//h_C_ref = h_C;
//
//	/* Performs operation using cublas */
//	status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, a_h, b_w, b_h, &alpha, d_A, a_h, d_B, b_h, &beta, d_C, a_h);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, a_h, b_w, a_w, &alpha, d_A, a_w, d_B, b_w, &beta, d_C, b_w);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_T, a_h, b_h, a_h, &alpha, d_A, a_h, d_B, b_h, &beta, d_C, b_h);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! kernel execution error.\n");
//		 
//	}
//
//	/* Allocate host memory for reading back the result from device memory */
//	//h_C = reinterpret_cast<float *>(malloc(a_h * b_w * sizeof(h_C[0])));
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//		 
//	}
//
//	/* Read the result back */
//	status = cublasGetVector(a_h * b_w, sizeof(float), d_C, 1, h_C, 1);
//	result.resize(0);
//	for (int i = 0; i < a_h * b_w; i++)
//	{
//		//std::cout << " " << h_C[i] << ", ";
//		result.push_back(h_C[i]);
//	}
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (read C)\n");
//		 
//	}
//
//	/* Check result against reference */
//	error_norm = 0;
//	ref_norm = 0;
//
//
//	/* Memory clean up */
//	free(h_A);
//	free(h_B);
//	free(h_C);
//
//	/* Shutdown */
//	status = cublasDestroy(handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! shutdown error (A)\n");
//		 
//	}
//	//std::cout << "\n matrix szorzas siker!!!!!!!!!!!!!!!\n";
//
//	return result;
//}
//
//std::vector<float> onlabor::GPUSzorzo::Dot(std::vector<float> a, int a_h, int a_w, std::vector<float> b, int b_h, int b_w)
//{
//	
//	std::vector<float> result = std::vector<float>(a.size() * b.size());
//	cublasStatus_t status;
//	float *h_A = &a[0];
//	float *h_B = &b[0];
//	/*float *h_C = 0; &result[0];*/
//	float *h_C = &result[0];
//
//	/*for (int i = 0; i < a_h * a_w; i++)
//	{
//		if (i % a_w == 0) std::cout << std::endl;
//		std::cout << " " << a[i] << ", ";
//	}*/
//
//	float *d_A = 0;
//	float *d_B = 0;
//	float *d_C = 0;
//	float alpha = 1.0f;
//	float beta = 0.0f;
//	int n2 = 16 * 16;
//	int i;
//	float error_norm;
//	float ref_norm;
//	float diff;
//	cublasHandle_t handle;
//
//
//	/* Initialize CUBLAS */
//	//printf("simpleCUBLAS test running..\n");
//
//	status = cublasCreate(&handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! CUBLAS initialization error\n");
//	}
//
//	///* Allocate host memory for the matrices */
//	h_A = reinterpret_cast<float *>(malloc(a_w * a_h * sizeof(h_A[0])));
//
//	if (h_A == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (A)\n");
//	}
//
//	h_B = reinterpret_cast<float *>(malloc(b_h * b_w * sizeof(h_B[0])));
//
//	if (h_B == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (B)\n");
//	}
//
//
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//	}
//
//	/* Fill the matrices with test data */
//	for (i = 0; i < a_w * a_h; i++) {
//		h_A[i] = a[i];
//	}
//	for (i = 0; i < b_h * b_w; i++) {
//		h_B[i] = b[i];
//	}
//	//for (i = 0; i < a_h * b_w; i++) {
//	//	h_C[i] = 0;
//	//}
//
//
//
//	/* Allocate device memory for the matrices */
//	if (cudaMalloc(reinterpret_cast<void **>(&d_A), a_w * a_h * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate A)\n");
//
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_B), b_h * b_w * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate B)\n");
//
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_C), a_h * b_w * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate C)\n");
//
//	}
//
//	/* Initialize the device matrices with the host matrices */
//	status = cublasSetVector(a_w * a_h, sizeof(h_A[0]), h_A, 1, d_A, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write A)\n");
//
//	}
//
//	status = cublasSetVector(b_h * b_w, sizeof(h_B[0]), h_B, 1, d_B, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write B)\n");
//
//	}
//
//	status = cublasSetVector(a_h * b_w, sizeof(h_C[0]), h_C, 1, d_C, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write C)\n");
//
//	}
//
//	///* Performs operation using plain C code */
//	//simple_sgemm(N, alpha, h_A, h_B, beta, h_C);
//	//h_C_ref = h_C;
//	float alpha2 = 0.0;
//	/* Performs operation using cublas */
//
//	status = cublasSger(handle, a_h, b_w, &alpha2, d_A, 1, d_B, 1, d_C, a_h);	
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, a_h, b_w, b_h, &alpha, d_A, a_h, d_B, b_h, &beta, d_C, a_h);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, a_h, b_w, a_w, &alpha, d_A, a_w, d_B, b_w, &beta, d_C, b_w);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_T, a_h, b_h, a_h, &alpha, d_A, a_h, d_B, b_h, &beta, d_C, b_h);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! kernel execution error.\n");
//
//	}
//
//	/* Allocate host memory for reading back the result from device memory */
//	//h_C = reinterpret_cast<float *>(malloc(a_h * b_w * sizeof(h_C[0])));
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//
//	}
//
//	/* Read the result back */
//	status = cublasGetVector(a_h * b_w, sizeof(float), d_C, 1, h_C, 1);
//	//result.resize(0);
//	for (int i = 0; i < a_h * b_w; i++)
//	{
//		//std::cout << " " << h_C[i] << ", ";
//		//result.push_back(h_C[i]);
//	}
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (read C)\n");
//
//	}
//
//	/* Check result against reference */
//	error_norm = 0;
//	ref_norm = 0;
//
//
//	/* Memory clean up */
//	free(h_A);
//	free(h_B);
//
//	/* Shutdown */
//	status = cublasDestroy(handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! shutdown error (A)\n");
//
//	}
//
//	//std::cout << "\n matrix szorzas siker!!!!!!!!!!!!!!!\n";
//	return result;
//}
//
//std::vector<float> onlabor::GPUSzorzo::AddMtx(std::vector<float> a, std::vector<float> b, int height, int width)
//{
//
//	std::vector<float> result = std::vector<float>(height*width);
//	cublasStatus_t status;
//	float *h_A = &a[0];
//	float *h_B = &b[0];
//	float *h_C = 0; &result[0];
//
//	/*for (int i = 0; i < a_h * a_w; i++)
//	{
//		if (i % a_w == 0) std::cout << std::endl;
//		std::cout << " " << a[i] << ", ";
//	}*/
//
//	float *d_A = 0;
//	float *d_B = 0;
//	float *d_C = 0;
//	float alpha = 1.0f;
//	float beta = 0.0f;
//	int n2 = 16 * 16;
//	int i;
//	float error_norm;
//	float ref_norm;
//	float diff;
//	cublasHandle_t handle;
//
//
//	/* Initialize CUBLAS */
//	//printf("simpleCUBLAS test running..\n");
//
//	status = cublasCreate(&handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! CUBLAS initialization error\n");
//	}
//
//	///* Allocate host memory for the matrices */
//	h_A = reinterpret_cast<float *>(malloc(height * width * sizeof(h_A[0])));
//
//	if (h_A == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (A)\n");
//	}
//
//	h_B = reinterpret_cast<float *>(malloc(height * width * sizeof(h_B[0])));
//
//	if (h_B == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (B)\n");
//	}
//
//	h_C = reinterpret_cast<float *>(malloc(height * width * sizeof(h_C[0])));
//
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//	}
//
//	/* Fill the matrices with test data */
//	for (i = 0; i < height * width; i++) {
//		h_A[i] = a[i];
//	}
//	for (i = 0; i < height * width; i++) {
//		h_B[i] = b[i];
//	}
//	//for (i = 0; i < a_h * b_w; i++) {
//	//	h_C[i] = 0;
//	//}
//
//
//
//	/* Allocate device memory for the matrices */
//	if (cudaMalloc(reinterpret_cast<void **>(&d_A), height * width * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate A)\n");
//
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_B), height * width * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate B)\n");
//
//	}
//
//	if (cudaMalloc(reinterpret_cast<void **>(&d_C), height * width * sizeof(float)) !=
//		cudaSuccess) {
//		fprintf(stderr, "!!!! device memory allocation error (allocate C)\n");
//
//	}
//
//	/* Initialize the device matrices with the host matrices */
//	status = cublasSetVector(height * width, sizeof(h_A[0]), h_A, 1, d_A, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write A)\n");
//
//	}
//
//	status = cublasSetVector(height * width, sizeof(h_B[0]), h_B, 1, d_B, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write B)\n");
//
//	}
//
//	status = cublasSetVector(height * width, sizeof(h_C[0]), h_C, 1, d_C, 1);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (write C)\n");
//
//	}
//
//	///* Performs operation using plain C code */
//	//simple_sgemm(N, alpha, h_A, h_B, beta, h_C);
//	//h_C_ref = h_C;
//
//	/* Performs operation using cublas */
//	status = cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, height, width, &alpha, d_A, height, &beta, d_B, height, d_C, height);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, a_h, b_w, a_w, &alpha, d_A, a_w, d_B, b_w, &beta, d_C, b_w);
//	//status = cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_T, a_h, b_h, a_h, &alpha, d_A, a_h, d_B, b_h, &beta, d_C, b_h);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! kernel execution error.\n");
//
//	}
//
//	/* Allocate host memory for reading back the result from device memory */
//	//h_C = reinterpret_cast<float *>(malloc(a_h * b_w * sizeof(h_C[0])));
//	if (h_C == 0) {
//		fprintf(stderr, "!!!! host memory allocation error (C)\n");
//
//	}
//
//	/* Read the result back */
//	status = cublasGetVector(height * width, sizeof(float), d_C, 1, h_C, 1);
//	result.resize(0);
//	for (int i = 0; i < height * width; i++)
//	{
//		//std::cout << " " << h_C[i] << ", ";
//		result.push_back(h_C[i]);
//	}
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! device access error (read C)\n");
//
//	}
//
//	/* Check result against reference */
//	error_norm = 0;
//	ref_norm = 0;
//
//
//	/* Memory clean up */
//	free(h_A);
//	free(h_B);
//	free(h_C);
//
//	/* Shutdown */
//	status = cublasDestroy(handle);
//
//	if (status != CUBLAS_STATUS_SUCCESS) {
//		fprintf(stderr, "!!!! shutdown error (A)\n");
//
//	}
//	//std::cout << "\n matrix szorzas siker!!!!!!!!!!!!!!!\n";
//
//	return result;
//}
//
//std::vector<float> onlabor::GPUSzorzo::SubtractMtx(std::vector<float> a, std::vector<float> b, int height, int width)
//{
//	return std::vector<float>();
//}
//
//std::vector<float> onlabor::GPUSzorzo::Hadamard(std::vector<float> a, std::vector<float> b, int height, int width)
//{
//	return std::vector<float>();
//}

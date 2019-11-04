#pragma once
#ifndef __MNIST_H2__
#define __MNIST_H2__
#include <iostream>
#include <fstream>
typedef struct mnistdata {
	unsigned char  data[784]; /* 28x28 data for the image */
	float dataf[784];
	unsigned char label; /* label : 0 to 9 */
	float labelf[10];

	mnistdata()
	{
	}

	void convert()
	{
	
		for (int i = 0; i < 784; i++)
		{
			dataf[i] = ((float)data[i] / 255.0f);
		}

		for (int i = 0; i < 10; i++)
		{
			labelf[i] = 0;
		}

		labelf[label] = 1;
	}

	~mnistdata()
	{
		
	}


} mnistdata;



typedef struct mnistreader
{
	uint32_t swap_endian(uint32_t val) {
		val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		return (val << 16) | (val >> 16);
	}

	mnistdata* read_mnist_cv(const char* image_filename, const char* label_filename, unsigned int* cnt, int mennyit) {
		// Open files
		std::ifstream image_file(image_filename, std::ios::in | std::ios::binary);
		std::ifstream label_file(label_filename, std::ios::in | std::ios::binary);

		if (image_file.good())
		{
			std::cout << "LETEZIK" << std::endl;
		}

		// Read the magic and the meta data
		uint32_t magic;
		uint32_t num_items;
		uint32_t num_labels;
		uint32_t rows;
		uint32_t cols;

		image_file.read(reinterpret_cast<char*>(&magic), 4);
		magic = swap_endian(magic);
		if (magic != 2051) {
			//cout << "Incorrect image file magic: " << magic << endl;
			return NULL;
		}

		label_file.read(reinterpret_cast<char*>(&magic), 4);
		magic = swap_endian(magic);
		if (magic != 2049) {
			//cout << "Incorrect image file magic: " << magic << endl;
			return NULL;
		}

		image_file.read(reinterpret_cast<char*>(&num_items), 4);
		num_items = swap_endian(num_items);
		label_file.read(reinterpret_cast<char*>(&num_labels), 4);
		num_labels = swap_endian(num_labels);
		if (num_items != num_labels) {
			//cout << "image file nums should equal to label num" << endl;
			return NULL;
		}

		image_file.read(reinterpret_cast<char*>(&rows), 4);
		rows = swap_endian(rows);
		image_file.read(reinterpret_cast<char*>(&cols), 4);
		cols = swap_endian(cols);

		/*cout << "image and label num is: " << num_items << endl;
		cout << "image rows: " << rows << ", cols: " << cols << endl;*/

		mnistdata* data = new mnistdata[num_items];

		char label;
		char* pixels = new char[rows * cols];
		num_items = mennyit;
		*cnt = (int)num_items;
		for (int item_id = 0; item_id < num_items; ++item_id) {
			system("CLS");
			std::cout << "loaded training data: " << item_id << " of " << num_items << std::endl;
			// read image pixel
			image_file.read(pixels, rows * cols);
			// read label
			label_file.read(&label, 1);

			//std::string sLabel = std::to_string(int(label));

			for (int i = 0; i < rows * cols; i++)
			{
				data[item_id].data[i] = pixels[i];
			}

			for (int i = 0; i < 10; i++)
			{
				data[item_id].label = label;
			}

			data[item_id].convert();

			//std::cout << "label: " << label << std::endl;

			/*cout << "lable is: " << sLabel << endl;*/
			// convert it to cv Mat, and show it
			//cv::Mat image_tmp(rows, cols, CV_8UC1, pixels);
			//// resize bigger for showing
			//cv::resize(image_tmp, image_tmp, cv::Size(100, 100));
			//cv::imshow(sLabel, image_tmp);
			//cv::waitKey(0);
		}

		delete[] pixels;
		return data;
	}

} mnistreader;



/*
 * MNIST loader by Nuri Park - https://github.com/projectgalateia/mnist
 */

#define USE_MNIST_LOADER

#ifdef USE_MNIST_LOADER /* Fundamental macro to make the code active */

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * Make mnist_load function static.
	 * Define when the header is included multiple time.
	 */
#ifdef MNIST_STATIC
#define _STATIC static
#else
#define _STATIC 
#endif

	 /*
	  * Make mnist loader to load image data as double type.
	  * It divides unsigned char values by 255.0, so the results ranges from 0.0 to 1.0
	  */
#ifdef MNIST_DOUBLE
#define MNIST_DATA_TYPE double
#else
#define MNIST_DATA_TYPE unsigned char
#endif

	typedef struct mnist_data {
		unsigned char  data[784]; /* 28x28 data for the image */
		float* dataf;
		unsigned int label; /* label : 0 to 9 */
		float* labelf;

		mnist_data()
		{
		}

		void convert()
		{
			dataf = new float[784];
			labelf = new float[10];
			for (int i = 0; i < 784; i++)
			{
				dataf[i] = ((float)data[i] / 255.0f);
			}

			for (int i = 0; i < 10; i++)
			{
				labelf[i] = 0;
			}

			labelf[label] = 1;
		}

		~mnist_data()
		{
			delete[] dataf;
			delete[] labelf;
		}
	

	} mnist_data;

	/*
	 * If it's header inclusion, make only function prototype visible.
	 */
#ifdef MNIST_HDR_ONLY



#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	 /*
	  * Load a unsigned int from raw data.
	  * MSB first.
	  */
	static unsigned int mnist_bin_to_int(char *v)
	{
		int i;
		unsigned int ret = 0;

		for (i = 0; i < 4; ++i) {
			ret <<= 8;
			ret |= (unsigned char)v[i];
		}

		return ret;
	}

	/*
	 * MNIST dataset loader.
	 *
	 * Returns 0 if successed.
	 * Check comments for the return codes.
	 */

	static int mnist_load(
		const char *image_filename,
		const char *label_filename,
		mnist_data **data,
		unsigned int *count)
	{
		int return_code = 0;
		int i;
		char tmp[4];

		unsigned int image_cnt, label_cnt;
		unsigned int image_dim[2];

		FILE *ifp = fopen(image_filename, "rb");
		FILE *lfp = fopen(label_filename, "rb");

		if (!ifp || !lfp) {
			return_code = -1; /* No such files */
			goto cleanup;
		}

		fread(tmp, 1, 4, ifp);
		if (mnist_bin_to_int(tmp) != 2051) {
			return_code = -2; /* Not a valid image file */
			goto cleanup;
		}

		fread(tmp, 1, 4, lfp);
		if (mnist_bin_to_int(tmp) != 2049) {
			return_code = -3; /* Not a valid label file */
			goto cleanup;
		}

		fread(tmp, 1, 4, ifp);
		image_cnt = mnist_bin_to_int(tmp);

		fread(tmp, 1, 4, lfp);
		label_cnt = mnist_bin_to_int(tmp);

		if (image_cnt != label_cnt) {
			return_code = -4; /* Element counts of 2 files mismatch */
			goto cleanup;
		}

		for (i = 0; i < 2; ++i) {
			fread(tmp, 1, 4, ifp);
			image_dim[i] = mnist_bin_to_int(tmp);
		}

		if (image_dim[0] != 28 || image_dim[1] != 28) {
			return_code = -2; /* Not a valid image file */
			goto cleanup;
		}

		*count = image_cnt;
		*data = (mnist_data *)malloc(sizeof(mnist_data) * image_cnt);

		for (i = 0; i < image_cnt; ++i) {
			int j;
			unsigned char read_data[28 * 28];
			mnist_data *d = &(*data)[i];

			fread(read_data, 1, 28 * 28, ifp);

#ifdef MNIST_DOUBLE
			for (j = 0; j < 28 * 28; ++j) {
				d->data[j / 28][j % 28] = read_data[j] / 255.0;
			}
#else
			memcpy(d->data, read_data, 28 * 28);
#endif

			fread(tmp, 1, 1, lfp);
			d->label = tmp[0];
		}

	cleanup:
		if (ifp) fclose(ifp);
		if (lfp) fclose(lfp);

		return return_code;
	}
#endif /* MNIST_HDR_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* USE_MNIST_LOADER */
#endif /* __MNIST_H__ */
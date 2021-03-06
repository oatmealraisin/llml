/*
 * idx.c: Utility functionf for the IDX file format
 * http://yann.lecun.com/exdb/mnist/
 * 
 * Copyright (C) 2018 oatmealraisin (ryan@murph.ai)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef llml_idx_c
#define llml_idx_c

#include <stdio.h>
#include <stdlib.h>

// IDX files are big-endian, this flips any short
unsigned short short_flip(unsigned short input)
{
	input = (((input & 0xff00) >> 8) | ((input & 0x00ff) << 8));
	return input;
}

// IDX files are big-endian, this flips any int
unsigned int int_flip(unsigned int input)
{
//	printf("Input: 0x%x\n", input);
	input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
	input = (((input & 0xffff0000) >> 16) | ((input & 0x0000ffff) << 16));
//	printf("Output: 0x%x\n", input);
	return input;
}

FILE *open_IDX_file(char *fileName)
{
	FILE *result = fopen(fileName, "rb");
	if(result == NULL) {
		printf("Couldn't open file! %s\n", fileName);
		return NULL;
	}

	return result;
}

// The magic number contains 4 bytes. The first 2 are always 0, the third
// encodes the data type, the fourth is the number of dimensions.
// get_magic_number(FILE*) gets all of these bytes as an int
unsigned int get_magic_number(FILE *idxFile)
{
	unsigned int result;
	if(1 != fread(&result, sizeof(unsigned int), 1, idxFile)) {
		printf("Could not read the file\n");
		// TODO: Should I close the file here?
		return 0;
	}

	return int_flip(result);
}

// The magic number contains 4 bytes. The first 2 are always 0, the third
// encodes the data type, the fourth is the number of dimensions.
// get_data_type(FILE*) gets the third byte from this number
unsigned char get_data_type(FILE *idxFile)
{
	if(fseek(idxFile, 2, SEEK_SET) != 0) {
		printf("Train file has become unoperable.\n");
		return 0;
	}

	unsigned char dtype;
	if(1 != fread(&dtype, sizeof(dtype), 1, idxFile)) {
		printf("Could not read the file\n");
		return 0;
	}

	return dtype;
}

// The magic number contains 4 bytes. The first 2 are always 0, the third
// encodes the data type, the fourth is the number of dimensions.
// get_num_dimensions(FILE*) gets the fourth byte from this number
unsigned char get_num_dimensions(FILE *idxFile)
{
	if(fseek(idxFile, 3, SEEK_SET) != 0) {
		printf("Train file has become unoperable.\n");
		return 0;
	}

	unsigned char numDims;
	if(1 != fread(&numDims, sizeof(numDims), 1, idxFile)) {
		printf("Could not read the file\n");
		return 0;
	}

	return numDims;
}

// After the magic number, there are a number of int's which tell the
// dimensions of the following data, including the number of samples
unsigned int get_dimension(FILE *idxFile, unsigned int index)
{
	if(fseek(idxFile, 4 + index * 4, SEEK_SET) != 0) {
		printf("Train file has become unoperable.\n");
		return 0;
	}

	unsigned int dim;
	if(1 != fread(&dim, sizeof(dim), 1, idxFile)) {
		printf("Could not read the file\n");
		fclose(idxFile);
		return 0;
	}

	return int_flip(dim);
}


void *get_sample_at(FILE *idxFile, unsigned int index)
{
	unsigned int numDims = get_num_dimensions(idxFile);
	unsigned int dataTypeSize;

	switch(get_data_type(idxFile)) {
		case 0x08:
			dataTypeSize = sizeof(unsigned char);
			break;
		case 0x09:
			dataTypeSize = sizeof(char);
			break;
		case 0x0B:
			dataTypeSize = sizeof(short);
			break;
		case 0x0C:
			dataTypeSize = sizeof(int);
			break;
		case 0x0D:
			dataTypeSize = sizeof(float);
			break;
		case 0x0E:
			dataTypeSize = sizeof(double);
			break;
		default:
			printf("Unknown data type in IDX file.\n");
			return NULL;
	}

	unsigned int size_sample = 1;
	for(int i = 1; i < numDims; i++) {
		size_sample *= get_dimension(idxFile, i);
	}

	if(fseek(idxFile, 4 + 4 * numDims + index * size_sample * dataTypeSize, SEEK_SET) != 0) {
		printf("Train file has become unoperable.\n");
		return NULL;
	}

	void* sample = malloc(dataTypeSize * size_sample);
	if(size_sample != fread(sample, dataTypeSize, size_sample, idxFile)) {
		printf("Could not read the file\n");
		return NULL;
	}

	return sample;
}

// get_all_samples dumps all of the data from a file
void *get_all_samples(FILE *idxFile)
{
	unsigned int numDims = get_num_dimensions(idxFile);
	unsigned int dataTypeSize;

	switch(get_data_type(idxFile)) {
		case 0x08:
			dataTypeSize = sizeof(unsigned char);
			break;
		case 0x09:
			dataTypeSize = sizeof(char);
			break;
		case 0x0B:
			dataTypeSize = sizeof(short);
			break;
		case 0x0C:
			dataTypeSize = sizeof(int);
			break;
		case 0x0D:
			dataTypeSize = sizeof(float);
			break;
		case 0x0E:
			dataTypeSize = sizeof(double);
			break;
		default:
			printf("Unknown data type in IDX file.\n");
			return NULL;
	}

	unsigned int size_data = 1;
	for(int i = 0; i < numDims; i++) {
		size_data *= get_dimension(idxFile, i);
	}

	// Position at the beginning of the data
	if(fseek(idxFile, 4 + 4 * numDims, SEEK_SET) != 0) {
		printf("Train file has become unoperable.\n");
		return NULL;
	}

	// Slurp
	void* data = malloc(dataTypeSize * size_data);
	if(size_data != fread(data, dataTypeSize, size_data, idxFile)) {
		printf("Could not read the file\n");
		return NULL;
	}

	return data;
}
#endif

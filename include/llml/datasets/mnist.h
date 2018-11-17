/* 
 * mnist.h: Utility functions for working with the MNIST OCR dataset
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

#ifndef llml_mnist_h
#define llml_mnist_h

#include <stdio.h>

void mnist_set_working_dir(char *dir);

void mnist_set_url_trImages(char *url);
void mnist_set_url_trLabels(char *url);
void mnist_set_url_teImages(char *url);
void mnist_set_url_teLabels(char *url);

void mnist_set_name_trImages(char *name);
void mnist_set_name_trLabels(char *name);
void mnist_set_name_teImages(char *name);
void mnist_set_name_teLabels(char *name);

unsigned char mnist_download();

FILE *mnist_open_trImages();
FILE *mnist_open_trLabels();
FILE *mnist_open_teImages();
FILE *mnist_open_teLabels();

unsigned int mnist_num_entries(FILE *mnist);
unsigned int mnist_size_img(FILE *mnist);

unsigned char *mnist_alloc_images(unsigned int num);

unsigned char mnist_tr_get_image(FILE *mnist, unsigned int idx,
	unsigned char *dst);
unsigned char mnist_tr_get_images(FILE *mnist, unsigned int idx,
	unsigned int count, unsigned char *dst);
unsigned char mnist_tr_get_label(FILE *mnist, unsigned int idx,
	unsigned char *dst);
unsigned char mnist_tr_get_labels(FILE *mnist, unsigned int idx,
	unsigned int count, unsigned char *dst);

unsigned char mnist_te_get_image(FILE *mnist, unsigned int idx,
	unsigned char *dst);
unsigned char mnist_te_get_images(FILE *mnist, unsigned int idx,
	unsigned int count, unsigned char *dst);
unsigned char mnist_te_get_label(FILE *mnist, unsigned int idx,
	unsigned char *dst);
unsigned char mnist_te_get_labels(FILE *mnist, unsigned int idx,
	unsigned int count, unsigned char *dst);

#endif

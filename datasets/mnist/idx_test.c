/*
 * idx_test.c: Unit tests for idx.c
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

#include "minunit.h"

#include "idx.c"

int tests_run = 0;
FILE *testFile;

void setup()
{
	testFile = open_IDX_file("/home/ryan/.local/mnist/train-images-idx3-ubyte");
	if (testFile == NULL) mu_fail("Unable to open test file");
}

void teardown()
{
	fclose(testFile);
}

// Tests concerning the first few bytes in an IDX file, describing the data
MU_TEST(Metadata)
{
	mu_assert_int_eq(2051, get_magic_number(testFile));
	mu_assert_int_eq(8, get_data_type(testFile));
	mu_assert_int_eq(3, get_num_dimensions(testFile));
	mu_assert_int_eq(60000, get_dimension(testFile, 0));
	mu_assert_int_eq(28, get_dimension(testFile, 1));
	mu_assert_int_eq(28, get_dimension(testFile, 2));
}

// Tests about extracting content from IDX files
MU_TEST(Content)
{
	// First non-zero pixel of the first image
	char *sample = (char*) get_sample_at(testFile, 0);
	if( sample == NULL ) mu_fail("Returned data was empty.");
	mu_assert_int_eq(3, sample[152]);
	free(sample);

	// First non-zero pixel of the 5306th image
	sample = (char*) get_sample_at(testFile, 5305);
	if( sample == NULL ) mu_fail("Returned data was empty.");
	mu_assert_int_eq(52, sample[160]);
	free(sample);

	sample = (char*) get_all_samples(testFile);
	if( sample == NULL ) mu_fail("Returned data was empty.");
	// First non-zero pixel of the first image
	mu_assert_int_eq(3, sample[152]);
	// First non-zero pixel of the 5306th image
	mu_assert_int_eq(52, sample[5305 * 28 * 28 + 160]);
	free(sample);

	// TODO: Do non-char value
}

MU_TEST_SUITE(IDX)
{
	MU_SUITE_CONFIGURE(setup, teardown);
	MU_RUN_TEST(Metadata);
	MU_RUN_TEST(Content);
}

int main()
{
	MU_RUN_SUITE(IDX);
	MU_REPORT();

	return 0;
}

/* 
 * util.h: Utility functions for working with data and llml
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

#ifndef llml_util_h
#define llml_util_h

// Normalize the data in src to have a 0 mean and unit std
unsigned char *llml_normalize(double *src, unsigned int num, double *dst);

#endif

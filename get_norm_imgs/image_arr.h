/*
 * This module is for converting the .asc file movies into normalized arrays
 * that we can extract data from
 */
#ifndef IMAGE_ARR_H
#define IMAGE_ARR_H

#include<iostream>
#include<fstream>
#include<vector>

#define X 512
#define Y 512

float **build_bg_arr(std::string file, std::string temp);

bool populate_normal_arr(std::string file,
                         std::vector<float**> &arr, 
                         float** oxa,
                         float** instr,
                         float** gel);

#endif

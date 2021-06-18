/*****************************************************************************
    This program reads in the .asc value of the .sif movie and converts it into
    a 3D array. The passed particle initial conditions are then used to
    construct boxes of increasing sizes around the particle.
    By Ben Maloy
******************************************************************************/
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>

#define X 512
#define Y 512

// returns vector of array of ratios (floats) for each particle in the
// current spool
std::vector<float*> get_particle_ratios(std::vector<float**> &arr,
                                        std::vector<std::string> L_locs,
                                        std::vector<std::string> R_locs,
                                        int *num_slides);

void write_ratios_all_slides(std::vector<float*> all_data,
                             std::string file,
                             std::string temp,
                             int *num_slides);

float make_one_box(std::vector<float**> &arr,
                int slide,
                int x_null,
                int y_null,
                int w,
                int h);

/*
 * This file is for finding the ratios for specific particles
 */
#include "get_part_ratios.h"
#include <cmath>

// This function gets the ratios at the specified coordinates for
// the current spool
// arr contains the normalized movie for the curr spool
// side_spools contains the location of the particles in that spool
std::vector<float*> get_particle_ratios(std::vector<float**> &arr,
                                        std::vector<std::string> L_spools,
                                        std::vector<std::string> R_spools,
                                        int *num_slides)
{
    float tmp;

    // ratios through all slides in float* stored in vector for each spool
    std::vector<float*> all_parts_time;

    // make a 16x16 box around the particle
    int w = 16;
    int h = 16;

    int i = 0;

    for (std::string L_S : L_spools) {

        std::vector<int> x_L_vals, x_R_vals, y_L_vals, y_R_vals;
        int slide = 0; //initialize to zero, don't iterate it

        std::string R_S = R_spools[i];

        std::ifstream L_locs, R_locs;

        L_locs.open(L_S);
        R_locs.open(R_S);

        while (L_locs >> tmp) {

            // value will still be last slide even at EOF
            slide = tmp;

            if (!L_locs.good()) {
                break;
            }

            L_locs >> tmp;
            x_L_vals.push_back((int)tmp);

            L_locs >> tmp;
            y_L_vals.push_back((int)tmp);

            // slide numbers and size are the same for each side
            R_locs >> tmp;
            R_locs >> tmp;
            x_R_vals.push_back((int)tmp);

            R_locs >> tmp;
            y_R_vals.push_back((int)tmp);
        }

        num_slides[i] = x_L_vals.size();
        // get the initial slide; the slides are zero indexed
        slide = slide - (num_slides[i] - 1);
        
        //array of ratios through all slides (ratios in time)
        float *ratios_time = new float[num_slides[i]]();

        for(int j = 0; j < num_slides[i]; j++) {
            // make box around a quadrant of each side
            // assume the coordinate vals are the center of the particle
            float to_set_L = make_one_box(arr, slide + j, x_L_vals[j] - 8, y_L_vals[j] - 8, w, h);
            float to_set_R = make_one_box(arr, slide + j, x_R_vals[j] - 8, y_R_vals[j] - 8, w, h);

            // get the absolute value ***(assumes negative values are small)
            float tmp = std::abs(to_set_L) / std::abs(to_set_R);
            ratios_time[j] = tmp;
        }

        all_parts_time.push_back(ratios_time);

        i++;
    }

    return all_parts_time;
}

void write_ratios_all_slides(std::vector<float*> all_data,
                             std::string spool,
                             std::string temp,
                             int *num_slides)
{
    int num_parts = all_data.size();
    std::string location;

    for (int i = 0; i < num_parts; i++) {

        // change path as necessary
        location = "../../ratios/part_ratios/" + temp + "/"
                    + spool + "/part" + std::to_string(i);
        
        std::cout << location << std::endl;
        std::ofstream out;
        out.open(location, std::ios::out);
        // Choose the i_th particle
        float *part_brightness = all_data[i];

        for (int j = 0; j < num_slides[i]; j++) {
            out << part_brightness[j] << std::endl;
        }

        out.close();
    }
}

// Calculate the intensity of the given rectangle in the given slide
// x_null and y_null give the bottom left of the box when viewed with imageJ
float make_one_box(std::vector<float**> &arr,
                int slide,
                int x_null,
                int y_null,
                int w,
                int h)
{
    // arr is the 3d array of normalized pixel intensity values

    float val = 0.0;

    for (int j = 0; j < w; j++) {
        for (int k = 0; k < h; k++) {
            val += (arr[slide])[x_null + j][y_null + k];
        }
    }

    return val;
}

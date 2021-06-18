/*
 * This file is for finding the ratios over either quadrants of the image or
 * specific particles
 */
#include "get_sector_ratios.h"
#include <cmath>

// This function gets the ratios of quadrants of the current spool
std::vector<float*> get_sector_ratios(std::vector<float**> &arr,
                                      int num_slides)
{
    int h = 256;
    int w = 64;

    int x_L_vals[4] = {0, 64, 128, 192};
    int x_R_vals[4] = {256, 320, 384, 448};
    int y_vals[2] = {0, 256};

    std::vector<float*> all_parts_time;

    for(int i = 0; i < 4; i++) { //4 = number of x values
        for(int j = 0; j < 2; j++) { //2 = number of y values
            //make box around an octant of each side
            float *to_set_L = make_box(arr, num_slides, x_L_vals[i], y_vals[j], w, h);
            float *to_set_R = make_box(arr, num_slides, x_R_vals[i], y_vals[j], w, h);

            //array of ratios through all slides (ratios in time)
            float *ratios_time = new float[num_slides]();

            for (int k = 0; k < num_slides; k++) {
                // get the absolute value (assumes negative values are small)
                float tmp = std::abs(to_set_L[k]) / std::abs(to_set_R[k]);

                ratios_time[k] = tmp;
            }

            all_parts_time.push_back(ratios_time);
        }
    }

    return all_parts_time;
}

void write_ratios_all_slides(std::vector<float*> all_data,
                             std::string file,
                             std::string temp,
                             int num_slides)
{
    int num_parts = all_data.size();
    std::string location;

    for (int i = 0; i < num_parts; i++) {

        location = "../../ratios/sector_ratios/" + temp + "/"
                   + file + "/sector" + std::to_string(i);
        
        std::cout << location << std::endl;
        std::ofstream out;
        out.open(location, std::ios::out);
        float *part_brightness = all_data[i];

        for (int j = 0; j < num_slides; j++) {
            out << part_brightness[j] << std::endl;
        }

        out.close();
    }
}

// Calculate the intensity of the given rectangle through all slides
// x_null and y_null give the bottom left of the box when viewed with imageJ
float *make_box(std::vector<float**> &arr,
                int num_slides,
                int x_null,
                int y_null,
                int w,
                int h)
{
    // arr is the 3d array of normalized pixel intensity values
    float *val = new float[num_slides]();

    for (int i = 0; i < num_slides; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                val[i] += (arr[i])[x_null + j][y_null + k];
            }
        }
    }

    return val;
}

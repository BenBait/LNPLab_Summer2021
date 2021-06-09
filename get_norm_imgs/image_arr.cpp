#include"image_arr.h"
#include <iostream>
#include <fstream>

// This function is for averaging the movie with the background data and making
// it into a single 2D array that we can use for every slide of the data movie
float **build_bg_arr(std::string file, std::string temp)
{
    std::fstream infile;
    std::cout << file << std::endl;
    infile.open(file);

	if (!infile.is_open()) {
		std::cerr << "Error opening ascii file (bg)";
	}

    float **bg_arr = new float*[X]();
    for (int i = 0; i < X; i++)
        bg_arr[i] = new float[Y]();

    float tmp;
    int num_slides = 0;

    // time average the 3D array to get 2D bg array
    while (infile.good()) {
        for (int j = 0; j < X; j++) {
            infile >> tmp; // this is the row index
            for (int k = 0; k < Y; k++) {
                infile >> tmp;

                if (!infile.good())
                    break; // we reached the end or there was error

                num_slides++;
                bg_arr[j][k] += tmp;
            }
        }
    }

    infile.close();
    num_slides /= (512*512);

    for (int j = 0; j < X; j++) {
        for (int k = 0; k < Y; k++) {
            bg_arr[j][k] /= num_slides;
        }
    }

    return bg_arr;
}

//Put the ascii values in the 3d array, and then calibrates
bool populate_normal_arr(std::string file,
                         std::vector<float**> &arr, 
                         float** oxa,
                         float** instr,
                         float** gel)
{
    std::fstream infile;
    infile.open(file);

	if (!infile.is_open()) {
		std::cerr << "Error opening ascii file (populate normalized)";
        return false;
	}

    float tmp;
    //I(strumental), G(el), O(xazine) fluctuations
    float I, G, O;
    int num_slides = 0;

    while (infile.good()) { //for every index read, there is a 512x512 image

        float** slide = new float*[X]();
        for (int x = 0; x < X; x++)
           slide[x] = new float[Y]();

        for (int j = 0; j < X; j++) { 
            infile >> tmp; // this is the row index
            for (int k = 0; k < Y; k++) {
                infile >> tmp;

                if (!infile.good())
                    break; // we reached the end or there was error

                num_slides++;
                I = instr[j][k];
                G = gel[j][k];
                O = oxa[j][k];

                tmp = ((tmp - I) - (G - I))/(O - I);
                // this value should now only have the brightness due to the particle

                slide[j][k] = tmp;
            }
        }

        if (!infile.good()) // have to have the second or else we will read 512 one more time
            break; // we reached the end or there was error

        arr.push_back(slide);
    }

    num_slides /= (512*512);

    // Clear the memory to make sure nothing spooky happens
    for (int j = 0; j < X; j++) {
        delete [] oxa[j];
        delete [] instr[j];
        delete [] gel[j];
    }
    delete [] oxa;
    delete [] instr;
    delete [] gel;

    return true;
}

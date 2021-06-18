/*
 * This file is for finding the variation of the ratio as more frames and pixels
 * (from SECTORS of the normalized image) are included in the average ratio
 * value
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

float *get_each_var(float **ratios,
                     int m_p,
                     int num_sectors, 
                     int num_frames, 
                     float tot_ave_rat);
 
int main(int argc, char *argv[]) {

    // I usually comment 2 temps out and make an executable for each temperature
    // to parallelize a little bit
    std::vector<std::string> temps = {"25C", "35C", "45C"};
    
    int tot_slides = 200; // each spool has at least 200 slides
    int num_slides = 100; // we will average through 100 slides
    int num_sectors = 28; // num_sectors_per_image(4) * num_spools
    int i,j;

    for(std::string T: temps) {
        std::string ratios_parent = "../ratios/part_ratios" + T + "/";
        std::vector<std::string> files;
        for (i = 2; i < 9; i++) { //this give num_spools
            std::string name = "spool" + std::to_string(i) + "/";
            files.push_back(name);
        }

        // count the number of total particles/pixel sectors through all spools
        // and find the smallest number of slides for each spool
        float **all_ratios = new float*[num_sectors]();
        for (i = 0; i < num_sectors; i++) {
            all_ratios[i] = new float[tot_slides]();
        }

        int c = 0;
        float tot_ave_rat = 0;
        float through_time = 0;
        float tmp;
        std::vector<float> rats_to_end;
        for (std::string F : files) {

            // MAKE THIS A FOR EACH FILE IN SPOOL DIRECTORY LOOP
            for (i = 0; i < 4; i++) { // each spool is split into quadrants
				      // hence each spool folder has 4 files
                std::ifstream infile;
                std::string file_name = ratios_parent + F + "sector" + std::to_string(i);
                infile.open(file_name, std::ios::in);
                std::cout << file_name << std::endl;

                // we read the file backwards because the last frames will be
                // closer to equilibrium
                int count = 0;
		
                while (infile >> tmp) {

                    if (!infile.good())
                        break;

                    rats_to_end.insert(rats_to_end.begin(), tmp);
                    count++;
                }
		
                infile.close();

                through_time = 0;
                for (j = 0; j < tot_slides; j++) {
                    all_ratios[c][j] = rats_to_end[j];
                    through_time += all_ratios[c][j];
                }
                through_time /= tot_slides;
                tot_ave_rat += through_time;
                c++;

                rats_to_end.clear();
                rats_to_end.shrink_to_fit();
            }
        }

        tot_ave_rat /= num_sectors;

        // num_sizes is the number of averages we will take through time
        int num_sizes = num_slides / 4;

        float *dev = new float[num_sizes]();

        std::ofstream outfile;

        std::string filename2 = T + "err_space_and_frames.txt";
        outfile.open(filename2, std::ios::out);

        for(j = 0; j < num_sectors; j++){
            dev = get_each_var(all_ratios, j + 1, num_sectors, num_slides,
                                  tot_ave_rat);
            // The last entry of dev is the deviation for the average over all frames for
            // a particular m_p/j from the total average ratio
            // Write it to a file and plot it vs. j/number of parts
            for(int k = 0; k < num_sizes; k++) {
                    outfile << dev[k] << std::endl;
            }
            delete [] dev;
        }

        outfile.close();

        for (int i = 0; i < num_sectors; i++) {
            delete [] all_ratios[i];
        }
        delete [] all_ratios;
    }

    return 0;
}

// this function returns an array of average ratio values that are found through
// all numbers of slides (increasing by 4 in this case) for the specified number
// of particles m_p
float *get_each_var(float **ratios,
                 int m_p, //curr m_p for each round
                 int num_sectors, 
                 int num_slides, 
                 float tot_ave_rat)
{
    // increase by 4 frames every iteration
    int num_sizes = num_slides / 4;
    int i, j;

    int *sizes = new int[num_sizes]();
    for(i = 0, j = 1; i < num_sizes; i++, j+=4) {
        sizes[i] = j;
    }

    float *err = new float[num_sizes]();

    std::vector<int*> m_p_combs;

    std::string bitmask(m_p, 1); // K leading 1's
    bitmask.resize(num_sectors, 0); // N-K trailing 0's
    
    // print integers and permute bitmask
    do {
       int *curr_comb = new int[m_p]();
       int k = 0;
       for (i = 0; i < num_sectors; ++i) // [0..N-1] integers
       {
           if (bitmask[i]) {
              curr_comb[k] = i;
              k++;
           }
       }
       m_p_combs.push_back(curr_comb);
    } while(std::prev_permutation(bitmask.begin(),bitmask.end()));

    int nCk = m_p_combs.size();

    std::cout << "M_P: " << m_p << std::endl;

    int m_f, a; //counters
    int num_positions;
    float rat_ave; //values we calculate through each iter
    int end_frame;
    int b, c; 

    for(m_f = 0; m_f < num_sizes; m_f++) {

        num_positions = num_slides - sizes[m_f];

        for (int *curr_comb : m_p_combs){

            // average in time for all starting positions and m_p
            // particles (order given by 'part')
            for(a = 0; a < num_positions; a++) { 

                rat_ave = 0;
                // average through the specified number of particles in the order given
                // by 'particles', and through the specified frames (start to end_frame)
                
                // do a single RATIO measurement for a specific order of particles and
                // initial position for the average through time
                end_frame = a + sizes[m_f];
                for(b = 0; b < m_p; b++) {
                    for(c = a; c<=end_frame; c++) {
                        rat_ave += ratios[curr_comb[b]][c];
                    }
                }

                rat_ave /= m_p;
                rat_ave /= end_frame - a;

                //find the error for this number of frames
                //add the value for every order of the average through
                //space and initial position

                err[m_f] += std::abs(rat_ave - tot_ave_rat);
            }
        }

        err[m_f] /= num_positions;
        err[m_f] /= nCk;
    }

    for(i = 0; i < nCk; i++) {
        // so we can clear out the vector
        int *to_rem = m_p_combs[i];
        delete [] to_rem;
    }

    delete [] sizes;

    return err;
}


#include "get_part_ratios.h"

int main(int argc, char *argv[])
{
    char *norm_mov = argv[1];
    char *temp     = argv[2];
    char *spool    = argv[3];
    int  num_parts = std::stoi(argv[4]);

    std::vector<std::string> L_locs;
    std::vector<std::string> R_locs;

    for (int i = 0; i < num_parts; i++) {
        L_locs.push_back(argv[5 + (2*i)]);
        R_locs.push_back(argv[6 + (2*i)]);
    }

    std::fstream infile;
    infile.open(norm_mov);

    float val;
    std::vector<float**> arr;

    while(infile.good()) {
    
        float** slide = new float*[X]();
        for (int x = 0; x < X; x++)
            slide[x] = new float[Y]();

        for (int j = 0; j < X; j++) {
            for (int k = 0; k < Y; k++) {
                infile >> val;

                if (!infile.good())
                    break; // we reached the end or there was error

                slide[j][k] = val;
            }
        }

        if (!infile.good()) // have to have the second or else we will read 512 one more time
            break; // we reached the end or there was error

        arr.push_back(slide);
    }

    int *num_slides = new int[num_parts];

    std::vector<float*> all_ratios = get_particle_ratios(arr, L_locs, R_locs, num_slides);

    write_ratios_all_slides(all_ratios, spool, temp, num_slides);

    return 0;
}

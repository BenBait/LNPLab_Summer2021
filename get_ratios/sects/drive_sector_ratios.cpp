#include "get_sector_ratios.h"

int main (int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "PLEASE SPECIFY 1 ARGUMENT\n";
    }

    char *norm_mov   = argv[1];
    std::string temp = argv[2]; //so I can use substr() method
    char *spool      = argv[3];

    if (spool[6] == '.') { spool[6] = '\0'; }
    else { spool[7] = '\0'; }

    std::ifstream infile;
    infile.open(norm_mov);

    float tmp;

    std::vector<float**> arr;

    int num_slides = 0;

    while(infile.good()) {
    
        float** slide = new float*[X]();
        for (int x = 0; x < X; x++)
            slide[x] = new float[Y]();

        for (int j = 0; j < X; j++) {
            for (int k = 0; k < Y; k++) {
                infile >> tmp;

                if (!infile.good())
                    break; // we reached the end or there was error

                num_slides++;

                slide[j][k] = tmp;
            }
        }

        if (!infile.good()) // have to have the second or else we will read 512 one more time
            break; // we reached the end or there was error

        arr.push_back(slide);
    }

    // number of slides is just the number of slides in the file
    // compare to particle ratios, where the number of slides is the
    // number of slides with particles in them
    num_slides /= (512*512);

    std::vector<float*> all_ratios = get_sector_ratios(arr, num_slides);

    write_ratios_all_slides(all_ratios, spool, temp, num_slides);

    return 0;
}

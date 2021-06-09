#include "image_arr.h"

int main(int argc, char *argv[]){

    if (argc < 6) {
        std::cerr << "PLEASE SPECIFY 5 ARGUMENTS\n";
    }

    char *movie      = argv[1];
    std::string oxa_file = argv[2]; //so I can use substr() method
    char *instr_file = argv[3];
    char *gel_file   = argv[4];
    char *spool      = argv[5];

    std::string temp = oxa_file.substr(25, 3);

    if (spool[6] == '.') { spool[6] = '\0'; }
    else { spool[7] = '\0'; }

    // Average the 3 fluctuation files through time to get one 2D array that
    // has the normalization data for each pixel
    float **gel   = build_bg_arr(gel_file, temp);
    std::cout << "GEL\n";
    float **oxa   = build_bg_arr(oxa_file, temp);
    std::cout << "OXA\n";
    float **instr = build_bg_arr(instr_file, temp);
    std::cout << "INSTR\n";

    std::vector<float**> arr;

    if(!populate_normal_arr(movie, arr, oxa, instr, gel)) {
        std::cerr << "Normalized arr not populated\n";
        return 1;
    }
    
    // Now write the normalized array to a new file
    std::string loc = "../norm_movies/" + temp + "/" + spool + ".asc";
    std::cout << loc << std::endl;

    std::ofstream out;
    out.open(loc, std::ios::out);

    int num_slides = arr.size();

    std::cout << num_slides << std::endl;

    for (int i = 0; i < num_slides; i++) {
        for (int j = 0; j < 512; j++) {
            for (int k = 0; k < 512; k++) {
                out << (arr[i])[j][k] << std::endl;
            }
        }
    }

    out.close();

    return 0;
}

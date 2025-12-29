#include "preprocess.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <code_file> <out_file>" << std::endl;
        return 1;
    }
    std::string code_file = argv[1];
    std::string out_file = argv[2];
    std::ifstream infile(code_file);
    if (!infile) {
        std::cerr << "Error opening input file: " << code_file << std::endl;
        return 1;
    }
    std::string code((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();
    std::string processed_code = preprocess_all(code);
    std::ofstream outfile(out_file);
    if (!outfile) {
        std::cerr << "Error opening output file: " << out_file << std::endl;
        return 1;
    }
    outfile << processed_code;
    outfile.close();
    return 0;
}
    
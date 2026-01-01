#include "preprocess.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>      // std::tmpnam, std::remove
#include <cstdlib>     // std::system
#include "vars.h"

using std::vector;

template <typename T>
bool in(T var, vector<T> arr) {
    for (T i : arr) {
        if(var == i) return true;
    } return false;
}

int main(int argc, char* argv[]) {
    vector range_1_4 = {1,2,3,4};
    bool in_valid_range = in<int>(argc, range_1_4);
    if (!(in_valid_range)) {
        std::cerr
            << "Usage:\n"
            << argv[0] << " <code_file> <out_file>\n\nor\n\n"
            << argv[0] << " <code_file> -e <interpreter>\n";
        return 1;
    }
    if ((argc == 2 && argv[1] == "-v") || argc == 1) {
        std::cout << "RePy version " << version << "\n";
        return 0;
    }
    // --- leer archivo de entrada ---
    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Error opening input file: " << argv[1] << "\n";
        return 1;
    }

    std::string code(
        (std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>()
    );
    infile.close();

    // --- preprocesar ---
    std::string processed_code = preprocess_all(code);

    // --- modo ejecución ---
    if (argc == 4 && std::string(argv[2]) == "-e") {
        std::string interpreter = argv[3];

        // crear archivo temporal
        char tmp_name[L_tmpnam];
        std::tmpnam(tmp_name);

        // escribir código procesado
        std::ofstream tmp(tmp_name);
        if (!tmp) {
            std::cerr << "Error creating temporary file\n";
            return 1;
        }
        tmp << processed_code;
        tmp.close();

        // ejecutar intérprete
        std::string cmd = interpreter + " " + tmp_name;
        int result = std::system(cmd.c_str());

        // limpiar
        std::remove(tmp_name);

        return result;
    }

    // --- modo salida a archivo ---
    std::ofstream outfile(argv[2]);
    if (!outfile) {
        std::cerr << "Error opening output file: " << argv[2] << "\n";
        return 1;
    }

    outfile << processed_code;
    outfile.close();

    return 0;
}

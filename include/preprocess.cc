#include "preprocess.hpp"
#include <iostream>
#include <string>
#include <regex>

std::string preprocess_regex_multilineliterals(std::string& code) {
    std::regex backtick(R"(```([\s\S]*?)```)");
    std::string replacement = "reCompile(r\"\"\"$1\"\"\")";
    return std::regex_replace(code, backtick, replacement);
}

std::string preprocess_regex_literals(std::string& code) {
    std::regex backtick(R"(`(.*?)`)");
    std::string replacement = "reCompile(r\"$1\")";
    return std::regex_replace(code, backtick, replacement);
}

std::string preprocess_all(const std::string& code) {
    std::string out = code;

    // Primero multilínea
    out = preprocess_regex_multilineliterals(out);

    // Luego simples
    out = preprocess_regex_literals(out);

    // Agregar import si aparece reCompile
    if (out.find("reCompile") != std::string::npos) {
        out = "from re import compile as reCompile\n" + out;
    }

    return out;
}
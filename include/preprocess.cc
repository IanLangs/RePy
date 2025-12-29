#include "preprocess.hpp"
#include <iostream>
#include <string>
#include <regex>

#include <string>
#include <vector>
#include <cctype>

std::vector<std::string> split_python_docstring(const std::string& src) {
    size_t i = 0;
    const size_t n = src.size();

    // 1. Saltar whitespace inicial
    while (i < n && std::isspace(static_cast<unsigned char>(src[i]))) {
        i++;
    }

    // 2. Detectar triple comilla
    std::string quote;
    if (i + 2 < n && src.compare(i, 3, R"(""")") == 0) {
        quote = R"(""")";
    } else if (i + 2 < n && src.compare(i, 3, "'''") == 0) {
        quote = "'''";
    } else {
        // No hay docstring
        return { "", src };
    }

    // 3. Extraer docstring
    size_t start = i + 3;
    size_t end = src.find(quote, start);
    if (end == std::string::npos) {
        // Docstring mal cerrado → tratamos como no-docstring
        return { "", src };
    }

    std::string doc = src.substr(start, end - start);

    // 4. El resto del código
    size_t rest_start = end + 3;
    std::string rest = src.substr(rest_start);

    return { doc, rest };
}


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
    if (out.find("reCompile") != std::string::npos && out.find("\"\"\"") != 0) {
        out = "from re import compile as reCompile\n" + out;
    } else if (out.find("reCompile") != std::string::npos && out.find("\"\"\"") == 0) {
        auto vdr = split_python_docstring(out);
        std::string docstring = vdr[0];
        std::string rest = vdr[1];
        out = "\"\"\"" + docstring + "\"\"\"" + "\nfrom re import compile as reCompile\n" + rest;
    }

    return out;
}
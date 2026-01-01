#include "types.hpp"

#include <unordered_map>
#include <sstream>
#include <iostream>
#include <regex>

namespace repy {

std::string to_string(PyType t) {
    switch (t) {
        case PyType::Number: return "number";
        case PyType::Bool:   return "bool";
        case PyType::String: return "str";
        case PyType::List:   return "list";
        case PyType::Tuple:  return "tuple";
        case PyType::Dict:   return "dict";
        default:             return "unknown";
    }
}

static PyType from_annotation(const std::string& t) {
    if (t == "int" || t == "float") return PyType::Number;
    if (t == "bool")  return PyType::Bool;
    if (t == "str")   return PyType::String;
    if (t == "list")  return PyType::List;
    if (t == "tuple") return PyType::Tuple;
    if (t == "dict")  return PyType::Dict;
    return PyType::Unknown;
}

static PyType infer_type(const std::string& value) {
    std::string v = value;
    v.erase(0, v.find_first_not_of(" \t"));
    v.erase(v.find_last_not_of(" \t") + 1);

    if (v == "True" || v == "False") return PyType::Bool;
    if (v.size() >= 2 && v.front() == '"' && v.back() == '"') return PyType::String;
    if (v.front() == '[' && v.back() == ']') return PyType::List;
    if (v.front() == '(' && v.back() == ')') return PyType::Tuple;
    if (v.front() == '{' && v.back() == '}') return PyType::Dict;

    static std::regex num(R"(^-?\d+(\.\d+)?$)");
    if (std::regex_match(v, num)) return PyType::Number;

    return PyType::Unknown;
}

bool verify_python_types(const std::string& code) {
    std::unordered_map<std::string, PyType> vars;
    std::istringstream input(code);
    std::string line;
    int lineno = 0;

    static std::regex typed_assign(
        R"(^\s*([a-zA-Z_]\w*)\s*:\s*([a-zA-Z_]\w*)\s*=\s*(.+)$)"
    );

    static std::regex assign(
        R"(^\s*([a-zA-Z_]\w*)\s*=\s*(.+)$)"
    );

    while (std::getline(input, line)) {
        lineno++;

        auto pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);

        std::smatch m;
        std::string var;
        PyType t = PyType::Unknown;

        if (std::regex_match(line, m, typed_assign)) {
            var = m[1];
            std::string ann = m[2];
            std::string val = m[3];

            t = from_annotation(ann);
            if (t == PyType::Unknown)
                continue;

            PyType inferred = infer_type(val);
            if (inferred != PyType::Unknown && inferred != t) {
                std::cerr
                    << "TypeError (line " << lineno << "): variable '"
                    << var << "' annotated as '" << to_string(t)
                    << "' but assigned '" << to_string(inferred) << "'\n";
                return false;
            }
        }
        else if (std::regex_match(line, m, assign)) {
            var = m[1];
            t = infer_type(m[2]);
            if (t == PyType::Unknown)
                continue;
        }
        else {
            continue;
        }

        if (!vars.count(var)) {
            vars[var] = t;
        }
        else if (vars[var] != t) {
            std::cerr
                << "TypeError (line " << lineno << "): variable '"
                << var << "' was '" << to_string(vars[var])
                << "' but now assigned '" << to_string(t) << "'\n";
            return false;
        }
    }

    return true;
}

} // namespace repy

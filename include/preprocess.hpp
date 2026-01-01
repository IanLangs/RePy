#pragma once
#include <iostream>
#include <string>
#include <regex>

namespace repy {
std::string preprocess_regex_multilineliterals(std::string& code);
std::string preprocess_regex_literals(std::string& code);
std::string preprocess_all(const std::string& code);
}
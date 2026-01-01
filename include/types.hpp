#pragma once

#include <string>

namespace repy {

enum class PyType {
    Number,
    Bool,
    String,
    List,
    Tuple,
    Dict,
    Unknown
};

std::string to_string(PyType t);

/**
 * Verifica tipos en código Python estilo RePy.
 * Soporta:
 *   x = 7
 *   x: int = 3
 *   x = "hola"  -> error si x era number
 *
 * Devuelve true si no hay errores.
 * En caso de error imprime TypeError y devuelve false.
 */
bool verify_python_types(const std::string& code);

} // namespace repy

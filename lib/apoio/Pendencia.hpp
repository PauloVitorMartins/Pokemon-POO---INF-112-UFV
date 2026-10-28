#ifndef PENDENCIA_HPP
#define PENDENCIA_HPP
#include <stdexcept>
#include <string>
// Apoio fornecido: identifica um método ainda vazio para a interface mostrar a pendência.
// Ao implementar um método, substitua seu TODO e o throw pela sua lógica.
class FuncaoNaoImplementada : public std::logic_error {
public:
    explicit FuncaoNaoImplementada(const std::string& metodo) : std::logic_error(metodo) {}
};
#endif

#include "Utilidades.hpp"

std::vector<std::string> decodificadorDados(char* dados, int tamanho) {

    std::vector<std::string> retorno;

    retorno.push_back("");

    for (int i = 0; i < tamanho; i++) {

        if (dados[i] == ',') {

            retorno.push_back("");

        }
        else {

            retorno[retorno.size() - 1] += dados[i];

        }

    }

    return retorno;

}
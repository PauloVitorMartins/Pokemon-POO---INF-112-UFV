#ifndef BATALHA_HPP
#define BATALHA_HPP

#include <string>
#include "Treinador.hpp"

using std::string;

class Batalha {
    Treinador jogadores[2];
    int turno;
    string mensagem;
    void encerrarAcao();

public:
    Batalha(string nome1, int equipe1[3], string nome2, int equipe2[3]);
    Treinador& getTreinador(int indice);

    int getTurno() const;
    string getMensagem() const;
    bool terminou() const;
    bool precisaTrocar() const;

    bool atacar(int golpe);
    bool trocar(int pokemon);
    bool usarItem(int item, int alvo);
};

#endif

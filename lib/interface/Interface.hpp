#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

class Batalha;

enum TipoAcao {
    SEM_ACAO,
    INICIAR_BATALHA,
    ATACAR,
    TROCAR,
    USAR_ITEM,
    ENCERRAR_BATALHA
};

struct Acao {
    TipoAcao tipo = SEM_ACAO;
    int indice = 0;
    int alvo = 0;
};

bool iniciarInterface();
bool interfaceAberta();
Acao atualizarInterface(Batalha* batalha);
void fecharInterface();

std::string getNomeEscolhido(int jogador);
int* getEquipeEscolhida(int jogador);
void mostrarCombate();
void mostrarResultado(bool sucesso);

#endif

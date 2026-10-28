#if MODELO_DISPONIVEL
#include "Batalha.hpp"
#include "Golpe.hpp"
#include "Pokemon.hpp"
#include "Item.hpp"
#include "Treinador.hpp"

Batalha::Batalha(string nome1, int equipe1[3], string nome2, int equipe2[3])
    : jogadores{{nome1, equipe1}, {nome2, equipe2}} {
    // Só inicia o combate quando os treinadores fornecerem seus Pokémon.
    this->turno = jogadores[0].getAtivo() && jogadores[1].getAtivo() ? 0 : -1;
    string nome = jogadores[0].getNome();
    if (turno == 0 && !nome.empty()) {
        this->mensagem = nome + ": escolha uma acao.";
    }
}

Treinador& Batalha::getTreinador(int indice) {
    return jogadores[indice];
}

int Batalha::getTurno() const {
    return turno;
}

string Batalha::getMensagem() const {
    return mensagem;
}

bool Batalha::terminou() const {
    if (!jogadores[0].getAtivo() || !jogadores[1].getAtivo()) return false;
    return jogadores[0].perdeu() || jogadores[1].perdeu();
}

bool Batalha::precisaTrocar() const {
    if (turno < 0 || turno > 1) return false;
    Pokemon* ativo = jogadores[turno].getAtivo();
    return ativo && !terminou() && ativo->derrotado();
}

void Batalha::encerrarAcao() {
    if (terminou()) {
        mensagem += " " + jogadores[turno].getNome() + " venceu!";
        return;
    }
    turno = 1-turno;
    if (precisaTrocar()) {
        mensagem += " Escolha outro Pokemon para continuar.";
    }
}

bool Batalha::atacar(int indice) {
    if (turno < 0 || turno > 1 || terminou() || precisaTrocar()) {
        return false;
    }

    Pokemon* atacante = jogadores[turno].getAtivo();
    Pokemon* defensor = jogadores[1-turno].getAtivo();
    if (!atacante || !defensor) return false;
    Golpe* golpe = atacante->getGolpe(indice);

    if (golpe==nullptr) {
        return false;
    }

    mensagem = golpe->aplicar(*atacante, *defensor);

    if (defensor->derrotado()) {
        mensagem += " " + defensor->getNome() + " foi derrotado.";
    }

    encerrarAcao();
    return true;
}

bool Batalha::trocar(int indice) {
    if (turno < 0 || turno > 1 || terminou()) {
        return false;
    }

    bool obrigatoria = precisaTrocar();

    if (!jogadores[turno].trocar(indice)) {
        mensagem = "Escolha outro Pokemon que esteja vivo.";
        return false;
    }

    mensagem = jogadores[turno].getAtivo()->getNome()+" entrou na batalha.";

    if (!obrigatoria) {
        encerrarAcao();
    }

    return true;
}

bool Batalha::usarItem(int indice, int indiceAlvo) {
    if (turno < 0 || turno > 1 || terminou() || precisaTrocar()) {
        return false;
    }

    Pokemon* alvo = jogadores[turno].getPokemon(indiceAlvo);

    if (alvo==nullptr) {
        return false;
    }

    int vidaAntes = alvo->getVida();
    Item* item = jogadores[turno].getItem(indice);
    bool fortificante = item && item->getNome() == "Fortificante";

    if (!jogadores[turno].usarItem(indice, indiceAlvo)) {
        mensagem = "Item indisponivel ou alvo invalido.";
        return false;
    }

    if (fortificante) {
        mensagem = alvo->getNome()+": próximo golpe de dano fortalecido em 10%!";
    } else {
        mensagem = alvo->getNome()+" recuperou "+std::to_string(alvo->getVida()-vidaAntes)+" PV.";
    }

    encerrarAcao();
    return true;
}

#endif

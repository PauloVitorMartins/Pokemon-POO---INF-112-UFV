#if MODELO_DISPONIVEL
#include "Batalha.hpp"
#endif
#include "Interface.hpp"
#include "Pendencia.hpp"

// Integração fornecida: os alunos criam as classes chamadas aqui.
int main() {
    if (!iniciarInterface()) return 1;
    Batalha* batalha = nullptr;

    while (interfaceAberta()) {
        Acao acao = atualizarInterface(batalha);
        try {
            if (acao.tipo == INICIAR_BATALHA) {
                // A arena pode mostrar silhuetas mesmo se a construção estiver pendente.
                mostrarCombate();
#if MODELO_DISPONIVEL
                Batalha* nova = new Batalha(
                    getNomeEscolhido(0), getEquipeEscolhida(0),
                    getNomeEscolhido(1), getEquipeEscolhida(1)
                );
                delete batalha;
                batalha = nova;
#endif
            }
            else if (acao.tipo == ENCERRAR_BATALHA) {
#if MODELO_DISPONIVEL
                delete batalha;
#endif
                batalha = nullptr;
            }
            else if (acao.tipo == ATACAR || acao.tipo == TROCAR || acao.tipo == USAR_ITEM) {
#if MODELO_DISPONIVEL
                if (batalha && !batalha->terminou()) {
                    bool sucesso = false;
                    if (acao.tipo == ATACAR) sucesso = batalha->atacar(acao.indice);
                    else if (acao.tipo == TROCAR) sucesso = batalha->trocar(acao.indice);
                    else sucesso = batalha->usarItem(acao.indice, acao.alvo);
                    mostrarResultado(sucesso);
                }
#endif
            }
        } catch (const FuncaoNaoImplementada&) {
            // Mantém a interface aberta enquanto o método ainda estiver com TODO.
        }
    }

#if MODELO_DISPONIVEL
    delete batalha;
#endif
    fecharInterface();
    return 0;
}

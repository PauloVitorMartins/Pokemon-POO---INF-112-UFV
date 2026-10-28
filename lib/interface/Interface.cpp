#include "Interface.hpp"
#include "Pendencia.hpp"
#include "raylib.h"
#if MODELO_DISPONIVEL
#include "Batalha.hpp"
#include "Pokemon.hpp"
#include "Golpe.hpp"
#include "Item.hpp"
#else
class Treinador;
class Pokemon;
class Golpe;
class Item;
#endif
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>

namespace {
using std::string;
// Estados da interface: indicam qual tela será desenhada e receberá comandos.
enum Tela { INICIO, TREINADORES, EQUIPE, COMBATE, VITORIA };
const Color INK={27,42,58,255}, PAPER={243,240,219,255}, TEAL={37,105,111,255}, AMBAR={244,194,86,255};
const char* nomes[]={"Charmander","Bulbasaur","Squirtle","Pikachu","Vulpix","Oddish","Psyduck","Voltorb","Ponyta","Growlithe","Bellsprout","Magnemite"};
const int TOTAL_POKEMON=sizeof(nomes)/sizeof(nomes[0]);
const char* tipos[]={"FOGO","PLANTA","ÁGUA","ELÉTRICO","FOGO","PLANTA","ÁGUA","ELÉTRICO","FOGO","FOGO","PLANTA","ELÉTRICO"};
const int ataquesCatalogo[]={22,18,17,25,19,17,22,20,24,21,23,19};
const int defesasCatalogo[]={14,18,22,12,17,19,16,18,13,16,14,23};
const char* treinadores[]={"Treinador 1","Treinador 2","Treinador 3","Treinador 4"};
Texture2D retratos[4]={},campo={},logo={},charizardTitulo={},textoTitulo={},chamasTitulo={};
Texture2D cura={},fortificante={},pokebola={};
const Color cores[]={{224,91,73,255},{84,153,124,255},{80,129,195,255},{173,104,173,255}};
bool som=true;
bool bloqueado=false;
// A navegação usa os controles desenhados no quadro anterior.
struct ControleTeclado { Rectangle area; bool habilitado; };
std::vector<ControleTeclado> controlesAnteriores, controlesAtuais;
Rectangle focoTeclado={};
bool usandoTeclado=false, confirmarTeclado=false, ativacaoControle=false;
int contextoTeclado=-1;
Vector2 ultimoMouse={};
bool mesmaArea(Rectangle a,Rectangle b) {
    return a.x==b.x && a.y==b.y && a.width==b.width && a.height==b.height;
}
bool controleInterativo(Rectangle r,bool habilitado) {
    controlesAtuais.push_back(ControleTeclado{r,habilitado});
    bool mouse=habilitado && CheckCollisionPointRec(GetMousePosition(),r);
    if(mouse && !usandoTeclado) focoTeclado=r;
    if(usandoTeclado && focoTeclado.width==0 && habilitado) focoTeclado=r;
    bool foco=habilitado && usandoTeclado && mesmaArea(r,focoTeclado);
    ativacaoControle=habilitado && ((mouse && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || (foco && confirmarTeclado));
    if(ativacaoControle) confirmarTeclado=false;
    if(foco) DrawRectangleRoundedLinesEx(Rectangle{r.x-3,r.y-3,r.width+6,r.height+6},.15f,4,2,AMBAR);
    return (mouse && !usandoTeclado) || foco;
}
void navegarTeclado(int contexto,bool digitando) {
    controlesAtuais.clear();
    confirmarTeclado=false;
    if(contexto!=contextoTeclado) {
        controlesAnteriores.clear(); focoTeclado=Rectangle{}; contextoTeclado=contexto;
    }
    Vector2 mouse=GetMousePosition();
    if(mouse.x!=ultimoMouse.x || mouse.y!=ultimoMouse.y || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) usandoTeclado=false;
    ultimoMouse=mouse;
    if(digitando) return;
    int dx=0,dy=0;
    if(IsKeyPressed(KEY_LEFT)) dx=-1;
    if(IsKeyPressed(KEY_RIGHT)) dx=1;
    if(IsKeyPressed(KEY_UP)) dy=-1;
    if(IsKeyPressed(KEY_DOWN)) dy=1;
    bool tab=IsKeyPressed(KEY_TAB);
    bool enter=IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE);
    if(dx || dy || tab || enter) usandoTeclado=true;
    int atual=-1;
    for(int i=0;i<(int)controlesAnteriores.size();i++)
        if(controlesAnteriores[i].habilitado && mesmaArea(controlesAnteriores[i].area,focoTeclado)) atual=i;
    if(atual<0) {
        for(int i=0;i<(int)controlesAnteriores.size();i++) if(controlesAnteriores[i].habilitado) { atual=i; focoTeclado=controlesAnteriores[i].area; break; }
    } else if(tab) {
        int passo=(IsKeyDown(KEY_LEFT_SHIFT)||IsKeyDown(KEY_RIGHT_SHIFT))?-1:1;
        int n=(int)controlesAnteriores.size();
        for(int k=1;k<=n;k++) {
            int i=(atual+passo*k+n)%n;
            if(controlesAnteriores[i].habilitado) { focoTeclado=controlesAnteriores[i].area; break; }
        }
    } else if(dx || dy) {
        Rectangle origem=focoTeclado;
        float melhor=1e9f;
        for(const auto& c:controlesAnteriores) if(c.habilitado) {
            float x=c.area.x+c.area.width/2-origem.x-origem.width/2;
            float y=c.area.y+c.area.height/2-origem.y-origem.height/2;
            float frente=dx*x+dy*y, lateral=std::fabs(dx*y-dy*x);
            if(frente>1 && frente>=lateral*.7f && frente+lateral*.3f<melhor) { melhor=frente+lateral*.3f; focoTeclado=c.area; }
        }
    }
    confirmarTeclado=enter;
}
Font fonteInterface={};
int larguraTexto(const char* s,int tamanho) {
    return (int)MeasureTextEx(fonteInterface,s,(float)tamanho,0).x;
}
void texto(const string& s,int x,int y,int tam,Color cor=INK) {
    DrawTextEx(fonteInterface,s.c_str(),Vector2{(float)x,(float)y},(float)tam,0,cor);
}
// Quebra as mensagens do modelo em linhas dentro da caixa disponível.
void textoEmCaixa(const string& mensagem,int x,int y,int largura,int altura,int tamanho,Color cor) {
    std::vector<string> linhas;
    do {
        linhas.clear();
        std::istringstream palavras(mensagem);
        string palavra, linha;
        while(palavras >> palavra) {
            string tentativa=linha.empty()?palavra:linha+" "+palavra;
            if(!linha.empty() && larguraTexto(tentativa.c_str(),tamanho)>largura) {
                linhas.push_back(linha);
                linha=palavra;
            } else linha=tentativa;
        }
        if(!linha.empty()) linhas.push_back(linha);
        if((int)linhas.size()*(tamanho+5)-5<=altura || tamanho<=10) break;
        tamanho--;
    } while(true);
    BeginScissorMode(x,y,largura,altura);
    for(const auto& linha:linhas) {
        texto(linha,x,y,tamanho,cor);
        y+=tamanho+5;
    }
    EndScissorMode();
}
void centro(const string& s,int y,int tam,Color cor=INK) { texto(s,(960-larguraTexto(s.c_str(),tam))/2,y,tam,cor); }
bool botao(Rectangle r,const string& s,bool habilitado=true,bool marcado=false) {
    bool hover=controleInterativo(r,habilitado && !bloqueado);
    Color cor=marcado?TEAL:(hover?AMBAR:PAPER);
    if(!habilitado) cor=Color{175,184,183,255};
    DrawRectangleRounded(r,0.18f,4,cor);
    DrawRectangleRoundedLinesEx(r,0.18f,4,2,INK);
    int tam=18;
    while(tam>12 && larguraTexto(s.c_str(),tam)>r.width-16) tam--;
    // Centraliza a parte visível das letras, incluindo os acentos.
    float topo=(float)fonteInterface.baseSize, base=0;
    for(const char* letra=s.c_str();*letra;) {
        int bytes=0;
        int codigo=GetCodepointNext(letra,&bytes);
        int glifo=GetGlyphIndex(fonteInterface,codigo);
        if(codigo!=' ') {
            topo=std::min(topo,(float)fonteInterface.glyphs[glifo].offsetY);
            base=std::max(base,fonteInterface.glyphs[glifo].offsetY+fonteInterface.recs[glifo].height);
        }
        letra+=bytes;
    }
    float escala=(float)tam/fonteInterface.baseSize;
    int y=(int)std::round(r.y+(r.height-(base-topo)*escala)/2-topo*escala);
    texto(s,(int)std::round(r.x+(r.width-larguraTexto(s.c_str(),tam))/2),y,tam,marcado?WHITE:INK);
    return ativacaoControle;
}
// Molduras e cursor em pixels para a caixa de ações da batalha.
void molduraBatalha(int x,int y,int largura,int altura,Color preenchimento) {
    DrawRectangle(x,y,largura,altura,INK);
    DrawRectangle(x+4,y+4,largura-8,altura-8,Color{236,224,182,255});
    DrawRectangle(x+8,y+8,largura-16,altura-16,Color{102,118,135,255});
    DrawRectangle(x+12,y+12,largura-24,altura-24,preenchimento);
}
bool opcaoBatalha(Rectangle r,const string& rotulo,bool selecionada=false,bool habilitado=true,const string& detalhe="") {
    bool hover=controleInterativo(r,habilitado && !bloqueado);
    if (hover) DrawRectangle((int)r.x,(int)r.y,(int)r.width,(int)r.height,Color{231,231,210,255});
    if (hover || (selecionada && habilitado)) {
        int x=(int)r.x+8,y=(int)(r.y+r.height/2)-10;
        for(int i=0;i<10;i++) DrawRectangle(x+i,y+i,1,20-2*i,INK);
    }
    int tamanho=24;
    while(tamanho>16 && larguraTexto(rotulo.c_str(),tamanho)>r.width-32) tamanho--;
    Color cor=habilitado?INK:Color{128,135,134,255};
    int y=detalhe.empty()?(int)(r.y+(r.height-tamanho)/2):(int)r.y+9;
    texto(rotulo,(int)r.x+28,y,tamanho,cor);
    if(!detalhe.empty()) texto(detalhe,(int)r.x+28,(int)r.y+42,12,cor);
    return ativacaoControle;
}
Rectangle posicaoOpcao(int indice) {
    return Rectangle{562.f+(indice%2)*192,466.f+(indice/2)*82,186,66};
}

void celebracaoVitoria(float tempo) {
    // Queda contínua, com balanço, rotação e pequenas viradas de cada papel.
    const Color paleta[]={AMBAR,TEAL,Color{224,131,115,255},Color{244,204,126,255}};
    for(int i=0;i<56;i++) {
        float velocidade=42.f+(i*17)%47;
        float y=std::fmod(tempo*velocidade+(i*173)%700,700.f)-30;
        float fase=tempo*(1.1f+(i%4)*.16f)+i*1.9f;
        float x=16.f+(i*137)%928+std::sin(fase)*(12+i%5*4);
        float giro=i*29.f+tempo*(20+i%6*11)+18*std::sin(fase*.7f);
        float largura=(4.f+i%3)*(.25f+.75f*std::fabs(std::cos(fase)));
        float altura=3.f+i%2;
        float entrada=std::max(0.f,std::min(1.f,(y+20)/45));
        float saida=std::max(0.f,std::min(1.f,(660-y)/90));
        Color cor=Fade(paleta[i%4],(.48f+.08f*(i%3))*entrada*saida);
        DrawRectanglePro(Rectangle{x,y,largura,altura},Vector2{largura/2,altura/2},giro,cor);
    }
}

void fundo(Tela tela) {
    switch(tela) {
        case INICIO:
            ClearBackground(Color{222,235,249,255});
            DrawRectangle(0,0,960,224,Color{188,210,235,255});
            DrawRectangle(0,224,960,4,Color{93,126,166,255});
            DrawRectangle(0,560,960,80,Color{188,210,235,255});
            break;
        case TREINADORES:
            ClearBackground(Color{222,235,249,255});
            DrawRectangle(0,0,960,116,Color{188,210,235,255});
            DrawRectangle(0,116,960,4,Color{93,126,166,255});
            DrawRectangle(0,514,960,126,Color{188,210,235,255});
            break;
        case EQUIPE:
            ClearBackground(Color{213,232,218,255});
            for(int y=112;y<480;y+=16) DrawRectangle(0,y,960,8,Color{204,225,210,255});
            DrawRectangle(0,0,960,106,Color{174,204,184,255});
            DrawRectangle(0,520,960,120,Color{174,204,184,255});
            break;
        case VITORIA:
            ClearBackground(Color{251,248,232,255});
            DrawRectangleGradientV(0,0,960,640,Color{253,250,237,255},Color{240,230,193,255});
            DrawRectangle(0,0,960,5,AMBAR);
            DrawRectangle(0,619,960,21,Fade(TEAL,.06f));
            break;
        case COMBATE:
            ClearBackground(INK);
            // O campo original e desenhado exclusivamente na tela de combate.
            break;
    }
}
void retrato(int id,int x,int y,int escala=4) {
    float fator=escala*.5f;
    DrawTextureEx(retratos[id],Vector2{(float)x-16,(float)y-4},0,fator,WHITE);
}
// Leituras seguras para a apresentação. Não executam regras nem criam objetos de domínio.
template<class T, class Consulta>
void consultar(Consulta consulta, T& valor, bool* disponivel=nullptr) {
    try {
        valor = consulta();
        if (disponivel) *disponivel = true;
    } catch (const FuncaoNaoImplementada&) {
        if (disponivel) *disponivel = false;
    }
}

struct GolpeVisual {
    Golpe* objeto = nullptr;
    string nome;
    void ler(Golpe* g) {
        objeto = g;
#if MODELO_DISPONIVEL
        if (g) consultar([&] { return g->getNome(); }, nome);
#endif
    }
    string getNome() const {
        if(nome=="Jato de Agua") return "Jato de Água";
        return nome.empty()?"??":nome;
    }
};
struct ItemVisual {
    Item* objeto = nullptr;
    string nome;
    void ler(Item* i) {
        objeto = i;
#if MODELO_DISPONIVEL
        if (i) consultar([&] { return i->getNome(); }, nome);
#endif
    }
    string getNome() const {
        return nome.empty()?"??":nome;
    }
};
struct PokemonVisual {
    Pokemon* objeto = nullptr;
    string nome, tipo;
    int idPokemon = -1, vida = 0, maxima = 0, ataque = 0, defesa = 0;
    bool temAtaque = false, temDefesa = false;
    bool temIdPokemon = false, temVida = false, temMaxima = false, morto = false;
    GolpeVisual golpes[2];
    bool golpeConsultado[2] = {};
    void ler(Pokemon* p) {
        objeto = p;
#if MODELO_DISPONIVEL
        if (!p) return;
        consultar([&] { return p->getNome(); }, nome);
        consultar([&] { return p->getTipo(); }, tipo);
        consultar([&] { return p->getIdPokemon(); }, idPokemon, &temIdPokemon);
        temIdPokemon = temIdPokemon && idPokemon >= 0 && idPokemon < TOTAL_POKEMON;
        consultar([&] { return p->getAtaque(); }, ataque, &temAtaque);
        consultar([&] { return p->getDefesa(); }, defesa, &temDefesa);
        consultar([&] { return p->getVida(); }, vida, &temVida);
        consultar([&] { return p->getVidaMaxima(); }, maxima, &temMaxima);
        temMaxima = temMaxima && maxima > 0;
        consultar([&] { return p->derrotado(); }, morto);
        for (int i = 0; i < 2; i++) {
            Golpe* g = nullptr;
            consultar([&] { return p->getGolpe(i); }, g, &golpeConsultado[i]);
            golpes[i].ler(g);
        }
#endif
    }
    string getNome() const { return nome.empty()?"??":nome; }
    string getTipo() const {
        if(tipo=="agua") return "água";
        if(tipo=="eletrico") return "elétrico";
        return tipo.empty()?"??":tipo;
    }
    int getIdPokemon() const { return idPokemon; }
    int getVida() const { return vida; }
    int getVidaMaxima() const { return maxima; }
    bool derrotado() const { return morto; }
    GolpeVisual* getGolpe(int i) { return golpeConsultado[i] && !golpes[i].objeto ? nullptr : &golpes[i]; }
};
struct TreinadorVisual {
    Treinador* objeto = nullptr;
    string nome;
    int ativo = -1;
    bool temAtivo = false, derrotado = false, temDerrota = false;
    PokemonVisual equipe[3], atual;
    ItemVisual itens[2];
    bool itemConsultado[2] = {};
    void ler(Treinador* t) {
        objeto = t;
#if MODELO_DISPONIVEL
        if (!t) return;
        consultar([&] { return t->getNome(); }, nome);
        consultar([&] { return t->getIndiceAtivo(); }, ativo, &temAtivo);
        temAtivo = temAtivo && ativo >= 0 && ativo < 3;
        consultar([&] { return t->perdeu(); }, derrotado, &temDerrota);
        Pokemon* p = nullptr;
        consultar([&] { return t->getAtivo(); }, p);
        atual.ler(p);
        for (int i = 0; i < 3; i++) {
            p = nullptr;
            consultar([&] { return t->getPokemon(i); }, p);
            equipe[i].ler(p);
        }
        for (int i = 0; i < 2; i++) {
            Item* item = nullptr;
            consultar([&] { return t->getItem(i); }, item, &itemConsultado[i]);
            itens[i].ler(item);
        }
#endif
    }
    string getNome() const { return nome.empty()?"??":nome; }
    int getIndiceAtivo() const { return temAtivo ? ativo : 0; } // Só índice gráfico seguro; não altera o modelo.
    bool perdeu() const { return derrotado; }
    PokemonVisual* getAtivo() { return &atual; }
    PokemonVisual* getPokemon(int i) { return &equipe[i]; }
    ItemVisual* getItem(int i) { return itemConsultado[i] && !itens[i].objeto ? nullptr : &itens[i]; }
};
struct BatalhaVisual {
    TreinadorVisual jogadores[2];
    int turno = -1;
    bool temTurno = false, fim = false, troca = false;
    string mensagem;
    explicit BatalhaVisual(Batalha* b) {
#if MODELO_DISPONIVEL
        if (!b) return;
        consultar([&] { return b->getTurno(); }, turno, &temTurno);
        temTurno = temTurno && turno >= 0 && turno < 2;
        consultar([&] { return b->terminou(); }, fim);
        consultar([&] { return b->precisaTrocar(); }, troca);
        consultar([&] { return b->getMensagem(); }, mensagem);
        for (int i = 0; i < 2; i++) {
            Treinador* t = nullptr;
            consultar([&] { return &b->getTreinador(i); }, t);
            jogadores[i].ler(t);
        }
#else
        mensagem = "Implemente as classes para habilitar a batalha.";
#endif
    }
    int getTurno() const { return temTurno ? turno : 0; }
    bool terminou() const { return fim; }
    bool precisaTrocar() const { return troca; }
    string getMensagem() const { return mensagem.empty()?"??":mensagem; }
    TreinadorVisual& getTreinador(int i) { return jogadores[i]; }
};

void painel(PokemonVisual* p,int x,int y,float hp) {
    DrawRectangleRounded(Rectangle{(float)x+4,(float)y+4,310,90},.12f,4,Fade(INK,.2f));
    DrawRectangleRounded(Rectangle{(float)x,(float)y,310,90},.12f,4,PAPER);
    texto(p->getNome(),x+14,y+12,22);
    texto(p->getTipo(),x+14,y+37,14,TEAL);
    texto("ATK: "+(p->temAtaque?std::to_string(p->ataque):"??"),x+126,y+39,11,Color{38,100,182,255});
    texto("DEF: "+(p->temDefesa?std::to_string(p->defesa):"??"),x+218,y+39,11,Color{181,55,54,255});
    DrawRectangle(x+52,y+59,240,12,INK);
    if (p->temVida && p->temMaxima) {
        Color cor=hp<p->getVidaMaxima()*.25f?RED:(hp<p->getVidaMaxima()*.5f?AMBAR:Color{72,179,114,255});
        float proporcao=std::max(0.f,std::min(1.f,hp/p->getVidaMaxima()));
        DrawRectangle(x+54,y+61,(int)(236*proporcao),8,cor);
    }
    texto("PV",x+14,y+57,16);
    string atual=p->temVida?std::to_string((int)std::ceil(hp)):"??";
    string maxima=p->temMaxima?std::to_string(p->getVidaMaxima()):"??";
    texto(atual+" / "+maxima,x+212,y+76,12);
}
enum Efeito { NENHUM, ATAQUE, CURA, FORCA, TROCA };
struct Animacao {
    Efeito efeito=NENHUM;
    int acao=0,indice=0,alvo=0,jogador=0;
    float tempo=0,duracao=1.6f,instanteImpacto=.65f;
    bool aplicada=false;
    bool ativa() const { return efeito!=NENHUM; }
    void iniciar(Efeito e,int a,int i,int dest,int j) { efeito=e; acao=a; indice=i; alvo=dest; jogador=j; tempo=0; aplicada=false; duracao=1.6f; instanteImpacto=.65f; }
};
void quadro(Texture2D tex,int largura,int altura,int frame,Vector2 pos,float escala=3,float rotacao=0,Color tint=WHITE) {
    int colunas=tex.width/largura, total=colunas*(tex.height/altura);
    if(total<1) return;
    frame=frame%total;
    Rectangle origem={(float)(frame%colunas*largura),(float)(frame/colunas*altura),(float)largura,(float)altura};
    DrawTexturePro(tex,origem,Rectangle{pos.x,pos.y,largura*escala,altura*escala},Vector2{largura*escala/2,altura*escala/2},rotacao,tint);
}
void efeitoGolpe(Efeito efeito,Vector2 alvo,float t) {
    if(efeito==CURA || efeito==FORCA) {
        Texture2D sprite=efeito==FORCA?fortificante:cura;
        Color cor=efeito==FORCA?YELLOW:WHITE;
        for(int i=0;i<6;i++) {
            float q=std::fmod(t+i*.15f,1.f);
            quadro(sprite,16,16,(int)(q*4),Vector2{alvo.x+std::sin(i*2.4f)*66,alvo.y+55-q*130},3,0,Fade(cor,std::sin(q*PI)));
        }
    }

}

// Estado gráfico privado: a main só precisa conhecer Acao e Batalha.
bool audio = false;
Music musicas[3] = {};
int faixa = -1;
Texture2D frente[TOTAL_POKEMON] = {}, costas[TOTAL_POKEMON] = {};
Rectangle limitesCostas[TOTAL_POKEMON] = {};
const float loopInicio[] = {10.151383f, 25.209569f, 2.077800f};
const float loopFim[] = {51.457800f, 107.804580f, 16.214263f};
// Estado da partida e escolhas dos jogadores.
Tela tela=INICIO;
int perfil[2]={0,1}, jogador=0,menu=0;
int paginaEquipe[2]={0,0};
string nome[2]={"",""};
std::vector<int> escolhas[2];
float saltoSelecao[2][TOTAL_POKEMON]={};
float destaqueSelecao[TOTAL_POKEMON]={};
Animacao anim;
float vidas[2][3]={};
bool vidasConhecidas[2][3]={};
bool sair=false, editando=false;

void reiniciarInterface() {
    tela=INICIO;
    contextoTeclado=-1; focoTeclado=Rectangle{}; controlesAnteriores.clear();
    perfil[0]=0; perfil[1]=1;
    nome[0].clear(); nome[1].clear();
    escolhas[0].clear(); escolhas[1].clear();
    for(int i=0;i<TOTAL_POKEMON;i++) {
        destaqueSelecao[i]=0;
        saltoSelecao[0][i]=saltoSelecao[1][i]=-10;
    }
    jogador=0; menu=0;
    paginaEquipe[0]=paginaEquipe[1]=0;
    anim=Animacao();
    editando=false; bloqueado=false; sair=false;
    for (int j=0;j<2;j++) for (int i=0;i<3;i++) {
        vidas[j][i]=0;
        vidasConhecidas[j][i]=false;
    }
    if (audio && faixa>=0) StopMusicStream(musicas[faixa]);
    faixa=-1;
    som=true;
}

}
// Inicialização da janela, áudio e imagens.
bool iniciarInterface() {
    InitWindow(960,640,"Pokémon POO | Programação Orientada a Objetos");
    if(!IsWindowReady()) return false;
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    if(!DirectoryExists("assets")) {
        if(DirectoryExists("lib/assets")) {
            if(!ChangeDirectory("lib")) { CloseWindow(); return false; }
        } else {
            if(!ChangeDirectory(GetApplicationDirectory())) { CloseWindow(); return false; }
            if(DirectoryExists("../assets") && !ChangeDirectory("..")) { CloseWindow(); return false; }
        }
    }
    // Inclui os caracteres acentuados usados nos textos em português.
    int caracteres[224];
    for(int i=0;i<224;i++) caracteres[i]=32+i;
    fonteInterface=LoadFontEx("assets/fontes/Pokemon Classic.ttf",32,caracteres,224);
    SetTextureFilter(fonteInterface.texture,TEXTURE_FILTER_BILINEAR);
    InitAudioDevice();
    audio=IsAudioDeviceReady();
    const char* faixas[]={"assets/audio/inicio.ogg","assets/audio/batalha.ogg","assets/audio/vitoria.ogg"};
    if(audio) for(int i=0;i<3;i++) { musicas[i]=LoadMusicStream(faixas[i]); musicas[i].looping=true; SetMusicVolume(musicas[i],.32f); }
    auto carregar=[](const char* nome) { Texture2D t=LoadTexture(TextFormat("assets/frlg/%s.png",nome)); SetTextureFilter(t,TEXTURE_FILTER_POINT); return t; };
    const char* perfis[]={"trainer_red","trainer_leaf","trainer_leader_brock","trainer_leader_misty"};
    for(int i=0;i<4;i++) retratos[i]=carregar(perfis[i]);
    campo=carregar("campo"); logo=carregar("logo"); charizardTitulo=carregar("charizard_title"); textoTitulo=carregar("title_text"); chamasTitulo=carregar("title_flames");
    cura=carregar("green_sparkle"); pokebola=carregar("pokeball");
    Image brilho=LoadImage("assets/frlg/green_sparkle.png");
    // Grayscale descartava o canal alpha e desenhava retangulos opacos.
    ImageFormat(&brilho,PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Color* pixels=static_cast<Color*>(brilho.data);
    for(int i=0;i<brilho.width*brilho.height;i++) {
        unsigned char intensidade=std::max(pixels[i].r,std::max(pixels[i].g,pixels[i].b));
        pixels[i].r=pixels[i].g=pixels[i].b=intensidade;
    }
    fortificante=LoadTextureFromImage(brilho);
    UnloadImage(brilho);
    SetTextureFilter(fortificante,TEXTURE_FILTER_POINT);
    for(int i=0;i<TOTAL_POKEMON;i++) {
        frente[i]=LoadTexture(TextFormat("assets/%i-frente.png",i));
        costas[i]=LoadTexture(TextFormat("assets/%i-costas.png",i));
        SetTextureFilter(frente[i],TEXTURE_FILTER_POINT);
        SetTextureFilter(costas[i],TEXTURE_FILTER_POINT);
        Image imagemCostas=LoadImage(TextFormat("assets/%i-costas.png",i));
        limitesCostas[i]=GetImageAlphaBorder(imagemCostas,.1f);
        UnloadImage(imagemCostas);
    }
    return true;
}

bool interfaceAberta() {
    return !WindowShouldClose() && !sair;
}

std::string getNomeEscolhido(int jogador) {
    return nome[jogador];
}

int* getEquipeEscolhida(int jogador) {
    return escolhas[jogador].data();
}

void mostrarCombate() {
    tela=COMBATE; menu=0; anim=Animacao();
    for (int j=0;j<2;j++) for (int i=0;i<3;i++) vidasConhecidas[j][i]=false;
}
void mostrarResultado(bool sucesso) {
    if (!sucesso) anim=Animacao();
}

// Atualiza um único quadro. A main controla a repetição e executa as ações.
Acao atualizarInterface(Batalha* modelo) {
    BatalhaVisual visao(modelo);
    BatalhaVisual* batalha=&visao;
    Acao acao;
    // A ação é executada pela main depois do quadro anterior. Esta checagem
    // também cobre um quadro lento que tenha encerrado a animação no impacto.
    if (tela == COMBATE && batalha && batalha->terminou() && !anim.ativa()) {
        tela = VITORIA;
    }
    float dt=GetFrameTime(),agora=(float)GetTime();
    int desejada=tela==COMBATE?1:(tela==VITORIA?2:0);
    if(audio) {
        if(desejada!=faixa) { if(faixa>=0) StopMusicStream(musicas[faixa]); faixa=desejada; PlayMusicStream(musicas[faixa]); }
        UpdateMusicStream(musicas[faixa]);
        if(GetMusicTimePlayed(musicas[faixa])>=loopFim[faixa]-.04f) SeekMusicStream(musicas[faixa],loopInicio[faixa]);
        SetMasterVolume(som?1.f:0.f);
    }
    bool digitandoNome=tela==TREINADORES && (editando ||
        (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(),Rectangle{280,411,400,48})));
    if(!digitandoNome && IsKeyPressed(KEY_M)) som=!som;
    // A ação é aplicada no instante de impacto da animação.
    if(tela==COMBATE && anim.ativa()) {
        anim.tempo+=dt;
        if(anim.tempo>=anim.instanteImpacto && !anim.aplicada) {
            // Entrega a ação à main no instante de impacto da animação.
            acao.tipo = anim.acao==0 ? ATACAR : (anim.acao==1 ? USAR_ITEM : TROCAR);
            acao.indice = anim.indice;
            acao.alvo = anim.alvo;
            anim.aplicada=true;
        }
        if(anim.efeito==ATAQUE && batalha->getTreinador(1-anim.jogador).getAtivo()->derrotado())
            anim.duracao=std::max(anim.duracao,anim.instanteImpacto+1.f);
        if(anim.tempo>=anim.duracao) { anim.efeito=NENHUM; menu=0; if(batalha->terminou()) tela=VITORIA; }
    }
    for(int j=0;j<2;j++) for(int i=0;i<3;i++) {
        PokemonVisual* p=batalha->getTreinador(j).getPokemon(i);
        if (!p->temVida) continue;
        float destino=(float)p->getVida();
        if (!vidasConhecidas[j][i]) { vidas[j][i]=destino; vidasConhecidas[j][i]=true; }
        else vidas[j][i]+=(destino-vidas[j][i])*std::min(1.f,dt*8);
        if(std::fabs(vidas[j][i]-destino)<.1f) vidas[j][i]=destino;
    }
    bloqueado=anim.ativa();
    bool sairPorEscape=tela==INICIO && IsKeyPressed(KEY_ESCAPE);
    bool encerrouEdicao=false;
    if(tela==TREINADORES && editando && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB))) {
        editando=false; encerrouEdicao=true;
        focoTeclado=Rectangle{640,535,250,48}; usandoTeclado=true;
    }
    if(IsKeyPressed(KEY_ESCAPE) && !encerrouEdicao && !anim.ativa()) {
        if(tela==TREINADORES) { if(jogador==1) jogador=0; else tela=INICIO; }
        else if(tela==EQUIPE) { if(jogador==1) jogador=0; else { tela=TREINADORES; jogador=1; } }
        else if(tela==COMBATE && !batalha->precisaTrocar()) {
            if(menu) menu=0;
            else { acao.tipo=ENCERRAR_BATALHA; reiniciarInterface(); }
        } else if(tela==VITORIA) { acao.tipo=ENCERRAR_BATALHA; reiniciarInterface(); }
    }
    if(tela==COMBATE && batalha->precisaTrocar() && !anim.ativa()) menu=2;
    BeginDrawing(); fundo(tela);
    int contexto=(int)tela+100*menu+1000*jogador+10000*paginaEquipe[jogador];
    navegarTeclado(contexto,editando || encerrouEdicao || anim.ativa());

    // Menu inicial.
    if(tela==INICIO) {
        DrawTextureEx(logo,Vector2{(960-logo.width*2.f)/2,20},0,2,WHITE);
        centro("Universidade Federal de Viçosa",246,22,TEAL);
        centro("Departamento de Informática - DPI",279,15,TEAL);
        DrawTextureEx(charizardTitulo,Vector2{610,298},0,2.4f,WHITE);
        texto("Trabalho 02 - INF 112",88,334,24,TEAL);
        texto("PROGRAMAÇÃO II - 2026/2",88,370,15,INK);
        bool iniciar=botao(Rectangle{88,398,360,58},"JOGAR");
        if(iniciar) {
            tela=TREINADORES; jogador=0; escolhas[0].clear(); escolhas[1].clear();
        }
        if(botao(Rectangle{88,474,360,46},"SAIR") || sairPorEscape) sair=true;
        centro("Setas: navegar   Enter/Espaço: escolher   Esc: voltar",576,14,INK);
        centro("Tab/Shift+Tab: alternar opções   M: som   Mouse: clicar",604,13,TEAL);
    // Seleção do perfil e nome de cada treinador.
    } else if(tela==TREINADORES) {
        centro("ESCOLHA SEU TREINADOR",38,36);
        centro("Jogador "+std::to_string(jogador+1)+" / 2",88,20,TEAL);
        for(int i=0;i<4;i++) {
            int x=70+i*225;
            DrawRectangleRounded(Rectangle{(float)x,145,195,200},.12f,4,Fade(cores[i],.20f));
            retrato(i,x+48,163);
            if(botao(Rectangle{(float)x+12,292,171,40},treinadores[i],true,perfil[jogador]==i)) { perfil[jogador]=i; }
        }
        centro("Nome do treinador",378,18,TEAL);
        Rectangle campo={280,411,400,48};
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editando=CheckCollisionPointRec(GetMousePosition(),campo);
        controleInterativo(campo,true);
        if(ativacaoControle) editando=true;
        DrawRectangleRounded(campo,.15f,4,PAPER);
        DrawRectangleRoundedLinesEx(campo,.15f,4,editando?3:1,TEAL);
        texto(nome[jogador]+(editando && ((int)(agora*2)%2==0)?"_":""),300,425,20);
        int ch=GetCharPressed();
        while(ch) {
            if(editando && ch>=32 && ch<=255 && GetCodepointCount(nome[jogador].c_str())<18) {
                int bytes=0;
                const char* letra=CodepointToUTF8(ch,&bytes);
                nome[jogador].append(letra,bytes);
            }
            ch=GetCharPressed();
        }
        if(editando && IsKeyPressed(KEY_BACKSPACE) && !nome[jogador].empty()) {
            size_t inicio=nome[jogador].size()-1;
            while(inicio>0 && ((unsigned char)nome[jogador][inicio]&0xC0)==0x80) inicio--;
            nome[jogador].erase(inicio);
        }
        if(botao(Rectangle{70,535,200,48},"VOLTAR")) { editando=false; if(jogador==1) jogador=0; else tela=INICIO; }
        if(botao(Rectangle{640,535,250,48},jogador==0?"PRÓXIMO TREINADOR":"ESCOLHER POKÉMON",!nome[jogador].empty())) {
            editando=false;
            if(jogador==0) jogador=1; else { tela=EQUIPE; jogador=0; }
        }
    // Montagem das equipes antes de criar a batalha.
    } else if(tela==EQUIPE) {
        centro("MONTE SUA EQUIPE",30,36);
        centro(nome[jogador]+" - escolha 3 Pokémon ("+std::to_string(escolhas[jogador].size())+"/3)",76,20,TEAL);
        const int porPagina=6;
        const int paginas=(TOTAL_POKEMON+porPagina-1)/porPagina;
        int inicio=paginaEquipe[jogador]*porPagina;
        for(int i=inicio;i<std::min(inicio+porPagina,TOTAL_POKEMON);i++) {
            int slot=i-inicio;
            int x=98+(slot%3)*272, y=114+(slot/3)*178;
            auto pos=std::find(escolhas[jogador].begin(),escolhas[jogador].end(),i);
            bool marcado=pos!=escolhas[jogador].end();
            Rectangle card={(float)x,(float)y,220,158};
            bool hover=CheckCollisionPointRec(GetMousePosition(),card);
            destaqueSelecao[i]+=(float(hover)-destaqueSelecao[i])*std::min(1.f,dt*12);
            float escala=1.2f+.12f*destaqueSelecao[i];
            float t=agora-saltoSelecao[jogador][i];
            float salto=(t>=0 && t<.4f)?8*std::sin(t/.4f*PI):0;
            DrawRectangleRounded(card,.1f,4,marcado?Fade(TEAL,.25f):PAPER);
            DrawTextureEx(frente[i],Vector2{x+(220-frente[i].width*escala)/2,y+93-frente[i].height*escala-salto},0,escala,WHITE);
            texto(tipos[i],x+12,y+10,12,TEAL);
            texto(TextFormat("ATK: %i",ataquesCatalogo[i]),x+14,y+98,13,Color{38,100,182,255});
            texto(TextFormat("DEF: %i",defesasCatalogo[i]),x+122,y+98,13,Color{181,55,54,255});
            if(marcado) texto(std::to_string((int)(pos-escolhas[jogador].begin())+1),x+194,y+10,18,TEAL);
            if(botao(Rectangle{(float)x+18,(float)y+116,184,30},nomes[i],marcado || escolhas[jogador].size()<3,marcado)) {
                if(marcado) escolhas[jogador].erase(pos);
                else { escolhas[jogador].push_back(i); saltoSelecao[jogador][i]=agora; }
            }
        }
        if(botao(Rectangle{98,474,160,36},"ANTERIOR",paginaEquipe[jogador]>0)) paginaEquipe[jogador]--;
        centro(std::to_string(paginaEquipe[jogador]+1)+" / "+std::to_string(paginas),484,18,TEAL);
        if(botao(Rectangle{702,474,160,36},"PRÓXIMA",paginaEquipe[jogador]+1<paginas)) paginaEquipe[jogador]++;
        texto("Sua equipe",240,551,16,TEAL);
        for(int i=0;i<3;i++) {
            int x=400+i*76;
            if(i<(int)escolhas[jogador].size()) {
                int idPokemon=escolhas[jogador][i];
                float t=agora-saltoSelecao[jogador][idPokemon];
                float salto=(t>=0 && t<.4f)?8*std::sin(t/.4f*PI):0;
                DrawTextureEx(frente[idPokemon],Vector2{(float)x,532-salto},0,.75f,WHITE);
            } else DrawEllipse(x+24,568,14,3,Fade(TEAL,.15f));
        }
        if(botao(Rectangle{48,584,200,42},"VOLTAR")) { if(jogador==1) jogador=0; else { tela=TREINADORES; jogador=1; } }
        if(botao(Rectangle{600,584,310,42},jogador==0?"EQUIPE DO JOGADOR 2":"ENTRAR NA ARENA",escolhas[jogador].size()==3)) {
            if(jogador==0) jogador=1;
            else {
                acao.tipo = INICIAR_BATALHA;
            }
        }
    // Exibição do combate e dos menus de ataque, item e troca.
    } else if(tela==COMBATE) {
        int turno=anim.ativa()?anim.jogador:batalha->getTurno();
        TreinadorVisual& t=batalha->getTreinador(turno);
        PokemonVisual* a=t.getAtivo();
        TreinadorVisual& inferior=batalha->getTreinador(0);
        TreinadorVisual& superior=batalha->getTreinador(1);
        PokemonVisual* baixo=inferior.getAtivo();
        PokemonVisual* cima=superior.getAtivo();
        DrawTexturePro(campo,Rectangle{0,0,240,110},Rectangle{0,0,960,440},Vector2{0,0},0,WHITE);
        // Os sprites de costas têm a base aberta: ela encontra o limite do campo.
        const float chaoInferior=440;
        Vector2 pa={220,chaoInferior},pb={624,78};
        if(anim.ativa() && anim.efeito==ATAQUE) {
            float fase=std::min(1.f,anim.tempo/.42f);
            float avanco=std::sin(fase*PI);
            if(turno==0) pa.x+=28*avanco;
            else pb.x-=28*avanco;
        }
        bool impacto=anim.ativa() && anim.efeito==ATAQUE && anim.tempo>anim.instanteImpacto && anim.tempo<anim.instanteImpacto+.35f;
        if(impacto) {
            float tremor=std::sin(anim.tempo*85)*4;
            if(turno==0) pb.x+=tremor; else pa.x+=tremor;
        }
        bool piscar=impacto && (int)(anim.tempo*20)%2;
        auto desenharPokemon=[&](PokemonVisual* p,Texture2D sprite,Vector2 pos,float escala,int lado) {
            bool trocando=anim.ativa() && anim.efeito==TROCA && anim.jogador==lado;
            bool desmaiando=p->derrotado() && anim.ativa() && anim.efeito==ATAQUE && anim.jogador!=lado;
            float queda=desmaiando?std::max(0.f,std::min(1.f,(anim.tempo-anim.instanteImpacto-.15f)/.65f)):0;
            float suavidade=queda*queda*(3-2*queda);
            if(piscar && turno!=lado && !desmaiando) return;
            // O derrotado desaparece nesta ação e permanece oculto durante a troca.
            bool mostrarSprite=!p->derrotado() || (desmaiando && queda<1);
            float tamanho=1;
            Color cor=p->temIdPokemon?WHITE:BLACK;
            if(desmaiando) {
                pos.y+=32*suavidade;
                cor=Fade(cor,1-suavidade);
            }
            if(trocando) {
                if(anim.tempo<.45f) {
                    tamanho=1-anim.tempo/.45f;
                    if (p->temIdPokemon) cor=Color{255,130,150,255};
                } else if(anim.tempo<1.05f) tamanho=0;
                else tamanho=std::min(1.f,(anim.tempo-1.05f)/.35f);
            }
            if(mostrarSprite && tamanho>.01f) {
                Rectangle origem={0,0,(float)sprite.width,(float)sprite.height};
                float escalaAtual=escala*tamanho;
                if(lado==0) {
                    // O chão é a âncora do sprite, desconsiderando as bordas transparentes.
                    Rectangle limite=limitesCostas[p->temIdPokemon?p->getIdPokemon():3];
                    if(limite.height<=0) limite=origem;
                    DrawEllipse((int)pos.x,(int)chaoInferior,58*tamanho,7*tamanho,Fade(INK,.12f*tamanho*(1-suavidade)));
                    Rectangle dest={pos.x-(limite.x+limite.width/2)*escalaAtual,
                        pos.y-(limite.y+limite.height)*escalaAtual,
                        sprite.width*escalaAtual,sprite.height*escalaAtual};
                    DrawTexturePro(sprite,origem,dest,Vector2{0,0},0,cor);
                } else {
                    Vector2 centro={pos.x+sprite.width*escala/2,pos.y+sprite.height*escala/2};
                    Rectangle dest={centro.x,centro.y,sprite.width*escalaAtual,sprite.height*escalaAtual};
                    DrawTexturePro(sprite,origem,dest,Vector2{dest.width/2,dest.height/2},0,cor);
                }
            }
            if(trocando) {
                Vector2 centro=lado==0?Vector2{220,340}:Vector2{720,175};
                if(anim.tempo>.5f && anim.tempo<1.08f) {
                    float q=(anim.tempo-.5f)/.58f;
                    float direcao=lado==0?1.f:-1.f;
                    Vector2 bola={centro.x-direcao*145*(1-q),centro.y-std::sin(q*PI)*80};
                    quadro(pokebola,16,16,0,bola,2.5f,q*360);
                }
                if(anim.tempo>1.02f && anim.tempo<1.42f) {
                    float q=(anim.tempo-1.02f)/.4f;
                    for(int i=0;i<8;i++) {
                        float ang=i*PI/4;
                        Vector2 estrela={centro.x+std::cos(ang)*q*95,centro.y+std::sin(ang)*q*65};
                        quadro(cura,16,16,(int)(q*4),estrela,2,0,Fade(WHITE,1-q));
                    }
                }
            }
        };
        desenharPokemon(cima,frente[cima->temIdPokemon?cima->getIdPokemon():3],pb,3,1);
        desenharPokemon(baixo,costas[baixo->temIdPokemon?baixo->getIdPokemon():3],pa,4,0);
        painel(cima,45,64,vidas[1][superior.getIndiceAtivo()]);
        painel(baixo,610,322,vidas[0][inferior.getIndiceAtivo()]);
        texto(superior.getNome(),48,34,19,TEAL); texto(inferior.getNome(),612,292,19,TEAL);
        if(anim.ativa()) {
            Vector2 alvo=turno==0?Vector2{220,340}:Vector2{720,175};
            efeitoGolpe(anim.efeito,alvo,anim.tempo);
        }
        if(batalha->precisaTrocar() && !anim.ativa()) menu=2;
        int menuDesenhado=menu;
        // A mesma moldura é usada por todas as telas de escolha da batalha.
        molduraBatalha(0,440,548,200,Color{44,76,91,255});
        molduraBatalha(544,440,416,200,PAPER);
        string instrucao=visao.temTurno?"Vez de "+t.getNome():"Vez de ??";
        if(menuDesenhado==1) instrucao="Escolha um golpe.";
        else if(menuDesenhado==2) instrucao="Escolha um Pokémon.";
        else if(menuDesenhado==3) instrucao="Escolha um item.";
        string pergunta=menuDesenhado==0?"O que "+a->getNome()+" fará?":instrucao;
        texto(pergunta,28,466,22,WHITE);
        textoEmCaixa(visao.getMensagem(),28,503,492,84,16,WHITE);
        string vez=visao.temTurno?"Vez de "+t.getNome():"Vez de ??";
        texto(anim.ativa()?"Ação em andamento...":vez,28,594,18,WHITE);
        if(menuDesenhado==0) {
            bool cursorInicial=!usandoTeclado && !CheckCollisionPointRec(GetMousePosition(),Rectangle{560,460,384,164});
            if(opcaoBatalha(Rectangle{562,466,186,66},"ATACAR",cursorInicial)) menu=1;
            if(opcaoBatalha(Rectangle{754,466,186,66},"MOCHILA")) menu=3;
            if(opcaoBatalha(Rectangle{562,548,186,66},"POKÉMON")) menu=2;
            bool bloqueioAcao=bloqueado;
            bloqueado=false;
            if(opcaoBatalha(Rectangle{754,548,186,66},"MENU")) {
                acao.tipo=ENCERRAR_BATALHA;
                reiniciarInterface();
            } else bloqueado=bloqueioAcao;
        }
        // Os submenus usam a mesma grade, preenchida apenas com dados das classes.
        if(menuDesenhado==1) {
            for(int i=0;i<2;i++) {
                GolpeVisual* g=a->getGolpe(i);
                if(opcaoBatalha(posicaoOpcao(i),g?g->getNome():"--",false,g && g->objeto && visao.temTurno)) {
                    Efeito efeito=ATAQUE;
                    anim.iniciar(efeito,0,i,0,turno);
                    if(efeito==ATAQUE) { anim.duracao=.85f; anim.instanteImpacto=.24f; }
                    break;
                }
            }
        } else if(menuDesenhado==2) {
            for(int i=0;i<3;i++) {
                PokemonVisual* p=t.getPokemon(i);
                bool valido=visao.temTurno && t.temAtivo && p->objeto && i!=t.getIndiceAtivo() && !p->derrotado();
                string vida="PV "+(p->temVida?std::to_string(p->getVida()):"??")+" / "+(p->temMaxima?std::to_string(p->getVidaMaxima()):"??");
                if(opcaoBatalha(posicaoOpcao(i),p->getNome(),false,valido,vida)) {
                    anim.iniciar(TROCA,2,i,i,turno);
                    anim.duracao=1.8f;
                    break;
                }
            }
        } else if(menuDesenhado==3) {
            for(int i=0;i<2;i++) {
                ItemVisual* item=t.getItem(i);
                bool valido=visao.temTurno && t.temAtivo && item && item->objeto && a->objeto && !a->derrotado() && (i==1 || (a->temVida && a->temMaxima && a->getVida()<a->getVidaMaxima()));
                if(opcaoBatalha(posicaoOpcao(i),item?item->getNome():"Consumido",false,valido)) {
                    anim.iniciar(i==0?CURA:FORCA,1,i,t.getIndiceAtivo(),turno);
                    break;
                }
            }
        }
        if(menuDesenhado && opcaoBatalha(posicaoOpcao(3),"VOLTAR",false,!batalha->precisaTrocar())) menu=0;
    } else if(tela==VITORIA) {
        bloqueado=false;
        int vencedor=batalha->getTreinador(0).perdeu()?1:0;
        celebracaoVitoria(agora);
        centro("CAMPEÃO DA ARENA",48,14,TEAL);
        centro("VITÓRIA!",87,52,Fade(AMBAR,.55f));
        centro("VITÓRIA!",84,52,INK);
        if (batalha->getTreinador(0).temDerrota)
            centro(batalha->getTreinador(vencedor).getNome()+" venceu a batalha",148,22,TEAL);

        DrawCircleGradient(480,264,95,Color{249,218,143,180},Color{249,218,143,0});
        DrawEllipse(480,326,72,10,Fade(INK,.10f));
        retrato(perfil[vencedor],432,205,4);
        centro("EQUIPE VENCEDORA",340,13,TEAL);
        for(int i=0;i<3;i++) {
            PokemonVisual* p=batalha->getTreinador(vencedor).getPokemon(i);
            float x=154.f+i*224;
            DrawRectangleRounded(Rectangle{x+3,366,204,132},.08f,4,Fade(INK,.07f));
            DrawRectangleRounded(Rectangle{x,362,204,132},.08f,4,Color{255,253,241,255});
            DrawRectangleRoundedLinesEx(Rectangle{x,362,204,132},.08f,4,1,Color{213,199,157,255});
            string nomePokemon=p->getNome();
            int tamanho=16;
            while(tamanho>12 && larguraTexto(nomePokemon.c_str(),tamanho)>180) tamanho--;
            texto(nomePokemon,(int)(x+(204-larguraTexto(nomePokemon.c_str(),tamanho))/2),376,tamanho,TEAL);
            Texture2D sprite=frente[p->temIdPokemon?p->getIdPokemon():3];
            float escala=1.45f;
            DrawTextureEx(sprite,Vector2{x+(204-sprite.width*escala)/2,391+std::sin(agora*2+i)*3},0,escala,p->temIdPokemon?WHITE:BLACK);
        }
        textoEmCaixa(visao.getMensagem(),28,591,904,44,14,INK);
        if(botao(Rectangle{110,525,340,55},"JOGAR NOVAMENTE",true,true)) { acao.tipo=ENCERRAR_BATALHA; escolhas[0].clear(); escolhas[1].clear(); jogador=0; tela=TREINADORES; }
        if(botao(Rectangle{510,525,340,55},"MENU INICIAL")) { acao.tipo=ENCERRAR_BATALHA; reiniciarInterface(); }
    }
    bool travado=bloqueado; bloqueado=false;
    if(botao(Rectangle{804,12,142,28},som?"SOM: LIGADO [M]":"SOM: MUDO [M]")) som=!som;
    bloqueado=acao.tipo==ENCERRAR_BATALHA?false:travado;
    controlesAnteriores=controlesAtuais;
    EndDrawing();
    return acao;
}

void fecharInterface() {
    for(int i=0;i<TOTAL_POKEMON;i++) { UnloadTexture(frente[i]); UnloadTexture(costas[i]); }
    for(auto tex:retratos) UnloadTexture(tex);
    for(auto tex:{campo,logo,charizardTitulo,textoTitulo,chamasTitulo,cura,fortificante,pokebola}) UnloadTexture(tex);
    if(audio) { for(auto& m:musicas) UnloadMusicStream(m); CloseAudioDevice(); }
    UnloadFont(fonteInterface);
    CloseWindow();
}

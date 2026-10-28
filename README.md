<div align="center">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/versions/generation-v/black-white/animated/6.gif" alt="Charizard" width="80">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/master-ball.png" alt="Pokeball" width="40" style="margin: 0 30px;">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/versions/generation-v/black-white/animated/9.gif" alt="Blastoise" width="80">

  <h1>Simulador de Batalhas Pokémon ⚔️</h1>
  
  <p>
    <strong>Trabalho Prático 2 (TP2) • INF 112 • UFV</strong><br>
    <em>Disciplina de Programação Orientada a Objetos</em>
  </p>

  <p>
    <img src="https://img.shields.io/badge/Linguagem-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++">
    <img src="https://img.shields.io/badge/Institui%C3%A7%C3%A3o-UFV-red?style=for-the-badge" alt="UFV">
    <img src="https://img.shields.io/badge/Projeto-Individual-blue?style=for-the-badge" alt="Individual">
  </p>
</div>

<br>

> **Desenvolvido por:** Paulo Vitor Martins Silva  
> **Professor:** Julio Reis  
> **Estilo:** Pokémon FireRed / LeafGreen (GBA)

<br>
<div align="center">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30">
</div>
<br>

## 📖 Sobre o Projeto

Este projeto é um simulador de batalhas Pokémon focado na aplicação prática de **Programação Orientada a Objetos (POO)** e **Tipos Abstratos de Dados (TADs)**. 

A interface gráfica do simulador foi fornecida previamente pelo professor Julio Reis. O núcleo do trabalho consistiu em projetar, implementar e integrar o *backend* do jogo (a lógica de negócios). Isso exigiu a construção de um ecossistema de classes robusto para gerenciar Pokémons, seus atributos, bibliotecas de tipos, cálculos de dano e a mecânica de golpes, fazendo com que a interface gráfica se comunicasse perfeitamente com os objetos instanciados.

<br>
<div align="center">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30">
</div>
<br>

## 🧬 Conceitos Aplicados

O simulador foi arquitetado utilizando os pilares da Orientação a Objetos. Abaixo estão as mecânicas implementadas no código:

<table>
  <tr>
    <td align="center">
      <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/exp-share.png" width="40"><br>
      <b>Herança</b>
    </td>
    <td>Reutilização de código através de classes base (ex: <code>PokemonBase</code>, <code>Golpe</code>) estendidas para tipos específicos, permitindo especialização sem duplicação de lógica.</td>
  </tr>
  <tr>
    <td align="center">
      <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/choice-specs.png" width="40"><br>
      <b>Polimorfismo</b>
    </td>
    <td>Uso de métodos virtuais e sobrescrita para que diferentes tipos de ataques ou Pokémons respondam de maneira única à mesma chamada de função (ex: cálculo de dano modificado por tipo).</td>
  </tr>
  <tr>
    <td align="center">
      <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/tm01.png" width="40"><br>
      <b>Interfaces</b>
    </td>
    <td>Contratos bem definidos estabelecendo como a interface gráfica do professor deve acessar os dados das bibliotecas e dos objetos no backend.</td>
  </tr>
  <tr>
    <td align="center">
      <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokedex.png" width="40"><br>
      <b>TADs</b>
    </td>
    <td>Encapsulamento pesado. Atributos como HP, Status, Tipos e Golpes são protegidos, acessados e modificados apenas através de métodos seguros.</td>
  </tr>
</table>

<br>
<div align="center">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30">
</div>
<br>

## 🖼️ Demonstração Visual

<div align="center">
  <i>(Substitua os links abaixo pelas screenshots reais do seu jogo rodando)</i>
  <br><br>
  <table>
    <tr>
      <td align="center"><b>Menu de Batalha</b></td>
      <td align="center"><b>Seleção de Golpes</b></td>
    </tr>
    <tr>
      <td>
        <!-- INSIRA O LINK DA SUA IMAGEM AQUI -->
        <img src="https://via.placeholder.com/400x250/222222/ffffff?text=Screenshot+1:+Batalha+Inicial" alt="Tela de Batalha" width="400">
      </td>
      <td>
        <!-- INSIRA O LINK DA SUA IMAGEM AQUI -->
        <img src="https://via.placeholder.com/400x250/222222/ffffff?text=Screenshot+2:+Menu+de+Ataques" alt="Tela de Ataques" width="400">
      </td>
    </tr>
  </table>
</div>

<br>
<div align="center">
  <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30"><img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/miracle-seed.png" width="30">
</div>
<br>

## 🚀 Como Clonar, Compilar e Rodar

O projeto utiliza `Make` para automatizar o processo de compilação, garantindo que todas as dependências e classes sejam linkadas corretamente junto à interface gráfica.

Abra o seu terminal e execute exatamente o bloco abaixo:

```bash
git clone (https://github.com/PauloVitorMartins/Pokemon-POO---INF-112-UFV.git)
cd Pokemon-POO---INF-112-UFV/tp02
make
./lib/.build/pokemon
```

Se precisar recompilar use

```bash
make clean
make

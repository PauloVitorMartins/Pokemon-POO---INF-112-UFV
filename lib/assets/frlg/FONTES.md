# FireRed / LeafGreen

Os graficos e as composicoes sao de Pokemon FireRed / LeafGreen,
Game Freak / Nintendo / The Pokemon Company (2004).

Graficos originais: https://github.com/pret/pokefirered/tree/master/graphics

- Treinadores: Red, Leaf, Brock e Misty, de `trainers/front_pics`.
- Pokemon: sprites de frente e costas e paletas normais de `pokemon`.
- Campo: `battle_terrain/grass`, reconstruido do tilemap com a paleta
  carregada a partir do slot 2, conforme `src/battle_bg.c`.
- Logotipo: `title_screen/firered/game_title_logo`, reconstruido do tilemap.
- Efeitos: folhas, brasas, bolhas, agua, eletricidade, brilho de cura,
  energia, ondas e impacto de `battle_anims/sprites`.

Os PNGs preservam os pixels e as paletas, com indice 0 transparente para
sprites. A interface usa filtro de ponto. Os arquivos de origem estao em
`originais/graphics`. O conversor de apoio usa Pillow e nao participa do
`make run`.

Os ataques usam um avanco curto do atacante e piscadas no alvo, conforme
pedido para esta atividade. Nao ha projeteis elementais. A cura e o
fortificante usam sprites originais com tempos adaptados para Raylib.

Musicas originais, em OGG com pontos de loop, disponibilizadas por ENLS:
https://enls.eu/prelooped/album/?game=FRLG

- `../audio/inicio.ogg`: FRLG 03 Title Screen.
- `../audio/batalha.ogg`: FRLG 11 Battle (VS Trainer).
- `../audio/vitoria.ogg`: FRLG 12 Victory (VS Trainer).

Os pontos de loop seguem os metadados LOOPSTART e LOOPLENGTH dos OGGs.
Todos os arquivos necessarios acompanham o projeto; a execucao nao acessa
a internet nem requer conversores de imagem ou audio.

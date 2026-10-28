// Treinador::Treinador(string nome, int pokemons[3]) {
//     /* 
//     ==============================================================
//     Implementação fornecida: leitura do arquivo e criação da equipe
//     Complete o construtor com a inicialização dos demais atributos
//     ==============================================================
//     */

//     for (int indice = 0; indice < 3; indice++) {
//         equipe[indice] = nullptr;
//     }

//     std::ifstream arquivo("../pokemons.txt");

//     int id, vida, ataque, defesa;
//     string tipo, nomePokemon;
//     while (arquivo >> id >> tipo >> nomePokemon >> vida >> ataque >> defesa) {
//         for (int indice = 0; indice < 3; indice++) {
//             if (pokemons[indice] != id || equipe[indice] != nullptr) continue;

//             if (tipo == "fogo") {
//                 equipe[indice] = new PokemonFogo(nomePokemon, id, vida, ataque, defesa);
//             } else if (tipo == "agua") {
//                 equipe[indice] = new PokemonAgua(nomePokemon, id, vida, ataque, defesa);
//             } else if (tipo == "planta") {
//                 equipe[indice] = new PokemonPlanta(nomePokemon, id, vida, ataque, defesa);
//             } else if (tipo == "eletrico") {
//                 equipe[indice] = new PokemonEletrico(nomePokemon, id, vida, ataque, defesa);
//             }
//         }
//     }
// }
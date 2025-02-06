/*
Membros -> Caio Arthur Lima Domingos e João Vitor Reis Alvarenga.
Tema -> Pokemon.
*/

#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

struct pokemon {
  int id;
  char nome[16];
  char tipo[32];
  int total;
  int hp;
  int ataque;
  int defesa;
  int spataque;
  int spdefesa;
  int speed;
  int geracao;
  bool lendario;
};

void limpar_buffer();
int validar_inteiro(const char *mensagem, int min, int max);
bool validar_tipo(const char *tipo);
void validar_tipos(char *destino);
void validar_nome(pokemon *pokedex, int tamPokedex, char *nome, int tamanho);
bool inicializar_pokedex(pokemon *&pokedex, int &id, int &tamPokedex,
                        int &tamMax, int margem, string &cabecalho,
                        const string &nomeArquivo);
bool inicializar_pokedex_binario(pokemon *&pokedex, int &id, int &tamPokedex,
                                 int &tamMax, int margem,
                                 const string &nomeArquivoBinario);
void adicionar(pokemon *&pokedex, int &id, int &tamPokedex, int &tamMax,
               int margem);
void excluir(pokemon *&pokedex, int &tamPokedex, int &tamMax, int margem,
             int idMax);
void editar(pokemon *pokedex, int tamPokedex, int idMax);
void salva_arquivo(pokemon *pokedex, int tamPokedex, string cabecalho);
void salva_arquivo_binario(pokemon *pokedex, int tamPokedex);
pokemon *busca_binaria_nome(pokemon *pokedex, int inicio, int fim, const char *nome);
pokemon *busca_binaria_id(pokemon *pokedex, int inicio, int fim, int id);
void quicksort(pokemon *pokedex, int inicio, int fim, int campo,
               bool crescente);
bool compara(pokemon x, pokemon y, int campo, bool crescente);
string pokemon_to_string(pokemon &pokemon);
void exibirAscii();
void exibir_menu();

void limpar_buffer() {
  cin.clear();
  cin.ignore(2, '\n');
}

int validar_inteiro(const char *mensagem, int min, int max) {
  int valor;
  bool valido = false;
  do {
    cout << mensagem;
    cin >> valor;

    if (cin.fail() || valor < min || valor > max) {
      cout << "Valor inválido. Valores aceitos " << min << " - " << max << endl;
      limpar_buffer();
    } else {
      valido = true;
    }
  } while (!valido);

  return valor;
}

void validar_tipos(char *destino) {
  const char *tiposValidos[] = {
      "normal",   "lutador",  "voador", "veneno", "terra",   "pedra",
      "inseto",   "fantasma", "ferro",  "fogo",   "agua",    "grama",
      "eletrico", "psiquico", "gelo",   "dragao", "sombrio", "fada"};

  char tipo1[16];
  char tipo2[16];
  bool valido = false;

  do {
    cout << "Tipo (até dois tipos separados por espaço, ex: 'fogo agua'): ";
    cin.getline(destino, 32);

    int cont = sscanf(destino, "%15s %15s", tipo1, tipo2);
    if (cont == 1) {
      valido = false;
      for (int i = 0; i < 18; i++) {
        if (strcmp(tipo1, tiposValidos[i]) == 0) {
          valido = true;
        }
      }
    } else if (cont == 2) {
      bool tipo1Valido = false, tipo2Valido = false;
      for (int i = 0; i < 18; i++) {
        if (strcmp(tipo1, tiposValidos[i]) == 0) {
          tipo1Valido = true;
        }
        if (strcmp(tipo2, tiposValidos[i]) == 0) {
          tipo2Valido = true;
        }
      }

      if (tipo1Valido && tipo2Valido && strcmp(tipo1, tipo2) != 0) {
        valido = true;
      } else {
        cout << "Tipos inválidos ou iguais. Por favor, insira dois tipos "
                "válidos e distintos."
             << endl;
      }
    }

    if (!valido) {
      cout << "Tipo(s) inválido(s). Insira tipos válidos." << endl
           << "Tipos válidos: normal, lutador, voador, veneno, terra, pedra, "
              "inseto, fantasma, ferro, "
           << "fogo, agua, grama, eletrico, psiquico, gelo, dragao, sombrio, "
              "fada."
           << endl;
    }
  } while (!valido);
}

void validar_nome(pokemon *pokedex, int tamPokedex, char *destino) {
    bool valido;
    char nomeTemp[16];
    
    do {
        valido = true;
        cout << "Nome (até 15 caracteres): ";
        cin.ignore();
        cin.getline(nomeTemp, sizeof(nomeTemp));

        if (strlen(nomeTemp) < 0 || strlen(nomeTemp) > 15) {
            cout << "Nome inválido! Deve ter entre 1 e 15 caracteres.\n";
            valido = false;
            continue;
        }

        quicksort(pokedex, 0, tamPokedex - 1, 2, true); 
        pokemon* busca = busca_binaria_nome(pokedex, 0, tamPokedex - 1, nomeTemp);
        
        if (busca != nullptr && strcmp(nomeTemp, busca->nome) != 0) {
            cout << "Erro: Nome já existe na Pokedex!\n";
            valido = false;
        }

    } while (!valido);
    quicksort(pokedex, 0, tamPokedex - 1, 1, true);
    
    strncpy(destino, nomeTemp, 15);
    destino[15] = '\0';
}

bool inicializar_pokedex(pokemon *&pokedex, int &id, int &tamPokedex,
                         int &tamMax, int margem, string &cabecalho,
                         const string &nomeArquivo) {
  ifstream entrada(nomeArquivo);
  if (!entrada.is_open()) {
    cout << "Erro ao abrir o arquivo texto: " << nomeArquivo << endl;
    return false;
  }

  getline(entrada, cabecalho);
  entrada >> tamPokedex;
  id = tamPokedex;
  tamMax = tamPokedex + margem;
  pokedex = new pokemon[tamMax];

  for (int i = 0; i < tamPokedex; i++) {
    entrada >> pokedex[i].id;
    entrada.ignore();
    entrada.getline(pokedex[i].nome, sizeof(pokedex[i].nome), ',');
    entrada.getline(pokedex[i].tipo, sizeof(pokedex[i].tipo), ',');
    entrada >> pokedex[i].total;
    entrada.ignore();
    entrada >> pokedex[i].hp;
    entrada.ignore();
    entrada >> pokedex[i].ataque;
    entrada.ignore();
    entrada >> pokedex[i].defesa;
    entrada.ignore();
    entrada >> pokedex[i].spataque;
    entrada.ignore();
    entrada >> pokedex[i].spdefesa;
    entrada.ignore();
    entrada >> pokedex[i].speed;
    entrada.ignore();
    entrada >> pokedex[i].geracao;
    entrada.ignore();
    entrada >> pokedex[i].lendario;
  }

  entrada.close();

  cout << "Arquivo " << nomeArquivo << " lido!" << endl;
  return true;
}

bool inicializar_pokedex_binario(pokemon *&pokedex, int &id, int &tamPokedex,
                                 int &tamMax, int margem,
                                 const string &nomeArquivoBinario) {
  ifstream entrada(nomeArquivoBinario, ios::binary | ios::ate);
  if (!entrada) {
    cout << "Erro ao abrir o arquivo binario: " << nomeArquivoBinario << endl;
    return false;
  }

  streamsize tamanhoArquivo = entrada.tellg();
  entrada.seekg(0, ios::beg);

  tamPokedex = tamanhoArquivo / sizeof(pokemon);
  id = tamPokedex;
  tamMax = tamPokedex + margem;
  pokedex = new pokemon[tamMax];

  if (!entrada.read((char *)pokedex, tamanhoArquivo)) {
    cout << "Erro ao ler o arquivo binario!" << endl;
    delete[] pokedex;
    pokedex = nullptr;
    tamPokedex = 0;
    entrada.close();
    return false;
  }

  entrada.close();
  cout << "Arquivo binário " << nomeArquivoBinario << " lido com sucesso!"
       << endl;
  return true;
}

pokemon *busca_binaria_id(pokemon pokedex[],int comeco, int fim, int alvo){
  int meio = (comeco+fim)/2;

  if(pokedex[meio].id == alvo) return &pokedex[meio];
  if(comeco >= fim) return nullptr;

  return (pokedex[meio].id < alvo)?
  busca_binaria_id(pokedex,meio+1,fim,alvo):
  busca_binaria_id(pokedex,comeco,meio-1,alvo);
}

pokemon* busca_binaria_nome(pokemon pokedex[], int comeco, int fim, const char *alvo) {
    int meio = (comeco + fim) / 2;
    if (comeco > fim) return nullptr;

    if(strcmp(pokedex[meio].nome, alvo)==0) return &pokedex[meio];

    return (strcmp(pokedex[meio].nome, alvo) < 0)?
    busca_binaria_nome(pokedex, meio + 1, fim, alvo):
    busca_binaria_nome(pokedex, comeco, meio - 1, alvo);
}

void adicionar(pokemon *&pokedex, int &id, int &tamPokedex, int &tamMax,
               int margem) {
  if (tamPokedex >= tamMax) {
    tamMax += margem;
    pokemon *novoPokedex = new pokemon[tamMax];

    for (int i = 0; i < tamPokedex; i++) {
      novoPokedex[i] = pokedex[i];
    }

    delete[] pokedex;
    pokedex = novoPokedex;
  }

  pokemon novoPokemon;
  novoPokemon.id = ++id;

  cout << "Adicionando novo Pokémon com ID: " << novoPokemon.id << endl;
  limpar_buffer();

  validar_nome(pokedex, tamPokedex, novoPokemon.nome);
  validar_tipos(novoPokemon.tipo);

  novoPokemon.hp = validar_inteiro("HP (1-255): ", 1, 255);
  novoPokemon.ataque = validar_inteiro("Ataque (1-255): ", 1, 255);
  novoPokemon.defesa = validar_inteiro("Defesa (1-255): ", 1, 255);
  novoPokemon.spataque = validar_inteiro("SpAtaque (1-255): ", 1, 255);
  novoPokemon.spdefesa = validar_inteiro("SpDefesa (1-255): ", 1, 255);
  novoPokemon.speed = validar_inteiro("Speed (1-255): ", 1, 255);
  novoPokemon.geracao = validar_inteiro("Geração (1-8): ", 1, 8);
  novoPokemon.lendario = validar_inteiro("Lendário? (1 para sim, 0 para não): ", 0, 1);

  novoPokemon.total = novoPokemon.hp + novoPokemon.ataque + novoPokemon.defesa +
                      novoPokemon.spataque + novoPokemon.spdefesa +
                      novoPokemon.speed;

  pokedex[tamPokedex] = novoPokemon;
  tamPokedex++;
}

void excluir(pokemon *&pokedex, int &tamPokedex, int &tamMax, int margem,
             int idMax) {
  int idExcluir = validar_inteiro("Digite o ID do Pokémon a ser excluído: ", 1, idMax);

  int indice = -1;
  for (int i = 0; i < tamPokedex; i++) {
    if (pokedex[i].id == idExcluir) {
      indice = i;
    }
  }

  if (indice == -1) {
    cout << "Pokémon com ID " << idExcluir << " não encontrado." << endl;
    return;
  }

  for (int i = indice; i < tamPokedex - 1; i++) {
    pokedex[i] = pokedex[i + 1];
  }

  tamPokedex--;
  cout << "Pokémon com ID " << idExcluir << " foi excluído." << endl;

  if (tamPokedex < tamMax - margem) {
    tamMax -= margem;
    pokemon *novoPokedex = new pokemon[tamMax];

    for (int i = 0; i < tamPokedex; i++) {
      novoPokedex[i] = pokedex[i];
    }

    delete[] pokedex;
    pokedex = novoPokedex;
  }
}

void editar(pokemon *pokedex, int tamPokedex, int idMax) {
    int idEditar = validar_inteiro("Digite o ID do Pokémon a ser editado: ", 1, idMax);
    quicksort(pokedex, 0, tamPokedex - 1, 1, true); 
    pokemon *alvo = busca_binaria_id(pokedex, 0, tamPokedex - 1, idEditar);

    if (alvo == nullptr) {
        cout << "Pokémon não encontrado." << endl;
        return;
    }

    cout << "Editando Pokémon:\n" << pokemon_to_string(*alvo) << endl;
    
    validar_nome(pokedex, tamPokedex, alvo->nome);
    validar_tipos(alvo->tipo);

    alvo->hp = validar_inteiro("HP (1-255): ", 1, 255);
    alvo->ataque = validar_inteiro("Ataque (1-255): ", 1, 255);
    alvo->defesa = validar_inteiro("Defesa (1-255): ", 1, 255);
    alvo->spataque = validar_inteiro("SpAtaque (1-255): ", 1, 255);
    alvo->spdefesa = validar_inteiro("SpDefesa (1-255): ", 1, 255);
    alvo->speed = validar_inteiro("Speed (1-255): ", 1, 255);
    alvo->geracao = validar_inteiro("Geração (1-8): ", 1, 8);
    alvo->lendario = validar_inteiro("Lendário? (1 para sim, 0 para não): ", 0, 1);

    alvo->total = alvo->hp + alvo->ataque + alvo->defesa + alvo->spataque + alvo->spdefesa + alvo->speed;

    cout << "Pokémon atualizado com sucesso!" << endl;
}

bool compara(pokemon x, pokemon y, int campo, bool crescente) {
  switch (campo) {
    case 1:
      return crescente ? x.id < y.id : x.id > y.id;
      break;
    case 2:
      return crescente ? strcmp(x.nome, y.nome) < 0 : strcmp(x.nome, y.nome) > 0;
      break;
    case 3:
      return crescente ? x.total < y.total : x.total > y.total;
      break;
    case 4:
      return crescente ? x.hp < y.hp : x.hp > y.hp;
      break;
    case 5:
      return crescente ? x.ataque < y.ataque : x.ataque > y.ataque;
      break;
    case 6:
      return crescente ? x.defesa < y.defesa : x.defesa > y.defesa;
      break;
    case 7:
      return crescente ? x.spataque < y.spataque : x.spataque > y.spataque;
      break;
    case 8:
      return crescente ? x.spdefesa < y.spdefesa : x.spdefesa > y.spdefesa;
      break;
    case 9:
      return crescente ? x.speed < y.speed : x.speed > y.speed;
      break;
    default:
      return false;
      break;
  }
}

void quicksort(pokemon *pokedex, int inicio, int fim, int campo,
               bool crescente) {
  if (inicio > fim) return;

  int pivoIndex = inicio;
  pokemon pivo = pokedex[fim];

  for (int i = inicio; i < fim; i++) {
    if (compara(pokedex[i], pivo, campo, crescente)) {
      swap(pokedex[i], pokedex[pivoIndex]);
      pivoIndex++;
    }
  }

  swap(pokedex[pivoIndex], pokedex[fim]);

  quicksort(pokedex, inicio, pivoIndex - 1, campo, crescente);
  quicksort(pokedex, pivoIndex + 1, fim, campo, crescente);
}

string pokemon_to_string(pokemon &pokemon) {
  return "┌──────────────────────────────────────────\n"
         "ID: " + to_string(pokemon.id) + ", Nome: " + string(pokemon.nome) +
         ", Tipo: " + string(pokemon.tipo) +
         ", Total: " + to_string(pokemon.total) +
         ", HP: " + to_string(pokemon.hp) +
         ", Ataque: " + to_string(pokemon.ataque) +
         ", Defesa: " + to_string(pokemon.defesa) +
         ", SpAtaque: " + to_string(pokemon.spataque) +
         ", SpDefesa: " + to_string(pokemon.spdefesa) +
         ", Velocidade: " + to_string(pokemon.speed) +
         ", Geração: " + to_string(pokemon.geracao) +
         ", Lendário: " + (pokemon.lendario ? "Sim" : "Não") +
         " \n"
         "└──────────────────────────────────────────";
}

void salva_arquivo(pokemon *pokedex, int tamPokedex, string cabecalho) {
  cout << "Digite o nome do arquivo para salvar os dados em texto: ";
  string nomeArquivo;
  cin >> nomeArquivo;

  ofstream saida(nomeArquivo);

  if (cabecalho == "") {
    cabecalho =
        "numeroPokedex,nome,tipo,total,hp,ataque,defesa,spAtaque,spDefesa,"
        "velocidade,geracao,ehLendario";
  }

  saida << cabecalho << endl;
  saida << tamPokedex << endl;
  for (int i = 0; i < tamPokedex; i++) {
    saida << pokedex[i].id << "," << pokedex[i].nome << "," << pokedex[i].tipo
          << "," << pokedex[i].total << "," << pokedex[i].hp << ","
          << pokedex[i].ataque << "," << pokedex[i].defesa << ","
          << pokedex[i].spataque << "," << pokedex[i].spdefesa << ","
          << pokedex[i].speed << "," << pokedex[i].geracao << ","
          << pokedex[i].lendario << endl;
  }

  saida.close();
}

void salva_arquivo_binario(pokemon *pokedex, int tamPokedex) {
  cout << "Digite o nome do arquivo para salvar os dados em binario: ";
  string nomeArquivo;
  cin >> nomeArquivo;

  ofstream saida(nomeArquivo, ios::binary);

  saida.write((const char *)pokedex, sizeof(pokemon) * tamPokedex);
  saida.close();
}

void exibirAscii() {
  cout << R"(
                     ⢠⣤⣶⣿⣷⣤⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣶⣶⣾⡿⠃⠀⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢶⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠈⠛⠿⣿⣿⣿⣿⣿⣿⣧⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣷⣦⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀
⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣄⡀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡀⠀⠀
⢀⣾⣿⣿⣿⣿⣿⣿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⣀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⢀⣠⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀
⣼⣿⣿⣿⠟⠋⠁⠀⠀⠀⠈⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣠⣿⣿⣿⣿⣿⣿⣄⣤⣶⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀
⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⠟⠛⠛⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠻⣿⣿⣿⣿⣿⣿⡆
⢻⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠿⠋⠀⠀⣀⣴⣾⡿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⠀⠀⢰⣿⣿⠻⣿⣿⣿⣧
⠈⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣾⣿⣿⣇⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠈⢻⣿⣿⣿⣿⡿⠃⠀⠀⠀⢸⣿⣿⡆⠈⢿⣿⣿
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⢿⣿⡟⢿⣿⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⢀⣾⡿⣿⣿⠿⣿⠀⠀⠀⠀⣸⣿⣿⣷⠀⠸⣿⡿
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠀⠘⠉⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠀⠀⠙⠁⠸⠟⠀⠀⠀⠀⠀⢸⣿⣿⣿⣧⣄⠀⣿⠃
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⡀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⠿⠋⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⣠⣼⠟⠁⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⣀⣤⣴⣶⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠈⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⠟⠛⠛⠉⠉⠉⠉⠉⠛⠛⠿⠿⠿⠿⣿⣿⣿⣿⣿⣿⡋⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⢹⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠿⢿⣿⣿⣿⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⢹⡟⠙⠻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠈⠀
 _____    ____   _  __ ______  _____   ______ __   __
 |  __ \  / __ \ | |/ /|  ____||  __ \ |  ____|\ \ / /
 | |__) || |  | || ' / | |__   | |  | || |__    \ V /
 |  ___/ | |  | ||  <  |  __|  | |  | ||  __|    > <
 | |     | |__| || . \ | |____ | |__| || |____  / . \
 |_|      \____/ |_|\_\|______||_____/ |______|/_/ \_\
)" << endl;
}

void exibir_menu() {
  cout << "╔════════════════ POKEDEX ════════════════╗" << endl
       << "║ 1. Exibir Todos Pokémon                 ║" << endl
       << "║ 2. Adicionar Pokémon                    ║" << endl
       << "║ 3. Excluir Pokémon                      ║" << endl
       << "║ 4. Editar Pokémon                       ║" << endl 
       << "║ 5. Buscar Pokémon                       ║" << endl
       << "║ 6. Ordenar Pokémons                     ║" << endl
       << "║ 7. Salvar em arquivo                    ║" << endl
       << "║ 8. Exibir de X a Y                      ║" << endl
       << "║ 9. Carregar arquivo                     ║" << endl
       << "║ 10. Limpar Tela                         ║" << endl
       << "║ 0. Sair                                 ║" << endl
       << "╚═════════════════════════════════════════╝" << endl;
}

int main() {
  string cabecalho;
  int tamPokedex;
  int margem = 5;
  int tamMax;
  int id;
  pokemon *pokedex = nullptr;

  string nomeArquivoCSV = "Pokemon.csv";
  string nomeArquivoBinario = "Pokemon.bin";

  exibirAscii();

  if (!inicializar_pokedex_binario(pokedex, id, tamPokedex, tamMax, margem,
                                   nomeArquivoBinario))
    if (!inicializar_pokedex(pokedex, id, tamPokedex, tamMax, margem, cabecalho,
                             nomeArquivoCSV))
      cout << "Nenhum arquivo encontrado, informe um arquivo para ser lido "
              "(opção 8)"
           << endl;

  int opcao = 0;
  do {
    exibir_menu();
   opcao = validar_inteiro("Escolha uma opção de 0-10: ",0,10);

    switch (opcao) {
      case 1: {
        for (int i = 0; i < tamPokedex; i++) {
          cout << pokemon_to_string(pokedex[i]) << endl;
        }
        break;
      }

      case 2: {
        adicionar(pokedex, id, tamPokedex, tamMax, margem);
        break;
      }

      case 3: {
        excluir(pokedex, tamPokedex, tamMax, margem, id);
        break;
      }

      case 4: {
        editar(pokedex, tamPokedex, id);
        break;
      }

      case 5: {
        bool opcaoBusca =
            validar_inteiro("Deseja buscar por (0) Nome ou (1) Id? ", 0, 1);

        if (opcaoBusca == 1) {
          int idBusca = validar_inteiro("Digite o ID do Pokemon: ", 1, id);
          quicksort(pokedex, 0, tamPokedex - 1, 1, true);
          pokemon *busca = busca_binaria_id(pokedex, 0, tamPokedex - 1, idBusca);
          busca == nullptr ? cout << "Pokemon nao encontrado." << endl
                           : cout << pokemon_to_string(*busca) << endl;
        } else {
          char nome[16];
          cout << "Digite o Nome do Pokemon: ";
          cin.ignore();
          cin.getline(nome, sizeof(nome));
          quicksort(pokedex, 0, tamPokedex - 1, 2, true);
          pokemon *busca = busca_binaria_nome(pokedex, 0, tamPokedex - 1, nome);
          busca == nullptr ? cout << "Pokemon nao encontrado." << endl
                           : cout << pokemon_to_string(*busca) << endl;
          quicksort(pokedex, 0, tamPokedex - 1, 1, true);
        }

        break;
      }
      case 6: {
        int campo = validar_inteiro(
            "Por qual campo deseja ordenar? (opções: (1) id, (2) nome, (3) "
            "total, (4) hp, (5) ataque, (6) defesa, (7) spataque, (8) "
            "spdefesa, "
            "(9) speed): ",
            1, 9);
        bool crescente = validar_inteiro(
            "Deseja ordenar de forma crescente? (1 para sim, 0 para não): ", 0,
            1);

        quicksort(pokedex, 0, tamPokedex - 1, campo, crescente);

        cout << "Pokémons ordenados com sucesso!" << endl;
        break;
      }

      case 7: {
        bool opcaoSalvar = validar_inteiro(
            "Deseja salvar em um arquivo csv(0) ou binario(1)? ", 0, 1);
        if (opcaoSalvar) {
          salva_arquivo_binario(pokedex, tamPokedex);
        } else {
          salva_arquivo(pokedex, tamPokedex, cabecalho);
        }
        break;
      }

      case 8: {
        int comeco = validar_inteiro(
            "Digite o elemento que voce deseja comecar: ", 1, tamPokedex - 1);
        int fim = validar_inteiro(
            "Agora digite o elemento que voce deseja terminar: ", 2,
            tamPokedex);

        for (int i = comeco - 1; i < fim; i++) {
          cout << pokemon_to_string(pokedex[i]) << endl;
        }

        break;
      }
      case 9: {
        bool opcaoArquivo = validar_inteiro(
            "Deseja carregar um arquivo csv(0) ou um arquivo binario(1)? ", 0,
            1);
        cout << "Digite o nome do arquivo: ";
        char nomeArquivo[32];
        limpar_buffer();
        cin.getline(nomeArquivo, sizeof(nomeArquivo));

        if (opcaoArquivo) {
          inicializar_pokedex_binario(pokedex, id, tamPokedex, tamMax, margem,
                                      nomeArquivo);
        } else {
          inicializar_pokedex(pokedex, id, tamPokedex, tamMax, margem,
                              cabecalho, nomeArquivo);
        }

        break;
      }
      case 10: {
        system("clear||cls");
        exibirAscii();
        break;
      }
      case 0:
        cout << "Encerrando o programa. Até logo!" << endl;
        break;
      default:
        cout << "Opcao invalida! Tente novamente." << endl;
    }

  } while (opcao != 0);

  delete[] pokedex;
  return 0;
}
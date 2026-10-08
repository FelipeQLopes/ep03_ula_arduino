#include <iostream> // Biblioteca padrão para entrada/saída no terminal (cout, cin)
#include <fstream>  // Biblioteca para manipulação de arquivos (leitura ifstream / escrita ofstream)
#include <string>   // Biblioteca para usar e manipular textos da classe std::string
#include <unordered_map> // Estrutura de dados Tabela Hash para mapear chaves (strings) para valores (inteiros) com busca O(1)

// Criação de um dicionário (Hash Map) que vincula o nome legível da operação em Assembly com um ID inteiro.
// Note que esses IDs são mapeados de 1 a 16 aqui, e no código virarão de 0 a 15 ao subtrair 1.
std::unordered_map<std::string, int> operacao = 
    {   
        {"nA"    ,  1},// not A
        {"AoBn"  ,  2},// A and not B
        {"nAeB"  ,  3},// not A and B
        {"zeroL" ,  4},// zero
        {"AeBn"  ,  5},// A and not B
        {"nB"    ,  6},// not B
        {"AxB"   ,  7},// A and B
        {"AenB"  ,  8},// A and not B
        {"nAoB"  ,  9},// not A or B
        {"AxBn"  , 10},// A and not B
        {"copiaB", 11},// copy B
        {"AeB"   , 12},// A and B
        {"umL"   , 13},// one
        {"AonB"  , 14},// A or not B
        {"AoB"   , 15},// A or B
        {"copiaA", 16}// copy A
    };

// Protótipos das funções dos modos de execução
void mode0();
void mode1();

int main(){
    int mode;

    // Interface de linha de comando para o usuário escolher o modo
    std::cout << "Selecione o modo\n" << 
    "0. Interrompe o programa quando ocorre erro\n" <<
    "1. Não interrompe o programa quando ocorre erro\n";

    std::cin >> mode; // Lê a escolha do usuário

    // Encaminha a execução baseada na escolha
    if(mode == 0){
        mode0();
    }else if(mode == 1){
        mode1();
    }
    return 0; // Encerra o programa principal com sucesso
    
}

void mode0(){
    std::ifstream in("data/testeula.ula"); // Abre o arquivo-fonte (.ula) para LEITURA
    std::ofstream out("data/testeula.hex"); // Cria ou sobrescreve o arquivo de destino (.hex) para ESCRITA
    int cont = 0; // Contador de linhas para facilitar a depuração (informar onde o erro ocorreu)
    int x, y, w, fimLinha; // Variáveis para os operandos, operação, e marcador de fim de string
    std::string valor; // String que guardará o valor extraído da linha
    
    // Loop de leitura: Lê o arquivo linha por linha até o fim
    for(std::string line; std::getline(in, line); ){
        cont++; // Incrementa o contador da linha atual
        if(line.empty()){ // Verifica se a linha do arquivo está vazia
            std::cout << "Erro: linha vazia, linha: " << cont << "\n";
            return; // Interrompe IMEDIATAMENTE (comportamento do Modo 0)
        }
        fimLinha = line.find(";") - 2; // O formato esperado das linhas é algo como "X=5;" O find(";") localiza o índice do ponto e vírgula. Subtraindo 2 ele calcula quantos caracteres tem o dado isolado.
        valor = line.substr(2, fimLinha); // A função substr corta a string. Ignora "X=" (os 2 primeiros caracteres) e pega exatamente o valor.
        
        switch (line[0]){ // Olha a primeira letra da linha para saber qual dado está sendo carregado
            case 'X': // Se a linha começar com X
                x = std::stoi(valor); // Converte a string numérica extraída para um inteiro e guarda em 'x'
                break;

            case 'Y': // Se a linha começar com Y
                y = std::stoi(valor); // Converte a string para inteiro e guarda em 'y'
                break;

            case 'W': // Se a linha for W, é a instrução da operação, o que significa que os dados de X e Y já foram lidos.
                w = operacao[valor] - 1; // Busca a string de instrução no Hash Map, e subtrai 1 (para mapear de 0 até 15). Ex: operacao["AoB"] é 15, logo w = 14 (letra E).
                if(w == -1){ // Se a string não for achada no map, ele por padrão alocará como se fosse int=0, logo 0-1 = -1. Indica erro de sintaxe.
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    return; // Aborta a execução (Modo 0)
                }
                
                // Grava no arquivo .hex: A flag std::hex transforma os inteiros em base 16, e uppercase deixa as letras maiúsculas.
                // Aqui ele junta os 3 registradores numa string só. Ex: X=10, Y=15, W=14 vira a string "AFE" gravada no arquivo.
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default: // Se a linha começar com qualquer outra coisa, considera como fim ou quebra de padrão
                std::cout << "Fim do Programa\n";
                return; // Encerra tudo
        }
    }
}

// O mode1 é essencialmente igual ao mode0 em lógica de conversão. 
// A única diferença é a tolerância a falhas.
void mode1(){
    std::ifstream in("data/testeula.ula");
    std::ofstream out("data/testeula.hex");
    int cont = 0;
    int x, y, w, fimLinha;
    std::string valor;
    for(std::string line; std::getline(in, line); ){
        cont++;
        if(line.empty()){
            std::cout << "Erro: linha vazia, linha: " << cont << "\n";
            continue; // DIFFERENÇA: Usa 'continue' para pular para a próxima iteração do loop, sem parar o programa todo.
        }
        fimLinha = line.find(";") - 2;
        valor = line.substr(2, fimLinha);
        switch (line[0]){
            case 'X':
                x = std::stoi(valor);
                break;

            case 'Y':
                y = std::stoi(valor);
                break;

            case 'W':
                w = operacao[valor] - 1;
                if(w == -1){
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    break; // DIFFERENÇA: Aqui ele apenas sai do 'switch' com 'break', não aborta a leitura do arquivo inteiro com 'return'.
                }
                
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default:
                std::cout << "Fim do Programa\n";
                // DIFFERENÇA: Não tem o 'return' que havia no mode0. Ele avisa o fim mas tecnicamente deixaria o loop terminar por si só.
        }
    }
}

// Lê o programa fonte "testeula.ula" (X=..; Y=..; W=mnemônico;)
// e gera "testeula.hex", onde cada linha tem 3 dígitos hex: X Y S
// Exemplo: X=12; Y=6; W=AeB;  ->  C6B

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>  

// Tabela de mnemônicos
std::unordered_map<std::string, int> operacao = 
    {   
        {"nA"    ,  1},   // not a
        {"AoBn"  ,  2},   // not (a or b)
        {"nAeB"  ,  3},   // not a and b
        {"zeroL" ,  4},   // 0
        {"AeBn"  ,  5},   // not (a and b)
        {"nB"    ,  6},   // not b
        {"AxB"   ,  7},   // a xor b
        {"AenB"  ,  8},   // a and not b
        {"nAoB"  ,  9},   // not a or b
        {"AxBn"  , 10},   // not (a xor b)
        {"copiaB", 11},   // copy b
        {"AeB"   , 12},   // a and b
        {"umL"   , 13},   // 1
        {"AonB"  , 14},   // a or not b
        {"AoB"   , 15},   // a or b
        {"copiaA", 16}    // copy a
    };

// Variável global: true se os números do .ula estão em hexadecimal,
// false se estão em decimal. Global para a hexOuInt() enxergar a escolha.
bool isHex;

// Declarações antecipadas (protótipos), para o main poder chamá-las
// antes de elas serem definidas lá embaixo.
void mode0(); // modo que PARA no primeiro erro
void mode1();  // modo que mostra o erro e CONTINUA
int hexOuInt(std::string valor); // converte texto ("12" ou "C") em número

int main(){
    int mode; // guarda o modo escolhido (0 ou 1)

    // Menu: pergunta como tratar erros no arquivo fonte
    std::cout << "Selecione o modo\n" << 
    "0. Interrompe o programa quando ocorre erro\n" <<
    "1. Não interrompe o programa quando ocorre erro\n";

    std::cin >> mode; // lê a escolha do usuário

    int hexOuDecimal; // guarda se a entrada é decimal (0) ou hexa (1)

    // Menu: pergunta em que base os valores de X e Y foram escritos
    std::cout << "Agora selecione qual o tipo da entrada\n" << 
    "0. A entrada está em Decimal (0,1,2,...,14,15)\n" <<
    "1. A entrada está em Hexadecimal (0,1,2,...,E,F)\n";

    std::cin >> hexOuDecimal; // lê a escolha da base

    isHex = hexOuDecimal == 1; // vira true só se o usuário digitou 1

    if(mode == 0){ // modo "para no erro"
        mode0();
    }else if(mode == 1){ // modo "continua mesmo com erro"
        mode1();
    }
    // (se digitar outro número, nada é executado)
    return 0; // fim do programa, sem erro
    
}


// MODO 0: converte o .ula em .hex e ENCERRA ao achar o 1º erro

void mode0(){
    std::ifstream in("data/testeula.ula");   // abre o arquivo fonte para leitura
    std::ofstream out("data/testeula.hex");  // cria/sobrescreve o arquivo de saída
    int cont = 0; // contador de linhas (para informar onde está o erro)
    int x, y, w, fimLinha; // x e y: entradas; w: código da instrução; fimLinha: tamanho do valor
    std::string valor; // texto entre o "=" e o ";" (ex: "12" ou "AeB")

    // Lê o arquivo linha a linha até acabar
    for(std::string line; std::getline(in, line); ){
        if(valor.substr(0,valor.size()-1).compare("inicio:") == 0) continue;
        cont++; // conta a linha atual
        if(line.empty()){ // tratamento de linha vazia
            std::cout << "Erro: linha vazia, linha: " << cont << "\n";
            return; // modo 0: interrompe tudo
        }
        // Linha tem formato "X=12;": posição 0 = letra, 1 = '=', valor começa na 2.
        // find(";") dá a posição do ';'. Subtraindo 2 sobra o tamanho do valor.
        // Ex: "X=12;" -> find=4 -> 4-2 = 2 caracteres ("12").
        fimLinha = line.find(";") - 2;
        valor = line.substr(2, fimLinha); // extrai o valor (ou mnemônico) da linha
        switch (line[0]){ // a 1ª letra diz o que a linha faz
            case 'X':  // linha de atribuição de X
                x = hexOuInt(valor); // converte o texto em número e guarda
                break;

            case 'Y': // linha de atribuição de Y
                y = hexOuInt(valor); // converte o texto em número e guarda
                break;

            case 'W': // linha de instrução: executa a operação
                w = operacao[valor] - 1;  // busca o código; (0 - 1) = -1 se não existir
                if(w == -1){ // mnemônico não encontrado = sintaxe errada
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    return; // modo 0: interrompe tudo
                }
                
                // Grava no .hex os 3 dígitos hexa maiúsculos: X, Y e instrução.
                // Ex: x=12, y=6, w=11 -> "C6B"
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default: // qualquer outra linha (ex "fim.")
                std::cout << "Fim do Programa\n";
                return; // encerra a conversão
        }

        
    }
}


// MODO 1: igual ao modo 0, mas só AVISA os erros e segue em frente
void mode1(){
    std::ifstream in("data/testeula.ula"); // abre o arquivo fonte para leitura
    std::ofstream out("data/testeula.hex"); // cria/sobrescreve o arquivo de saída
    int cont = 0; // contador de linhas
    int x, y, w, fimLinha; // mesmas variáveis do modo 0
    std::string valor; // texto entre o "=" e o ";"
    for(std::string line; std::getline(in, line); ){ // lê linha a linha
        cont++; // conta a linha atual
        if(line.empty()){ // linha em branco: avisa e pula para a próxima
            std::cout << "Erro: linha vazia, linha: " << cont << "\n";
            continue; // diferente do modo 0, NÃO encerra
        }
        fimLinha = line.find(";") - 2; // tamanho do valor 
        valor = line.substr(2, fimLinha); // extrai o valor ou mnemônico (ex: W=AeB; o ; está na posição 5, 5 - 2 = 3, e substr(2, 3) devolve AeB.)
        switch (line[0]){ // decide o que fazer pela 1ª letra
            case 'X':
                x = hexOuInt(valor); // converte valor em número e guarda em x
                break;

            case 'Y':
                y = hexOuInt(valor); // converte valor em número e guarda em y
                break;

            case 'W':
                w = operacao[valor] - 1; // código da instrução (-1 = inválida) (-1 pois nossos minemonicos começam em 1)
                if(w == -1){  // mnemônico inexistente
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    break; // avisa, não grava nada e vai para a próxima linha
                }
                
                // Escreve "X Y S" em hexa maiúsculo no arquivo .hex
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default:
                std::cout << "Fim do Programa\n";
        }

        
    }

}

// Converte o texto de X ou Y em número (0 a 15), conforme a base escolhida no menu
int hexOuInt(std::string valor){
    int resposta; // número final que será devolvido
    if(isHex){
        // Hexa: olha só o 1º caractere (1 dígito hexa = 4 bits = 0..15)
        resposta = (int)valor[0]; // pega o código ASCII do caractere
        if(resposta >= 65){ // 65 = 'A' na tabela ASCII: é letra (A..F)
            resposta -= 55; // 'A'(65) - 55 = 10, 'B' = 11 ... 'F' = 15
        }else{ // senão é dígito '0'..'9'
            resposta -= 48; // '0'(48) - 48 = 0, '1' = 1 ... '9' = 9
        }
        // Obs: letras minúsculas ('a'..'f') dariam valor errado; use maiúsculas.
    }else{
        resposta = std::stoi(valor); // decimal: converte o texto inteiro ("12" -> 12)
    }
    
    return resposta; // devolve o número já convertido
}
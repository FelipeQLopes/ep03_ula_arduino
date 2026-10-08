#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> operacao = 
    {   
        {"nA"    ,  1},
        {"AoBn"  ,  2},
        {"nAeB"  ,  3},
        {"zeroL" ,  4},
        {"AeBn"  ,  5},
        {"nB"    ,  6},
        {"AxB"   ,  7},
        {"AenB"  ,  8},
        {"nAoB"  ,  9},
        {"AxBn"  , 10},
        {"copiaB", 11},
        {"AeB"   , 12},
        {"umL"   , 13},
        {"AonB"  , 14},
        {"AoB"   , 15},
        {"copiaA", 16}
    };

bool isHex;

void mode0();
void mode1();
int hexOuInt(std::string valor);

int main(){
    int mode;

    std::cout << "Selecione o modo\n" << 
    "0. Interrompe o programa quando ocorre erro\n" <<
    "1. Não interrompe o programa quando ocorre erro\n";

    std::cin >> mode;

    int hexOuDecimal;

    std::cout << "Agora selecione qual o tipo da entrada\n" << 
    "0. A entrada está em Decimal (0,1,2,...,14,15)\n" <<
    "1. A entrada está em Hexadecimal (0,1,2,...,E,F)\n";

    std::cin >> hexOuDecimal;

    isHex = hexOuDecimal == 1;

    if(mode == 0){
        mode0();
    }else if(mode == 1){
        mode1();
    }
    return 0;
    
}

void mode0(){
    std::ifstream in("data/testeula.ula");
    std::ofstream out("data/testeula.hex");
    int cont = 0;
    int x, y, w, fimLinha;
    std::string valor;

    for(std::string line; std::getline(in, line); ){
        if(valor.substr(0,valor.size()-1).compare("inicio:") == 0) continue;
        cont++;
        if(line.empty()){
            std::cout << "Erro: linha vazia, linha: " << cont << "\n";
            return;
        }
        fimLinha = line.find(";") - 2;
        valor = line.substr(2, fimLinha);
        switch (line[0]){
            case 'X':
                x = hexOuInt(valor);
                break;

            case 'Y':
                y = hexOuInt(valor);
                break;

            case 'W':
                w = operacao[valor] - 1;
                if(w == -1){
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    return;
                }
                
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default:
                std::cout << "Fim do Programa\n";
                return;
        }

        
    }
}

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
            continue;
        }
        fimLinha = line.find(";") - 2;
        valor = line.substr(2, fimLinha);
        switch (line[0]){
            case 'X':
                x = hexOuInt(valor);
                break;

            case 'Y':
                y = hexOuInt(valor);
                break;

            case 'W':
                w = operacao[valor] - 1;
                if(w == -1){
                    std::cout << "Erro: instrução inválida (" << valor <<"), linha: " << cont << "\n";
                    break;
                }
                
                out << std::hex << std::uppercase << x << y << w << "\n";
                break;

            default:
                std::cout << "Fim do Programa\n";
        }

        
    }

}

int hexOuInt(std::string valor){
    int resposta;
    if(isHex){
        resposta = (int)valor[0];
        if(resposta >= 65){
            resposta -= 55;
        }else{
            resposta -= 48;
        }
    }else{
        resposta = std::stoi(valor);
    }
    
    return resposta;
}
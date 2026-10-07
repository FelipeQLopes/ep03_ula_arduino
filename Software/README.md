# Software/ — Tradutor `.ula` → `.hex` (lado do PC)

## Propósito

Esta pasta contém o programa em **C++** que roda no computador. Ele lê um programa-fonte com mnemônicos (`.ula`), valida as linhas, converte cada operação em um código hexadecimal de 3 dígitos e grava o resultado em um arquivo `.hex`, que depois é enviado ao Arduino.

## O que há aqui

| Item | Descrição |
|---|---|
| `software.cpp` | Código-fonte do tradutor |
| `Makefile` | Compila (`make`) e executa (`make run`) o programa |
| `data/` | Arquivos que o programa lê e escreve (veja o [README de data](data/README.md)) |

O executável gerado (`software.exe`) é ignorado pelo git (ver `.gitignore` na raiz).

## Como o código funciona

### A tabela de mnemônicos
Um `unordered_map<string,int>` associa cada mnemônico a um número de **1 a 16**. O código subtrai 1 para obter o valor hexadecimal da instrução (0 a F), na mesma ordem da tabela do enunciado:

```
nA→1   AoBn→2   nAeB→3   zeroL→4   AeBn→5   nB→6   AxB→7   AenB→8
nAoB→9   AxBn→10   copiaB→11   AeB→12   umL→13   AonB→14   AoB→15   copiaA→16
```

Como o `map` devolve `0` para chaves inexistentes, `0 - 1 = -1` sinaliza **mnemônico inválido**.

### Leitura linha a linha
Para cada linha de `data/testeula.ula`:

| Linha | Ação |
|---|---|
| vazia | Mostra `Erro: linha vazia, linha: N` |
| `X=valor;` | Guarda o valor decimal de X |
| `Y=valor;` | Guarda o valor decimal de Y |
| `W=mnemônico;` | Busca o mnemônico. Se válido, grava `X Y S` em hexadecimal no `.hex`. Se inválido, mostra `Erro: instrução inválida (...), linha: N` |
| qualquer outra | Cai no `default` e mostra `Fim do Programa` |

Exemplo: `X=12;` `Y=6;` `W=AeB;` gera a linha `C6B` no `.hex`.

### Os dois modos de erro (`main`)

| Modo | Função | Comportamento diante de um erro |
|---|---|---|
| `0` | `mode0()` | Informa o erro e **interrompe** a tradução |
| `1` | `mode1()` | Informa o erro e **continua** com as demais linhas |

O modo 1 é o que atende ao requisito do enunciado: o erro é mostrado, mas a cópia do programa continua.

## Como usar

Execute **dentro da pasta `Software/`**, pois os caminhos dos arquivos são relativos (`data/testeula.ula`):

```sh
cd Software
make          # compila → gera software.exe
make run      # executa
```

O programa pergunta:
```
Selecione o modo
0. Interrompe o programa quando ocorre erro
1. Não interrompe o programa quando ocorre erro
```

Escolha `1` para ver todos os erros do arquivo de teste e gerar o `.hex` completo. O resultado fica em `data/testeula.hex`. O conteúdo desse arquivo deve ser colado no Monitor Serial do Arduino (ver `Arduino/`).

Requisitos: compilador C++ com suporte a C++17 (o `Makefile` usa `$(CXX) -std=c++17 -Wall -Wextra`).

## Roteiro de estudo sugerido
1. Abra `data/testeula.ula` e `data/testeula.hex` lado a lado e converta algumas linhas à mão.
2. Leia `mode1()` e veja como `substr`/`find` extraem o texto entre `=` e `;`.
3. Compare `mode0()` com `mode1()`: a diferença está no `return` (para) contra `break`/`continue` (segue).
4. Teste os erros: use `W=AeC;`, uma linha em branco, e compare os dois modos.

## Pontos de atenção ao estudar o código

Leituras do código-fonte, sem execução, para comparar com o enunciado:

- **Envio serial:** o programa **gera** o `.hex`, mas não escreve na porta serial. O envio é feito colando o conteúdo do arquivo no Monitor Serial.
- **Linhas `inicio:` e `fim.`:** o enunciado as usa no exemplo da Figura 3. Aqui, qualquer linha que não comece com `X`, `Y` ou `W` cai no `default` e imprime "Fim do Programa". No modo 0 isso encerra a leitura, e o arquivo de teste atual não usa essas linhas.
- **Sintaxe errada em `X=`/`Y=`:** só os mnemônicos de `W=` têm validação explícita. Um valor numérico inválido em `X=` ou `Y=` não é tratado.
- **Arquivos fixos:** os nomes `data/testeula.ula` e `data/testeula.hex` estão escritos no código.
# ULA de 4 bits com Arduino (Exercício Prático 03)

Implementação de uma **Unidade Lógica e Aritmética (ULA) de 4 bits** simulada em um Arduino, controlada por um programa no PC. Projeto acadêmico de Ciência da Computação (disciplina de Arquitetura de Computadores II).

O sistema é dividido em duas partes que conversam entre si:

- **Software no PC (C++):** lê um programa escrito com mnemônicos (`.ula`), traduz cada linha para hexadecimal e gera um arquivo `.hex`.
- **Hardware externo (Arduino):** recebe o conteúdo do `.hex` pela porta serial, guarda tudo em um vetor que simula a memória, executa as instruções uma a uma e mostra o resultado em 4 LEDs.

---

## Estrutura do Repositório

```
ep03_ula_arduino/
├── README.md                  ← você está aqui: visão geral do projeto
├── .gitignore                 ← ignora o executável compilado (Software/software.exe)
├── docs/                      ← enunciado oficial do exercício
│   └── EP03_2026_2.pdf
├── Arduino/                   ← código que roda no Arduino (a ULA)
│   └── ula_arduino.ino
├── Software/                  ← código que roda no PC (tradutor .ula → .hex)
│   ├── software.cpp
│   ├── Makefile
│   └── data/                  ← arquivos de entrada e saída do tradutor
│       ├── testeula.ula
│       └── testeula.hex
└── referencias/               ← respostas/versões de apoio (material de consulta)
    ├── Arquitetura de computadores _tp.pdf
    └── Arquitetura de computadores _tp_software.pdf
```

| Pasta | O que há nela | Quando consultar |
|---|---|---|
| [`docs/`](docs/) | PDF do enunciado (EP03 2026/2) | Para entender o que foi pedido e os critérios de avaliação |
| [`Arduino/`](Arduino/) | Sketch da ULA: memória, carga, execução, LEDs e DUMP | Para estudar o lado do hardware |
| [`Software/`](Software/) | Tradutor em C++ e Makefile | Para estudar o lado do PC |
| [`Software/data/`](Software/data/) | Programa de teste `.ula` e saída `.hex` | Para ver exemplos reais de entrada e saída |
| [`referencias/`](referencias/) | Duas respostas em PDF, com código comentado | Para comparar abordagens |

> **Por onde começar a estudar?** Leia `docs/` (o que foi pedido) → `Software/data/` (um exemplo concreto) → `Software/` (como o `.ula` vira `.hex`) → `Arduino/` (como o `.hex` é executado).

---

## Como as partes se conectam

```
 testeula.ula ──► [ software.cpp ] ──► testeula.hex ──► Monitor Serial ──► [ Arduino ] ──► 4 LEDs
 (mnemônicos)       (tradutor)         (hexadecimal)    (campo "Enviar")    (memória+ULA)   + DUMP
```

1. **Escrita:** o usuário escreve o programa em `Software/data/testeula.ula`, com linhas como `X=12;`, `Y=6;` e `W=AeB;`.
2. **Tradução:** `Software/software.cpp` converte cada operação `W=...` em **3 dígitos hexadecimais**: `X`, `Y` e o código da instrução `S`. Por exemplo, `X=12; Y=6; W=AeB;` vira `C6B`. O resultado vai para `testeula.hex`.
3. **Carga:** o conteúdo do `.hex` é colado no campo "Enviar" do Monitor Serial. O Arduino armazena cada instrução no vetor `memoria`, a partir do índice 4. Nada é executado nesta etapa.
4. **Confirmação:** após a carga, o Arduino mostra o vetor e pede que o usuário digite `S` para iniciar.
5. **Execução:** o ciclo **buscar → decodificar → executar** lê `memoria[PC]`, separa X, Y e S, calcula o resultado e grava X, Y e W na memória. O resultado aparece nos LEDs dos pinos 13 a 10, e a cada instrução há uma pausa de 4 segundos e um DUMP da memória.

### A memória simulada (vetor de 100 posições)

| Índice | Conteúdo |
|---|---|
| `0` | **PC**: índice da instrução atual (começa em 4) |
| `1` | **W**: resultado da última operação |
| `2` | **X**: último valor de X usado |
| `3` | **Y**: último valor de Y usado |
| `4` a `99` | **Programa**: instruções em hexadecimal (ex.: `C6B`) |

### O formato de uma instrução

Cada instrução tem 3 dígitos hexadecimais, na ordem **X, Y, S**:

```
  C 6 B
  │ │ └─ S = B → instrução AeB (AND)
  │ └─── Y = 6 (0110)
  └───── X = C (1100)   →   resultado W = 0100 = 4 → LED do pino 12 aceso
```

### Conjunto de instruções da ULA

| Mnemônico | Função | Hexa |   | Mnemônico | Função | Hexa |
|---|---|---|---|---|---|---|
| `nA` | A' | 0 |   | `AxBn` | A·B + A'·B' | 9 |
| `AoBn` | (A+B)' | 1 |   | `copiaB` | B | A |
| `nAeB` | A'·B | 2 |   | `AeB` | A·B | B |
| `zeroL` | 0 | 3 |   | `umL` | 1 | C |
| `AeBn` | (A·B)' | 4 |   | `AonB` | A + B' | D |
| `nB` | B' | 5 |   | `AoB` | A + B | E |
| `AxB` | A'·B + A·B' | 6 |   | `copiaA` | A | F |
| `AenB` | A·B' | 7 |   |   |   |   |
| `nAoB` | A' + B | 8 |   |   |   |   |

As operações sempre usam as variáveis **X** e **Y** e gravam o resultado em **W**.

### LEDs de saída

| Pino | Bit |
|---|---|
| 13 | F3 (mais significativo) |
| 12 | F2 |
| 11 | F1 |
| 10 | F0 (menos significativo) |

---

## Tratamento de erros

O programa `.ula` pode ter dois tipos de erro: **instrução com sintaxe errada** e **linha em branco**. O tradutor do PC informa o erro no console e oferece dois modos:

- **Modo 0:** interrompe a tradução no primeiro erro.
- **Modo 1:** informa o erro e continua traduzindo as demais linhas.

---

## O que foi entregue

- Sketch do Arduino com memória de 100 posições, carga prévia, confirmação para iniciar, ciclo de execução, 16 instruções, LEDs, atraso de 4 s e DUMP (`Arduino/`).
- Tradutor em C++ com tabela de mnemônicos, os dois modos de erro e geração do `.hex` (`Software/`).
- Programa de teste `.ula` com erros propositais e o `.hex` gerado (`Software/data/`).
- Simulação no TinkerCad, com o link no cabeçalho do código do Arduino.
- Material de apoio com duas respostas em PDF (`referencias/`).

## Como executar (resumo)

1. **PC:** `cd Software && make && make run`, depois escolha o modo (0 ou 1). Isso gera `data/testeula.hex`.
2. **Arduino:** abra `Arduino/ula_arduino.ino` na IDE (ou use o projeto do TinkerCad), configure o Monitor Serial em **9600 baud**, cole o conteúdo do `.hex` no campo "Enviar" e digite `S` quando for solicitado.

Instruções detalhadas estão no README de cada pasta.
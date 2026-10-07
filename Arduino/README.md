# Arduino/ — A ULA de 4 bits (hardware externo)

## Propósito

Esta pasta contém o programa que roda **no Arduino** e faz o papel da ULA: recebe as instruções pela porta serial, guarda-as em uma memória simulada, executa-as uma a uma e exibe o resultado em 4 LEDs e no Monitor Serial (DUMP).

## O que há aqui

| Arquivo | Descrição |
|---|---|
| `ula_arduino.ino` | Sketch completo. O link do projeto no **TinkerCad** (simulação do circuito) está no comentário do cabeçalho. |

## Como o código está organizado

### Variáveis globais

| Variável | Função |
|---|---|
| `String memoria[100]` | Memória simulada. `[0]`=PC, `[1]`=W, `[2]`=X, `[3]`=Y, `[4..99]`=programa |
| `int cont` | Próxima posição livre do vetor. Começa em 4 e cresce a cada instrução carregada |
| `bool iniciar` | "Trava" que só vira `true` depois da carga completa e da confirmação do usuário |
| `pin_f3 … pin_f0` | Pinos 13, 12, 11 e 10 (LEDs de saída) |

### Funções

| Função | O que faz |
|---|---|
| `setup()` | Zera W, X e Y, define PC = 4, abre a serial a 9600 baud e configura os pinos dos LEDs como saída |
| `loop()` | Controla as duas fases: **carga** e **execução** (veja abaixo) |
| `carregarEntradas()` | Lê da serial, separando as entradas por **espaço**, e grava cada uma em `memoria[cont++]` |
| `printVetor()` | Imprime o DUMP no formato `->\|PC\|W\|X\|Y\|instr1\|instr2\|…\|`, só até `cont` |
| `calcularInstrucao(x, y, w)` | A ULA: um `switch` sobre o código da instrução (`'0'` a `'F'`) que aplica a operação aos 4 bits de X e Y |
| `conversaoCharInt(c)` / `conversaoIntChar(i)` | Convertem entre o caractere hexadecimal e o valor inteiro |

### Fluxo do `loop()`

```
Fase 1 – CARGA (cont == 4 e há dados na serial)
   └─ carregarEntradas() → printVetor() → pede "Digite S para iniciar"
        └─ 'S' ou 's' → iniciar = true

Fase 2 – EXECUÇÃO (iniciar == true e cont > PC)
   ├─ BUSCA:      instrucao = memoria[memoria[0]]
   ├─ DECODIFICA: x = instrucao[0], y = instrucao[1], w = instrucao[2]
   ├─ EXECUTA:    s = calcularInstrucao(x, y, w)
   ├─ ESCREVE:    memoria[2]=X, memoria[3]=Y, memoria[1]=W
   ├─ MOSTRA:     printVetor() + digitalWrite nos pinos 13..10
   └─ delay(4000) → PC = PC + 1
```

A execução termina quando o PC alcança `cont` (não há mais instruções).

## Como usar

### Opção A: simulação no TinkerCad
1. Abra o link que está no cabeçalho de `ula_arduino.ino`.
2. Inicie a simulação e abra o **Monitor Serial**.
3. Cole as instruções (veja "Formato da entrada") e envie.
4. Digite `S` quando o programa pedir.

### Opção B: placa real
1. Abra `ula_arduino.ino` na IDE do Arduino (ela pode pedir para criar uma pasta com o mesmo nome do arquivo).
2. Ligue 4 LEDs (com resistores) nos pinos 13, 12, 11 e 10.
3. Carregue o sketch e abra o Monitor Serial em **9600 baud**.
4. Cole as instruções, envie e digite `S`.

### Formato da entrada
Cada instrução tem 3 dígitos hexadecimais (`X Y S`), por exemplo `C6B`. O código separa as entradas por **espaço**:

```
C6B A3E
```

O conteúdo vem do arquivo `Software/data/testeula.hex` (gerado pelo tradutor do PC).

### Exemplo de saída esperada (programa `C6B A3E`)
```
->|4|0|0|0|C6B|A3E|
Digite S para iniciar o programa:
Programa iniciado!
->|4|4|C|6|C6B|A3E|
->|5|B|A|3|C6B|A3E|
```
O LED do pino 12 acende na 1ª instrução (W = 4 = 0100). Os LEDs dos pinos 13, 11 e 10 acendem na 2ª (W = B = 1011).

## Roteiro de estudo sugerido
1. Leia `calcularInstrucao()` e confira cada `case` com a tabela de instruções (README da raiz).
2. Siga o `loop()` com o exemplo `C6B A3E` e veja como `memoria[0]` (PC) avança.
3. Observe como `& 0x0F` mantém tudo em 4 bits depois de operações com `~`.
4. Observe como `(s >> n) & 1` extrai cada bit para o LED correspondente.

## Pontos de atenção ao estudar o código

Leituras do código-fonte, sem execução, para comparar com o enunciado:

- **`case '9'` (AxBn):** a expressão usa `(x & y) | (~x | ~y)`. O XNOR seria `(x & y) | (~x & ~y)`. Do jeito que está, o resultado tende a ser sempre `F`.
- **PC no DUMP:** o `printVetor()` roda *antes* do incremento do PC, então cada linha mostra o PC da instrução que acabou de executar. Nos exemplos do enunciado, o PC já aparece incrementado.
- **Separador de entrada:** a carga separa por espaço (`readStringUntil(' ')`), enquanto o `.hex` gerado tem uma instrução por linha. Vale conferir como o texto colado chega ao Arduino.
- **Letras minúsculas:** `conversaoCharInt` assume hexadecimal maiúsculo (`A`–`F`).
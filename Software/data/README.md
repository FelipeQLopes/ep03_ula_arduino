# Software/data/ — Arquivos de entrada e saída do tradutor

## Propósito

Esta pasta guarda os dois arquivos que o programa `software.cpp` usa: o **programa-fonte de teste** (`.ula`, entrada) e o **programa gerado** (`.hex`, saída). Eles permitem ver, na prática, a transformação de mnemônicos em hexadecimal.

## O que há aqui

| Arquivo | Papel | Descrição |
|---|---|---|
| `testeula.ula` | Entrada | Programa de teste escrito com mnemônicos, no formato da Figura 3 do enunciado |
| `testeula.hex` | Saída | Instruções em hexadecimal geradas pelo tradutor, uma por linha (Figura 4) |

## Formato do `.ula`

```
X=5;        ← define X (decimal)
Y=3;        ← define Y (decimal)
W=nA;       ← executa a instrução nA sobre X e Y
```

- `X` e `Y` **permanecem** com o último valor atribuído. Por isso uma linha `W=...` pode vir logo após outra `W=...`.
- Cada `W=...` produz **uma** linha no `.hex`.

## Formato do `.hex`

Cada linha tem 3 dígitos hexadecimais: **X, Y, S**.

| Trecho do `.ula` | Linha no `.hex` | Leitura |
|---|---|---|
| `X=5;` `Y=3;` `W=nA;` | `530` | X=5, Y=3, S=0 (`nA`) |
| `X=0;` `W=AoBn;` | `031` | X=0, Y=3 (mantido), S=1 (`AoBn`) |
| `Y=9;` `W=nAeB;` | `092` | X=0 (mantido), Y=9, S=2 (`nAeB`) |

## Erros propositais no arquivo de teste

O `testeula.ula` contém, de propósito, os dois tipos de erro previstos no enunciado, para testar a detecção:

| Tipo de erro | Onde aparece no arquivo |
|---|---|
| Mnemônico inexistente (sintaxe errada) | `W=AeC;`, `W=zero;` e `W=nAoBn;` |
| Linha em branco | Uma linha vazia entre `W=AoB;` e `X=6;` |

Essas linhas **não geram** instrução no `.hex`. Por isso o `.hex` tem menos linhas do que o número de `W=` do `.ula`.

## Como usar e estudar

1. Rode o tradutor em modo `1` (veja `Software/README.md`) e observe as mensagens de erro no console.
2. Abra o `.hex` gerado e confira se há uma linha para cada `W=` **válido**.
3. Como exercício, converta à mão três ou quatro linhas do `.ula` e compare com o `.hex`.
4. Para testar seu próprio cenário, edite o `.ula`, rode o tradutor de novo e cole o novo `.hex` no Monitor Serial do Arduino.

> O `.hex` é **sobrescrito** a cada execução do tradutor.
# docs/ — Enunciado do exercício

## Propósito

Esta pasta guarda a **fonte da verdade** do projeto: o enunciado oficial do Exercício Prático 03. Tudo o que foi implementado no repositório (formato das instruções, mapa de memória, pinos dos LEDs, formato do DUMP, tratamento de erros) vem deste documento.

## O que há aqui

| Arquivo | Descrição |
|---|---|
| `EP03_2026_2.pdf` | Enunciado do EP03 – ULA 4 bits + Arduino (8 páginas) |

## O que o enunciado contém

| Página(s) | Assunto |
|---|---|
| 1–2 | Arquitetura do sistema (Figura 1), pinos dos LEDs e tabela de instruções e mnemônicos (Figura 2) |
| 3–5 | Mapa da memória (PC, W, X, Y), exemplos 1 e 2 de execução passo a passo, requisitos de carga do programa |
| 6 | Funcionamento da execução, intervalo de 4 s e formato do DUMP |
| 7 | Software no PC: formato do `.ula` (Figura 3), do `.hex` (Figura 4) e ciclo de execução (Figura 5) |
| 8 | Itens a entregar e **situações em que se perde pontos** |

## Como estudar

1. Leia as páginas 1–2 e **monte a tabela de instruções** à mão. Ela é a base de tudo.
2. Refaça os **exemplos 1 e 2** (páginas 3–5) no papel, preenchendo o vetor de memória antes e depois de cada instrução.
3. Leia a página 8 por último e use a lista de "situações onde se pode perder pontos" como **checklist** para conferir o código em `Arduino/` e `Software/`.
4. Consulte este PDF sempre que surgir dúvida sobre o comportamento esperado, antes de questionar o código.
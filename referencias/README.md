# referencias/ — Material de apoio

## Propósito

Esta pasta reúne **respostas em PDF** do exercício, usadas como material de consulta e comparação. Elas **não fazem parte do código executável** do projeto. Servem para estudar outras formas de resolver o mesmo problema. Foram adicionadas ao repositório sem correção (commit "resp sem correção"), então devem ser lidas de forma crítica.

## O que há aqui

| Arquivo | Páginas | Conteúdo |
|---|---|---|
| `Arquitetura de computadores _tp.pdf` | 7 | Código do **Arduino**, com comentários explicando "o motivo de existir" de cada bloco. Documenta o mapa da memória (PC, W, X, Y, programa) e os pinos dos LEDs. Usa funções como `carregarPrograma`, `executarPrograma`, `calcularULA`, `atualizarLEDs` e `mostrarDumpMemoria` |
| `Arquitetura de computadores _tp_software.pdf` | 3 | Código do **software do PC** em C++, com a tabela de mnemônicos e os modos 0/1 de tratamento de erro. Aparenta seguir a mesma lógica de `Software/software.cpp` |

## Como usar

1. Abra primeiro o PDF do Arduino e compare a estrutura dele com `Arduino/ula_arduino.ino`: nomes de funções, separação entre carga e execução e formato do DUMP.
2. Aproveite os comentários do PDF como modelo de **código bem comentado**, que é um critério de avaliação do enunciado (página 8 de `docs/EP03_2026_2.pdf`).
3. Ao comparar, tenha o enunciado em mãos para verificar qual versão cumpre melhor cada requisito.
4. Este é material de leitura. Para executar algo, use as pastas `Arduino/` e `Software/`.
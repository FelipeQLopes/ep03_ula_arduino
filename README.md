# Projeto: Simulação de ULA 4 bits com Arduino (Exercício Prático 03)

##  Sobre o Projeto
Este projeto consiste na construção de um sistema computacional dividido em duas partes: um **Software no PC** e um **Hardware Externo (Arduino)**, que atua como uma Unidade Lógica e Aritmética (ULA) de 4 bits.

O fluxo de funcionamento do projeto é o seguinte:
1. Um usuário escreve um programa em um arquivo de texto (`.ula`) utilizando mnemônicos específicos.
2. O **Software no PC** lê esse arquivo, converte as instruções em código hexadecimal gerando um arquivo `.hex` e envia esses dados pela porta Serial (USB) para a placa.
3. O **Arduino** recebe esses dados e os armazena em um vetor que simula uma memória de 100 posições.
4. Após o carregamento completo, o Arduino inicia o ciclo de execução (**Busca -> Decodifica -> Executa**), processando instrução por instrução.
5. O resultado de cada operação lógica/aritmética é salvo na memória e exibido fisicamente em 4 LEDs. A cada passo, o sistema exibe no monitor serial um "DUMP" (o estado atual) da memória.

---

##  Divisão de Tarefas

O projeto foi dividido em quatro blocos principais de desenvolvimento para facilitar o trabalho em equipe. Cada participante pode assumir um bloco.

###  Bloco 1: Software do PC e Comunicação Serial
**Responsável:** [Nome da pessoa]
* [ ] Ler um arquivo de texto inicial (ex: `testeula.ula`) contendo os mnemônicos do programa fonte.
* [ ] Converter as instruções lidas em valores hexadecimais e gerar um segundo arquivo com a extensão `.hex`.
* [ ] Enviar os valores hexadecimais convertidos para o Arduino através da porta USB/serial.
* [ ] Tratar dois erros específicos no arquivo fonte: sintaxe errada da instrução e presença de linhas em branco.
* [ ] Garantir que, ao encontrar os erros, o programa os informe no console, mas **não interrompa** a cópia das instruções corretas para a memória.
* [ ] **Atenção:** Comentar exaustivamente o código para avaliação.

###  Bloco 2: Arduino - Gerenciamento de Memória e Carga de Dados
**Responsável:** [Nome da pessoa]
* [ ] Criar um vetor interno no Arduino com 100 posições para atuar como a memória da Unidade.
* [ ] Configurar as quatro primeiras posições do vetor como registradores: PC (posição 0), W (posição 1), X (posição 2) e Y (posição 3).
* [ ] Receber os dados vindos da porta serial e armazenar o programa no vetor a partir do índice 4.
* [ ] Criar uma "trava" estrutural garantindo que o programa só inicie a execução de fato após **todas** as instruções serem completamente copiadas para o vetor.

###  Bloco 3: Arduino - ULA e Ciclo de Execução
**Responsável:** [Nome da pessoa]
* [ ] Programar o ciclo de execução contínuo: buscar a instrução (apontada pelo PC), decodificá-la e executá-la.
* [ ] Implementar a lógica das 16 instruções da ULA (operações lógicas e aritméticas para dados de 4 bits correspondentes aos códigos Hexa de `0` a `F`).
* [ ] Atualizar a memória após cada instrução: escrever os novos valores de X e Y, colocar o resultado em W, e somar 1 ao valor do PC.
* [ ] Enviar o resultado armazenado em W (4 bits) para acender os 4 LEDs indicadores (Pino 13 = bit mais significativo, 12, 11 e Pino 10 = bit menos significativo).

###  Bloco 4: Arduino - DUMP, Temporização e Testes
**Responsável:** [Nome da pessoa]
* [ ] Implementar um *delay* de 4 segundos entre a execução de cada instrução para visualização correta dos LEDs.
* [ ] Desenvolver a função de "DUMP", que imprime na tela do monitor serial os valores contidos na memória do Arduino a cada instrução executada.
* [ ] O DUMP deve exibir o PC, W, X, Y e o restante do programa, mas **imprimindo apenas as posições que possuem conteúdo**, e não o vetor inteiro de 100 posições.
* [ ] Criar arquivos `.ula` de teste próprios da equipe, inserindo intencionalmente falhas de sintaxe e linhas em branco para homologar o tratamento de erros antes da apresentação.

---

##  Ordem de Execução e Integração

### É possível fazer tudo ao mesmo tempo?
**Sim!** O **Bloco 1** é completamente isolado, pois roda no computador. Quem assumir essa parte não precisa encostar no código do Arduino no início.
No Arduino (Blocos 2, 3 e 4), a equipe pode programar suas partes em paralelo criando funções independentes (ex: `iniciaMemoria()`, `executaULA()`, `imprimeDump()`). Use valores "fictícios" no código (mock) para testar a lógica antes de juntar tudo.

### Momentos de Integração:

1. **Primeira Integração (Interna no Arduino):**
   * O **Bloco 2 (Memória)** e o **Bloco 3 (ULA)** precisam ser unidos primeiro, porque a ULA precisa ler e escrever os dados dentro do vetor.
   * Imediatamente depois, anexa-se o **Bloco 4 (DUMP)**. Assim, a equipe consegue enxergar no monitor serial se a ULA está realmente alterando as posições certas da memória e se os LEDs acendem corretamente.
2. **Segunda Integração (O "Casamento" Final):**
   * Com o Arduino testado e validado, une-se o **Bloco 1 (Software do PC)** com o sistema da placa.
   * O fluxo completo acontecerá: O C++ enviará os dados convertidos pela porta serial, o Arduino (Bloco 2) lerá essa comunicação e preencherá a memória, permitindo que a ULA (Bloco 3) execute o programa oficial com o feedback do DUMP (Bloco 4).

## Compilar e executar o software do PC

Na pasta `Software`, use:

```sh
make
make run
```

O primeiro comando compila; o segundo executa. Os arquivos `.ula` e `.hex`
ficam em `Software/data/`.

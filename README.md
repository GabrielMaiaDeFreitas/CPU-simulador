# Simulador de CPU em C

Simulador de uma CPU simples desenvolvido em linguagem C para a disciplina de Arquitetura e Organização de Computadores (AOC).

O projeto simula o funcionamento básico de um processador baseado na arquitetura de Von Neumann, incluindo memória, registradores, conjunto de instruções e ciclo de execução.

## Sobre o projeto

O simulador permite carregar um programa em assembly a partir de um arquivo `programa.txt` e executar suas instruções de forma passo a passo.

A cada ciclo de execução, é possível acompanhar o estado da CPU e da memória, incluindo:

- Registradores R0 a R7;
- MBR;
- MAR;
- PC;
- IR;
- IMM;
- Campos RO0 e RO1;
- Flags E, L e G;
- Memória principal com 256 endereços.

A execução ocorre por meio das etapas de:

1. Busca da instrução;
2. Decodificação;
3. Execução.

O usuário pode avançar cada ciclo pressionando `ENTER` no terminal.

## Programa de exemplo

O projeto inclui um programa em assembly responsável por realizar a soma de duas matrizes 3 × 3, com valores de 16 bits.

A memória é organizada da seguinte forma:

- **Matriz A:** `0xCA` até `0xDB`
- **Matriz B:** `0xDC` até `0xED`
- **Resultado:** `0xEE` até `0xFF`

O programa percorre os elementos das duas matrizes, realiza as somas e armazena os resultados na região de memória destinada à matriz resultante.

A execução termina quando a instrução `hlt` é encontrada.

## Tecnologias

- C
- GCC
- Arquitetura de Von Neumann
- Assembly
- Arquitetura e Organização de Computadores

## Compilação

O projeto foi desenvolvido para compilação utilizando GCC no Ubuntu.

Na pasta raiz do projeto:

```bash
gcc CodigoFonte/main.c -o cpu_simulador

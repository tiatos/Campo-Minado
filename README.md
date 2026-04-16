# Campo-Minado
Jogo básico de campo minado feito com a linguagem C

Implementação do clássico jogo **Minesweeper (Campo Minado)** desenvolvida em linguagem C, utilizando conceitos fundamentais como matrizes, recursão, manipulação de entrada/saída e geração de números aleatórios.

## Sobre o Projeto

Este projeto recria o jogo Campo Minado no terminal, com sistema de:

- Escolha de dificuldade (Fácil e Normal)
- Geração aleatória de bombas
- Primeira jogada segura (sem bombas ao redor)
- Contagem automática de bombas adjacentes
- Expansão automática de áreas vazias (Expansion)
- Sistema de vitórias e derrotas

## Idioma

Todo o jogo **(interface, mensagens e explicações no código)** está em **português**.

## Conceitos Utilizados

- Matrizes bidimensionais
- Recursão (expansão automática)
- Validação de entrada
- Geração de números aleatórios (`rand`, `srand`)
- Lógica de jogo
- Controle de fluxo

## Possíveis Melhorias

Algumas ideias para evolução do projeto:

- Adicionar sistema de bandeiras 🚩
- Criar níveis de dificuldade personalizados
- Interface gráfica (SDL, ncurses, etc.)
- Salvar ranking de jogadores
- Melhorar validação de entrada

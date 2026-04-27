#include <stdio.h> // Standard in-out
#include <stdlib.h> // Necessário p/ as funções rand() e srand()
#include <time.h> // Necessário p/ função time()
#include <stdbool.h> // Necessário para ter funções booleanas

#define bomba '@' // Simbolo da bomba para simplificação de código
#define bandeira '!' // Simbolo da bandeira
#define vazio '_' // Simbolo para vazio
const char abc[] = "abcdefghi"; // Lados da matriz visivel
const char nums[] = "123456789";

void print_matrix(int rows, int columns, char matriz_visivel[][columns])
// Função para dar um print para o jogador ver o mapa
{
    printf("\n");
    for (int i = 0; i<rows ; i++) // linhas
    {
        for (int j = 0; j<columns ; j++) // Colunas
        {
            if (j == 0 && i != 0) {printf("{%c}", abc[i-1]);} // Primeira Coluna para mostrar letras

            else
            {
                if (i==0 && j ==0) {printf("[ ]");}

                else if (i == 0) {printf("{%d}", j);} // Primeira linha para mostrar números

                else {printf("[%c]",matriz_visivel[i][j]);}
            }

        }
        printf("\n");
    }
}

int procura_bombas(int rows, int columns, char matriz_bombas[][columns], int i, int j)
// Função para procurar bombas ao redor do quadrado escolhido
{
    int count_bomba = 0;
    for (int ib = i-1; ib <= i+1; ib++) // ib == i-bomba
    {
        for (int jb = j-1; jb <= j+1; jb++) // jb == j-bomba
        {
            if (ib < 0 || ib >= rows || jb < 0 || jb >= columns) {continue;} // Verificar se está dentro dos limites

            if (ib == i && jb == j) {continue;} // Casa atual

            if (matriz_bombas[ib][jb] == bomba) {count_bomba++;} // Contador de bomba aumenta caso tenha uma bomba
        }
    }
    return count_bomba;
}

void inicializar_bombas(int rows, int columns, char matriz_bombas[][columns], int ii, int ji, int bomb_count)
// Função para a inicialização da matriz das bombas
{
    for (int i = 0; i < rows; i++) // Esvazia a matriz
    {
        for (int j = 0; j < columns; j++)
        {
            matriz_bombas[i][j] = ' ';
        }
    }
    while (bomb_count > 0) // Espalha aleatóriamente todas as bombas no mapa (Menos na área que o jogador escolheu)
    {
        int i = (rand() % (rows - 1)) + 1;
        int j = (rand() % (columns - 1)) + 1;

        if (i >= ii-1 && i <= ii+1 && j >= ji-1 && j <= ji+1) continue; // Evita área inicial (3x3)

        if (matriz_bombas[i][j] != bomba)
        {
            matriz_bombas[i][j] = bomba;
            bomb_count--;
        }
    }
}

void inicializar_numeros(int rows, int columns, char matriz_bombas[][columns],char matriz_numeros[][columns])
// Função para inicializar matriz dos números
{
    for (int i = 0; i<rows;i++)
    {
        for (int j = 0; j <columns; j++)
        {
            if (matriz_bombas[i][j] != bomba) {matriz_numeros[i][j] = procura_bombas(rows, columns, matriz_bombas, i, j) + '0';} // Cada quadrado da matriz olha em volta de sí mesmos (se não forem bombas) para contar o número de bombas

            else matriz_numeros[i][j] = ' ';
        }
    }
}

void inicializar_jogador(int rows, int columns, char matriz_jogador[][columns])
// Função que preenche a matriz visível com "_"
{
    for (int i = 0; i<rows;i++)
    {
        for (int j = 0; j <columns; j++)
        {
            matriz_jogador[i][j] = vazio;
        }
    }
}

void pergunta_dificuldade(int *x,int *y, int *bomb_count)
// Função para perguntar a difículdade(tamanho da matriz): Fácil(5x5) e Normal(8x8)
{
    bool para_o_loop = true;
    while (para_o_loop)
    {
        int decisao_dificuldade;
        printf("Deseja Jogar em qual dificuldade?\n1- Fácil (5x5)\n2- Normal (8x8)\n");
        scanf("%d", &decisao_dificuldade);
        switch (decisao_dificuldade)
        {
            case 1:
                *x = 6; // Matriz 5x5
                *y = 6;
                *bomb_count = 6; // Número de bombas
                para_o_loop = false;
                break;
            case 2:
                *x = 9; // Matriz 8x8
                *y = 9;
                *bomb_count = 10; // Número de bombas
                para_o_loop = false;
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    }
}

bool contains(char lista[], int size, int target) {
// Função de procura em listas
    for (int i = 0; i < size; i++) {
        if (lista[i] == target) {
            return true; // Elemento encontrado, função para e retorna True
        }
    }
    return false; // Elemento não encontrado
}

bool bombas_faltando(int rows, int columns, char matriz_jogador[][columns], int bomb_count, char matriz_bombas[][columns])
// Função para retornar se todas as bombas foram encontradas
{
    int bomb_missing = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if ((matriz_jogador[i][j] == vazio) || (matriz_jogador[i][j] == bandeira && matriz_bombas[i][j] == bomba)) {bomb_missing++;}
            // Todos os '_' e os '!' em um lugar de bomba são contados
        }
    }
    return (bomb_count == bomb_missing); // Se número contado é igual ao número de bombas, jogo é ganho
}

void revelar(int rows, int columns, char matriz_jogador[][columns], char matriz_numeros[][columns], int i, int j)
// Função para revelar automaticamente áreas sem minas (expansão)
{
    if (i < 0 || i >= rows || j < 0 || j >= columns) return; // Procura se está fora do limite
    if (matriz_jogador[i][j] != vazio) return; // Procura se quadrado já foi revelado
    matriz_jogador[i][j] = matriz_numeros[i][j]; // Revela o quadrado
    if (matriz_numeros[i][j] != '0') return; // Se não for 0, função para
    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            if (di == 0 && dj == 0) continue;
            revelar(rows, columns, matriz_jogador, matriz_numeros, i + di, j + dj); // Loop de procura
        }
    }
}

void primeira_jogada(int rows, int columns, char matriz_jogador[][columns], char matriz_bombas[][columns], char matriz_numeros[][columns], char posicao[], int bomb_count) // Função para inicializar as bombas após primeira jogada do jogador
{
    while (true)
    {
        print_matrix(rows,columns, matriz_jogador); // Imprime Matriz
        printf("Insira uma posição para iniciar (Ex: a1)!\n");
        scanf(" %c%c", &posicao[0],&posicao[1]); // Escaneia posição da resposta
        if (contains(abc,rows-1,posicao[0]) && contains(nums,rows-1,posicao[1])) // Procura se ela é uma coordenada possível
        {
            int i = (posicao[0] - 'a') + 1; // Converte input para indice de matrix
            int j = (posicao[1] - '1') + 1;

            printf("Posição escolhida: linha %c, coluna %d\n", abc[i-1], j);
            inicializar_bombas(rows,columns, matriz_bombas, i, j, bomb_count); // Preenche aleatóriamente a matriz com bombas, menos na opção do jogador e ao redor
            inicializar_numeros(rows,columns, matriz_bombas,matriz_numeros); // Preenche a matriz com números baseado nas bombas
            revelar(rows, columns, matriz_jogador, matriz_numeros, i, j);
            break; // Sai do loop da primeira jogada
        }
        else
        {
            printf("\nOpcao invalida! Tente novamente.\n");
        }
    }
}

void loop_jogo(int rows, int columns, char matriz_jogador[][columns],char matriz_bombas[][columns],char matriz_numeros[][columns], int bomb_count, int *wins, int *loss) // Função para loop de jogabilidade
{
    char posicao[2];
    char decisao_bandeira;
    bool saida_jogo = true;
    primeira_jogada(rows, columns, matriz_jogador, matriz_bombas, matriz_numeros, posicao, bomb_count); // Bombas colocadas após primeira jogada
    while (saida_jogo) {
        print_matrix(rows,columns, matriz_jogador); // Imprime Matriz
        printf("Limpar um quadrado(q) ou colocar uma bandeira(b)? (q/b)\n");
        scanf(" %c", &decisao_bandeira);
        if (decisao_bandeira == 'q' || decisao_bandeira == 'b')
        {
            switch (decisao_bandeira)
            {
                case 'q':
                    printf("Insira uma posição para limpar o quadrado(Ex: a1):\n");
                    scanf(" %c%c", &posicao[0],&posicao[1]); // Escaneia posição da resposta
                    if (contains(abc,rows-1,posicao[0]) && contains(nums,rows-1,posicao[1])) // Procura se ela é uma coordenada possível
                    {
                        int i = (posicao[0] - 'a') + 1; // Converte input para indice de matrix
                        int j = (posicao[1] - '1') + 1;

                        printf("Posição escolhida: linha %c, coluna %d\n", abc[i-1], j);
                        if (matriz_bombas[i][j] == bomba) // Se tem bomba, perde o jogo
                        {
                            printf("VOCÊ PERDEU O JOGO\n");
                            *loss = *loss + 1;
                            saida_jogo = false;
                            break; // Sai do loop do jogo
                        }
                        else
                        {
                            revelar(rows, columns, matriz_jogador, matriz_numeros, i, j); // Se é número, substitui quadrado visível por um número
                            if (bombas_faltando(rows, columns, matriz_jogador, bomb_count, matriz_bombas))
                            {
                                printf("VOCÊ GANHOU, PARABÉNS! VAMOS JOGAR MAIS UMA VEZ!");
                                *wins = *wins + 1;
                                saida_jogo = false;
                                break;
                            }
                        }
                    }
                    else {printf("\nOpcao invalida! Tente novamente.\n");}
                break;

                case 'b':
                    printf("Insira uma posição para colocar a bandeira (Ex: a1):\n");
                    printf("Se já há uma bandeira na posição escolhida, ela será retirada \n");
                    scanf(" %c%c", &posicao[0],&posicao[1]); // Escaneia posição da resposta
                    if (contains(abc,rows-1,posicao[0]) && contains(nums,rows-1,posicao[1])) // Procura se ela é uma coordenada possível
                    {
                        int i = (posicao[0] - 'a') + 1; // Converte input para indice de matrix
                        int j = (posicao[1] - '1') + 1;

                        printf("Posição escolhida: linha %c, coluna %d\n", abc[i-1], j);
                        if (matriz_jogador[i][j] == bandeira) {matriz_jogador[i][j] = '_';} // Tinha bandeira
                        else if (matriz_jogador[i][j] == vazio) {matriz_jogador[i][j] = bandeira;} // Não tem bandeira
                        else {printf("Já foi revelado, não pode colocar bandeira");}
                    }
                    else {printf("\nOpcao invalida! Tente novamente.\n");}
                    break;

                default: printf("\nOpcao invalida! Tente novamente.\n");
            }
        }
    }
}

int main() // Main Do jogo completo
{
    char opcao;
    int wins = 0, loss = 0;
    bool cont = true;
    srand(time(NULL));

    while (cont) {
        //Loop Menu
        printf("\n");
        printf("Vamos jogar Campo minado? [s/n]\n");
        printf("\nTOTAL DE JOGOS: %d\n", wins+loss);
        scanf(" %c", &opcao);
        switch(opcao)
            {
            case 's':
            {
                printf("\nVamos continuar então!");
                printf("\nVitórias: %d X Derrotas: %d \n", wins, loss);
                int rows, columns, bomb_count;
                pergunta_dificuldade(&rows, &columns, &bomb_count);
                char matriz_bombas[rows][columns];
                char matriz_jogador[rows][columns];
                char matriz_numeros[rows][columns];
                inicializar_jogador(rows, columns, matriz_jogador);
                loop_jogo(rows, columns, matriz_jogador, matriz_bombas, matriz_numeros, bomb_count, &wins, &loss);
                break;
            }
            case 'n':
                printf("\nObrigado Por Jogar!\n");
                printf("\nPONTUAÇÃO FINAL:");
                printf("\nVitórias: %d X Derrotas: %d \n", wins, loss);
                cont = false;
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    }
    return 0;
}

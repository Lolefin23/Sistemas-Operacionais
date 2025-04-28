#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

// Faça um programa em C que implemente um interpretador de comandos com as seguintes funcionalidades:
// Executar programas com seus respectivos argumentos digitados na linha de comando. Quando a linha de
// comando terminar com o caracter '&' o interpretador de comandos será liberado para novos comandos sem
// aguardar o término da execução do programa solicitado. Quando não for digitado '&' o interpretador de
// comandos aguardará o término da execução do programa solicitado para depois liberar a linha de comando.
// A criação e execução de programas deve ocorrer através das funções fork() e execlp();
// Quando digitado o comando "tree PID", imprimir a subárvore de nomes de processos em execução no
// sistema tendo como raiz dessa subárvore o processo com identificação PID. A árvore deve ser construída
// usando as informações contidas no diretório /proc. Considere que cada subdiretório em /proc  cujo nome
// seja um número X representa um processo de PID X. Por exemplo, o subdiretório /proc/2345 possui informações
// a respeito do processo cujo PID é 2345. Considere ainda que o arquivo stat dentro de cada subdiretório
// possui as seguintes informações iniciais, respectivamente: PID, nome (entre parênteses), estado e PPID
// (PID do processo pai). Mais informações a respeito do diretório /proc podem ser obtidas através do comando
// "man proc". Para percorrer a estrutura de diretórios pode ser útil utilizar funções como opendir()/readdir()

#define args_max 6 // 5 argumentos + o próprio comando
#define linha_max 1024

// estrutura que armazena informações básicas de um processo, como seu PID, PPID e nome

typedef struct entrada
{
    pid_t Pid;
    pid_t Ppid;
    char nome[256];
} InfoProcess;

// função que executa os argumentos dados
void executa(char *args[], int fundo)
{
    // crio um processo filho e duplico ele
    pid_t pid = fork();

    if (pid == 0)
    {
        // verificamos se foi passado algum comando para executar
        if (args[0] == NULL)
        {
            puts("Nenhum comando especificado \n");
            exit(EXIT_FAILURE);
        }
        // contamos a quantida de argumentos
        int n = 0;

        while (n < args_max && args[n] != NULL)
        {
            n++;
        }
        // Garante terminação NULL
        if (n == args_max)
        {
            // Se atingiu o máximo sem ver NULL, força o sentinel
            args[args_max - 1] = NULL;
        }
        else
        {
            args[n] = NULL;
        }

        // Chama execvp, que busca no PATH e aceita vetor de argumentos
        execvp(args[0], args);
        // Se retornar, deu erro:
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else
    {
        if (!fundo)
        {
            waitpid(pid, NULL, 0);
        }
        else
        {
            printf("[1] %d\n", pid);
        }
    }
}

int main(int argc, char *argv[])
{

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

#define args_max 6 // máximo de argumentos: 5 + comando
#define linha_max 1024 // tamanho máximo da linha de comando

// estrutura para armazenar info de processos
typedef struct entrada
{
    pid_t Pid;
    pid_t Ppid;
    char nome[256];
} InfoProcess;

// função para executar comandos
void executa(char *args[], int fundo)
{
    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // processo filho

        if (args[0] == NULL) 
        {
            puts("Nenhum comando especificado \n");
            exit(EXIT_FAILURE);
        }

        int n = 0;
        while (n < args_max && args[n] != NULL)
            n++;

        if (n == args_max)
            args[args_max - 1] = NULL; // garante que args é NULL-terminated
        else
            args[n] = NULL;

        execvp(args[0], args); // executa comando
        perror("execvp"); // se execvp falhar
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // erro ao criar processo
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
    {
        // processo pai
        if (!fundo)
        {
            waitpid(pid, NULL, 0); // espera o filho terminar
        }
        else
        {
            printf("[1] %d\n", pid); // imprime PID se for em background
        }
    }
}

// lê processos do /proc e preenche o vetor de InfoProcess
int ler_processos(InfoProcess processos[], int max_proc)
{
    DIR *dir = opendir("/proc");
    struct dirent *entry;
    int count = 0;

    if (!dir)
    {
        perror("opendir /proc");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && count < max_proc)
    {
        if (isdigit(entry->d_name[0])) // verifica se é diretório de processo
        {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

            FILE *fp = fopen(path, "r");
            if (fp)
            {
                InfoProcess p;
                fscanf(fp, "%d (%[^)]) %*c %d", &p.Pid, p.nome, &p.Ppid); // lê PID, nome e PPID
                processos[count++] = p;
                fclose(fp);
            }
        }
    }

    closedir(dir);
    return count;
}

// função recursiva para imprimir árvore de processos
void imprime_arvore(InfoProcess processos[], int total, pid_t pid, int nivel, char *prefixo)
{
    int filhos[1024];
    int n_filhos = 0;

    // procura filhos do processo atual
    for (int i = 0; i < total; i++)
    {
        if (processos[i].Ppid == pid)
        {
            filhos[n_filhos++] = i;
        }
    }

    for (int i = 0; i < n_filhos; i++)
    {
        printf("%s", prefixo);

        if (i == n_filhos - 1)
        {
            printf("└── ");
        }
        else
        {
            printf("├── ");
        }

        printf("%s (%d)\n", processos[filhos[i]].nome, processos[filhos[i]].Pid);

        // monta novo prefixo para os filhos
        char novo_prefixo[1024];
        strcpy(novo_prefixo, prefixo);

        if (i == n_filhos - 1)
            strcat(novo_prefixo, "    ");
        else
            strcat(novo_prefixo, "│   ");

        imprime_arvore(processos, total, processos[filhos[i]].Pid, nivel + 1, novo_prefixo);
    }
}

int main(int argc, char *argv[])
{
    char linha[linha_max];

    while (1)
    {
        printf("prompt> ");
        fflush(stdout);

        if (fgets(linha, sizeof(linha), stdin) == NULL)
        {
            break; // EOF ou erro
        }

        linha[strcspn(linha, "\n")] = 0; // remove \n do final

        if (strlen(linha) == 0)
            continue; // ignora linhas vazias

        if (strcmp(linha, "exit") == 0)
        {
            printf("Saindo do terminal.\n");
            exit(EXIT_SUCCESS);
        }

        char *args[args_max];
        int fundo = 0;

        // divide linha em tokens
        int i = 0;
        char *token = strtok(linha, " ");
        while (token != NULL && i < args_max - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (i > 0 && strcmp(args[0], "tree") == 0)
        {
            // comando especial: tree <PID>
            if (i < 2)
            {
                printf("Uso: tree <PID>\n");
                continue;
            }

            pid_t raiz = atoi(args[1]);
            InfoProcess processos[1024];
            int total = ler_processos(processos, 1024);

            if (total >= 0)
            {
                for (int j = 0; j < total; j++)
                {
                    if (processos[j].Pid == raiz)
                    {
                        printf("%s (%d)\n", processos[j].nome, processos[j].Pid);
                        imprime_arvore(processos, total, raiz, 1, ""); // imprime a árvore

                        break;
                    }
                }
            }
        }
        else
        {
            // comando normal
            if (i > 0 && strcmp(args[i - 1], "&") == 0)
            {
                args[i - 1] = NULL; // remove &
                fundo = 1;
            }
            executa(args, fundo);
        }
    }

    return 0;
}

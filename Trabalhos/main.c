#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char *argv[])
{
    
    return 0;
}
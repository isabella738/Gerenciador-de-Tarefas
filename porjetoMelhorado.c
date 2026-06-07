#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define APAGAR "\033[0J"//apaga a tela da posição do cursor para baixo
#define APAGAR_LIN "\033[K"//apaga a linha atual
#define VOLTAR_LIN "\033[1F"//volta uma linha
#define VOLTAR "\033[19H"//volta para a escolha de função; 7+numero de funcoes
#define VOLTAR_1 "\033[22H"//volta para o inicio de cada funcao; 10+numero de funcoes
#define SALVAR "\033[s"//salva a posição atual do cursor
#define VOLTAR_2 "\033[u"//volta para a ultima posicao salva


/*
1. Max de 50 caracteres por item
2. Max de 200 itens por lista
3. Max de 500 caracteres por descrição
4. Max de 10 listas

- Dá pra criar outras listas
- Os itens tem prazo
- As 3 primeiras listas sao fixas e nao podem ser modificadas ou apagadas
- em reorganizar tarefas, exibir a opcao de reorganizacao automatica "por prazo"
*/

typedef struct{
    char item[50];
    char descricao[250];
    int prazo; //em dias
}tarefa;

typedef struct{
    char nome[15];
    int lim;
    tarefa t[200];
}lista;

lista l[10] = {0};
int n_de_listas;

//
//FUNCOES GERAIS
void remover_enter(char t[]);

int encerrar();

void minusculas(char t[]);

int comparar(char a[], char b[]);

void redistribuicao(lista *l, int x);

//
//FUNCOES DE BASE
void ler_int(int *n);

int ler_string(char t[], int tam);

int string_vazia(char t[]);

int nome_duplicado(char t[], int p);

int validar_lista(int n);

int validar_item(int item, lista *l);

int lista_vazia(int n);

//
//FUNCOES DE LEITURA DE ENTRADA
void lerNome_item(char teste[]);

void lerNome_descricao(char desc[]);

void lerNome_lista(char teste[]);

void lerInt_prazo(int *dias);

void lerInt_item(int x, int *item);

void lerInt_lista(int *n, int p);

//
//FUNCOES DE USO ESPECIFICO
void cadastro(lista *l, char item[], char desc[], int prazo);


//
int main(){
    system("clear");
    n_de_listas=3;
    strcpy(l[0].nome, "Pendentes");
    strcpy(l[1].nome, "Andamento");
    strcpy(l[2].nome, "Concluido");

    printf("=:: GERENCIADOR DE TAREFAS ::============\n\n");
    printf("Codigos das funcionalidades:\n");
    printf("1. Cadastrar tarefa\n");
    printf("2. Exibir Listas e Itens\n");
    printf("3. Adicionar nova lista\n");
    printf("4. Apagar Item\n");
    printf("5. Renomear Item\n");
    printf("6. Renomear Lista\n");
    printf("7. Apagar Lista\n");
    printf("8. Buscar Tarefas\n");
    printf("9. Mover tarefas entre listas\n");
    printf("10. Reorganizar Tarefas\n");
    printf("11. Redefinir prazo para uma tarefa\n");
    printf("12. Adicionar/Editar descricao\n");
    printf("0. Encerrar programa\n");
    printf("*Digite 0 para desistir de uma acao, se o programa requerir um inteiro.\n\n");

    while(1){
        printf("Escolha um numero: ");
        int escolha=0; ler_int(&escolha);

        if(escolha==0){printf("O programa foi encerrado.\n"); break;}
        else if(escolha==1){
            printf("\n:: Cadastro de tarefas ::\n");
            while(1){
                getchar();
                int erro=0, dias, n;
                char teste[50], desc[250];

                lerInt_lista(&n, 0); if(n==0)break;
                getchar();
                lerNome_item(teste);
                lerNome_descricao(desc);
                lerInt_prazo(&dias);
                cadastro(&l[n-1], teste, desc, dias);

                printf("\nTarefa cadastrada com sucesso!\n");
                getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==2){
            printf("\n:: Exibir tarefas ::\n");

            while(1){
                int n, item, erro=0;
                for(int i=0; i<n_de_listas; i++)
                    printf("%d. %s (%d)\n", i+1, l[i].nome, l[i].lim);
                printf("\n");

                printf("Selecione uma lista para ver mais detalhes, ou digite 0 para sair.\n");
                lerInt_lista(&n, 1);
                if(!n)break;
                n--;

                for(int i=0; i<l[n].lim; i++){
                    printf("%d. %s\n", i+1, l[n].t[i].item);
                    if(l[n].t[i].prazo==0)printf("Sem prazo.\n");
                    else printf("Prazo termina em: %d dias\n", l[n].t[i].prazo);
                }
                printf("\n");

                do{
                    printf(SALVAR "Selecione uma item para ver a descricao, ou digite 0 para sair: ");
                    do{
                        ler_int(&item);
                        if(!item)break;
                        erro = validar_item(item, &l[n]);
                    }while(erro);
                    if(!item){printf(VOLTAR_1 APAGAR); break;}
                    item--;

                    printf(APAGAR);
                    if(l[n].t[item].descricao[0]=='\0')printf("Este item nao possui descricao.");
                    else printf("- %s: %s", l[n].t[item].item, l[n].t[item].descricao);
                    printf(VOLTAR_2 APAGAR_LIN);
                }while(1);
            }
        }
        else if(escolha==3){
            printf("\n:: Adicionar nova lista ::\n");
            while(1){
                getchar();
                int erro=0;
                char teste[16];
                lerNome_lista(teste);

                strcpy(l[n_de_listas].nome, teste);
                n_de_listas++;
                printf("Lista adicionada com sucesso!\n");
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==4){
            printf("\n:: Apagar Item ::\n");
            while(1){
                int n, m, confirmar;
                lerInt_lista(&n, 1); 
                if(!n)break;
                lerInt_item(n-1, &m);
                printf(APAGAR_LIN"Tem certeza que deseja apagar este item?\n");
                printf("%s (Lista %s, item %d)\n(S/N)", l[n-1].t[m-1].item, l[n-1].nome, m);

                getchar();
                char c = getchar();
                if(c=='n' || c=='N'){printf("Acao desfeita."); break;}

                n--; m--;
                redistribuicao(&l[n], m);
                printf("O item foi apagado com sucesso.\n");

                getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==5){
            printf("\n:: Renomear Item ::\n");
            while(1){
                int n, m;
                char teste[50];
                lerInt_lista(&n, 1);
                lerInt_item(n-1, &m);
                n--; m--; getchar();
                lerNome_item(teste);
                strcpy(l[n].t[m].item, teste);

                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==6){
            printf("\n:: Renomear Lista ::\n");
            while(1){
                int n;
                char teste[50];
                lerInt_lista(&n, 2);
                if(n==0)break;
                n--; getchar();
                lerNome_lista(teste);
                strcpy(l[n].nome, teste);

                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==7){
            printf("\n:: Apagar Lista ::\n");
            while(1){
                int n;
                lerInt_lista(&n, 2);
                if(n==0)break;
                n--;
                printf(APAGAR_LIN"Tem certeza que deseja apagar esta lista?\n");
                printf("Lista %s, %d itens)\n(S/N)", l[n].nome, l[n].lim);

                getchar();
                char c = getchar();
                if(c=='n' || c=='N'){printf("Acao desfeita."); break;}

                for(int i=n; i<n_de_listas; i++)l[i]=l[i+1];
                n_de_listas--;
                printf("Lista apagada com sucesso.\n");

                getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==8){
            printf("\n:: Buscar Item ::\n");
            while(1){
                getchar();
                char teste[50];
                int contador=0;
                printf("Busca: "); ler_string(teste, 50);

                for(int i=0; i<n_de_listas; i++){
                    for(int j=0; j<l[i].lim; j++){
                        if(strstr(l[i].t[j].item, teste)!=NULL){
                            printf("- %s (Lista '%s', item %d)\n", l[i].t[j].item, l[i].nome, j+1);
                            contador++;
                        }
                    }
                }
                printf("Foram encontrados %d resultados para esta busca.\n", contador);
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==9){
            printf("\n:: Mover tarefas entre listas ::\n");
            while(1){
                int lista1, lista2, item;
                lerInt_lista(&lista1, 1);
                lerInt_item(lista1-1, &item);
                lerInt_lista(&lista2, 0);
                lista1--; lista2--; item--;

                l[lista2].t[l[lista2].lim] = l[lista1].t[item];
                l[lista2].lim++;
                for(int i=item; i<l[lista1].lim; i++)l[lista1].t[i]=l[lista1].t[i+1];
                l[lista1].lim--;

                printf("\nMudanca bem sucedida.\n"); getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==10){//
            printf("\n:: Reorganizar Tarefas ::\n");
            while(1){
                getchar();
                printf("Deseja reorganizar as listas automaticamente por ordem de menor prazo? (S/N) ");
                char c = getchar();
                if(c=='n' || c=='N'){
                    printf("Reorganizacao Manual\n");
                    int list, item1, item2;
                    lerInt_lista(&list, 1);
                    lerInt_item(list-1, &item1);
                    lerInt_item(list-1, &item2);
                    list--; item1--; item2--;
                    tarefa x;
                    x = l[list].t[item1];
                    l[list].t[item1] = l[list].t[item2];
                    l[list].t[item2] = x;
                }
                else{
                    for(int i=0; i<n_de_listas; i++){
                        for(int n=0; n<l[i].lim-1; n++){
                            int menor=100000000, a, achou=0;
                            tarefa x;
                            for(int j=n; j<l[i].lim; j++){
                                if(l[i].t[j].prazo<menor && l[i].t[j].prazo>0){
                                    menor=l[i].t[j].prazo; a=j; achou=1;
                                }
                            }
                            if(achou){
                                x = l[i].t[n];
                                l[i].t[n]=l[i].t[a];
                                l[i].t[a] = x;
                            }
                        }
                    }
                }
                printf("Mudancas bem sucedidas.\n"); getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==11){
            printf("\n:: Redefinir Prazos ::\n");
            while(1){
                int list, item, prazo;
                lerInt_lista(&list, 1);
                lerInt_item(list-1, &item);
                list--; item--;
                lerInt_prazo(&prazo);
                l[list].t[item].prazo=prazo;
                printf("Mudanca bem sucedida.\n"); getchar();
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else if(escolha==12){
            printf("\n:: Alterar Descricao ::\n");
            while(1){
                int list, item;
                char t[250];
                lerInt_lista(&list, 1);
                lerInt_item(list-1, &item);
                list--; item--;
                getchar();
                printf("Insira a nova descricao, ou deixe vazio para apagar:\n");
                if(ler_string(t, 250))l[list].t[item].descricao[0]='\0';
                else strcpy(l[list].t[item].descricao, t);

                printf("Modificacao bem sucedida.\n");
                if(encerrar())break;
                printf(VOLTAR_1 APAGAR);
            }
        }
        else printf("Numero invalido. Tente novamente.");
        printf(VOLTAR APAGAR);
    }
    return 0;
}


//
//FUNCOES GERAIS
void remover_enter(char t[]){
    int x = strlen(t);
    if(t[x-1]=='\n' && x>0)t[x-1]='\0';
}

int encerrar(){
    printf("Continuar? (S/N) ");
    char c = getchar();
    if(c=='n' || c=='N')return 1;
    return 0;
}

void minusculas(char t[]){
    for(int i=0; t[i]!='\0'; i++)
        if(t[i]>='A' && t[i]<='Z')t[i]+='a'-'A';
}

int comparar(char a[], char b[]){//1=iguais; compara duas strings sem levar em conta maiusculas e minusculas
    int igual=0;
    char x[250], y[250];
    strcpy(x, a); minusculas(x);
    strcpy(y, b); minusculas(y);

    if(strcmp(x, y)==0)return 1;
    return 0;
}

void redistribuicao(lista *l, int x){
    for(int i=x; i<l->lim; i++)l->t[i]=l->t[i+1];
    l->lim--;
}

//
//FUNCOES DE BASE
void ler_int(int *n){//verifica se é numero mesmo
    while(scanf("%d", n)==0){
        printf(VOLTAR_LIN APAGAR_LIN"Por favor, insira um numero valido: ");
        while(getchar()!='\n');
    }
}

int string_vazia(char t[]){//verifica se é vazia
    for(int i=0; t[i]!='\0'; i++)
        if(t[i]!=' ' && t[i]!='\n')return 0;
    return 1;
}

int ler_string(char t[], int tam){//verifica se é vazia ou se estoura o limite
    fgets(t, tam, stdin); 

    //Verifica se não estoura o limite de caracteres
    if(t[strlen(t)-1]!='\n'){
        while(getchar()!='\n');
        printf(APAGAR_LIN"O nome é grande demais. Tente novamente."VOLTAR_LIN APAGAR_LIN);
        return 1;
    }

    //Verifica se a string é vazia
    int erro = string_vazia(t);
    if(erro){printf(APAGAR_LIN"O nome nao pode estar vazio. Tente novamente."VOLTAR_LIN APAGAR_LIN); return 1;}

    remover_enter(t);
    return 0;
}

int nome_duplicado(char t[], int p){//1=lista, 0=tarefa; verifica nomes duplicados
    for(int i=0; i<n_de_listas; i++){
        if(p){
            if(comparar(t, l[i].nome)==1){
            printf(APAGAR_LIN"Ja existe uma lista com este nome. Tente novamente."VOLTAR_LIN APAGAR_LIN); 
            return 1;
            }
        }
        else{ 
            for(int j=0; j<l[i].lim; j++){
                if(comparar(t, l[i].t[j].item)==1){
                    printf(APAGAR_LIN"Ja existe uma tarefa com este nome. Tente novamente."VOLTAR_LIN APAGAR_LIN); 
                    return 1;
                }
            }
        }
    }
    return 0;
}

int validar_lista(int n){//verifica se o numero lista existe; deve receber quantidade, nao indice
    if(n<0 || n>n_de_listas){
        printf("Por favor, insira um numero valido.");
        printf(VOLTAR_LIN APAGAR_LIN);
        return 1;
    }
    return 0;
}

int validar_item(int item, lista *l){//verifica se o numero item existe (em quantidade, nao indice)
    if(item<0 || item>l->lim){
        printf("Este item nao existe. Insira um numero valido."VOLTAR_LIN APAGAR_LIN);
        return 1;
    }
    return 0;
}

int lista_vazia(int n){//verifica se a lista é vazia; deve receber quantidade, nao indice
    n--;
    if(l[n].lim==0){
        printf("A lista informada nao possui itens. Tente novamente."VOLTAR_LIN APAGAR_LIN);
        return 1;
    }
    return 0;
}

//
//FUNCOES DE LEITURA DE ENTRADA
void lerNome_item(char teste[]){
    int erro=1;
    do{
        printf("Insira o nome da nova tarefa: "); 
        erro = ler_string(teste, 50);
        if(!erro)erro=nome_duplicado(teste, 0);
    }while(erro);
    printf(APAGAR_LIN);
}

void lerNome_descricao(char desc[]){
    printf("Insira uma descricao para esta tarefa (deixe vazio para pular esta etapa): ");
    fgets(desc, 250, stdin);
}

void lerNome_lista(char teste[]){
    int erro=0;
    do{
        printf("Nome da nova lista: ");
        erro = ler_string(teste, 16);
        if(!erro) erro = nome_duplicado(teste, 1);
    }while(erro);
}

void lerInt_prazo(int *dias){
    while(1){
        printf("Defina um prazo (dias): ");
        ler_int(dias);
        if(*dias<0)printf("Entrada invalida. Tente novamente.\n");
        else break;
    }
}

void lerInt_item(int x, int *item){//x: numero da lista
    int erro=0;
    do{
        printf("Selecione um item: ");
        ler_int(item);
        if(!(*item))break;
        erro = validar_item(*item, &l[x]);
    }while(erro);
}

void lerInt_lista(int *n, int p){//p=1: verifica lista vazia; p=2: lista de 1-3
    int erro=0;
    do{
        printf("Escolha a lista: ");
        ler_int(n);
        if(!(*n))break;
        erro = validar_lista(*n);

        if(p==1 && !erro)erro = lista_vazia(*n);

        if(p==2 && (*n)>=1 && (*n)<=3 && !erro){
            printf("Voce nao pode modificar esta lista. Escolha outra."VOLTAR_LIN APAGAR_LIN);
            erro=1;
        }
    }while(erro);
    printf(APAGAR_LIN);
}

//
//FUNCOES DE USO ESPECIFICO
void cadastro(lista *l, char item[], char desc[], int prazo){
    strcpy(l->t[l->lim].item, item);

    if(!string_vazia(desc)){
        strcpy(l->t[l->lim].descricao, desc);
    }
    else l->t[l->lim].descricao[0]='\0';

    l->t[l->lim].prazo = prazo;

    l->lim++;
}

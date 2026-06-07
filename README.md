# Gerenciador-de-Tarefas

Limitações:
- Max de 10 listas
- Max de 200 itens por lista
- Max de 15 caracteres por nome de lista
- Max de 50 caracteres por nome de item
- Max de 250 caracteres por descricao de item

Resumo:
É um projeto pessoal, basicamente uma versão melhorada do projeto de LP1, focando mais no uso de structs, ponteiros e, sobretudo, na funcionalidade

Funcionalidades:
- Vem com 3 listas por padrão: Afazeres, em Andamento e Concluidas, que não podem ser modificadas
- O usuário também pode criar outras listas, renomeá-las e apagá-las. O mesmo não é possível com as 3 principais
- O usuário pode criar tarefas, movimentá-las, renomeá-las e apagá-las de qualquer lista
- Para cada item existe um nome título (que nao pode ser vazio), uma descrição e um prazo medido em dias. Cada um destes pode ser editado posteriormente
- É possível buscar por titulos de itens, que mostra todos os resultados em que a busca está contida
- É possivel reorganizar as listas automaticamente por ordem crescente de prazo, ou manualmente se necessario

Pontos a serem melhorados:
- Informacoes sao guardadas na memória
- Verificacões de estouro de array não foram bem testadas, e em alguns casos nem existe
- O programa não conta os dias de verdade
- Pode conter alguns erros de formatação

Sobre o código:
- Foram usados códigos de escape ANSI para manter o terminal legível e "limpo"
- Cada entrada é verificada antes de ser usada, através de funções, que contem entrada de dados, verificacao e mensagens de erro
- Entradas que esperam inteiro, caso recebam 0, sempre vão encerrar sua função (exceto ao se referir a prazos), uma vez que a posição 0 não existe
- Entradas de numeros inteiros são checadas para caso a entrada seja um caractere ou uma cadeia deste
- Entradas de inteiros que significam posição (tipo de lista e numero de item) são checadas para caso o numero da posição referida não exista ou não esteja sendo usado no momento
  - Em alguns casos, também é necessário também checar para caso a lista seja vazia
- Entradas de strings são checadas para caso a entrada seja vazia  (contenha somente ' ' ou quebra de linha)
  - No caso de nomes título, são checadas para caso tal nome já esteja sendo utilizado em outra posição
- Funcoes auxiliares incluem:
  - tornar todas as letras de uma string minusculas;
  - remover quebra de linha de uma string; redistribuir os itens de um vetor;
  - comparacao entre duas strings sem diferenciação entre maiúsculas e minúsculas;
  - funcao própria para perguntar ao usuario se deseja continuar ou parar o loop de certa funcionalidade
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct produto_estoque{
  //Estrutura para armazenar os registros do banco de dados de produtos
  char *codigo;
  char *nome;
  float preco;
  int estoque;
};

void bubbleSort(struct produto_estoque *x, int N){
  //Ordena a Stuct de Produtos com base na disponibilidade do estoque de forma crescente ou decrescente
  int maior,cont=0,max=N;
  for (int i=0;i<max;i++){
    maior = x[0].estoque;
    cont=0;
    for (int j=1;j<N-1;j++){
        if(x[j].estoque>maior){ 
        maior = x[j].estoque;
        cont+=1;
        }
        else{
          struct produto_estoque temp = x[cont];
          x[cont]=x[j];
          x[j]=temp;
          cont+=1;
        }     
    }
    N--;  
  }
}

int deletar_dados(char* filename){
  //Função para deletar dados do Produto (Funcionário)
  FILE *aquivo_banco, *arquivo_temp;
  char ch, *codigo=NULL, *delete_code=NULL, conf;
  int linha_deletar, cont = 1, sair=0;
  
  codigo = malloc(sizeof(char)*7);
  delete_code = malloc(sizeof(char)*7);

  //Abre o banco de dados no modo de leitura
  aquivo_banco = fopen(filename, "r");

  //Printa os registros atuais do banco de dados
  printar_banco_func(aquivo_banco,0);

  //Retorna ao começo do banco e procura a linha correspondendo ao código do produto desejado pelo usuário, caso o produto nao exista, o processo se repete
  do{
    rewind(aquivo_banco);
    ch='O';
    printf("\nInsira o código do produto para ser deletado:\n");
    scanf("%s", delete_code);

    do{
      fscanf(aquivo_banco,"%s",codigo);

      if(strcmp(codigo, delete_code)!=0){
        fscanf(aquivo_banco,"%*[^\n]");
        cont++;
      }
      ch = getc(aquivo_banco);
    }while((strcmp(codigo, delete_code)!=0) && ch!=EOF);

    if((strcmp(codigo, delete_code)!=0)){
      printf("\nCódigo de Produto não encontrado!");
      cont=1;
    }
  }while(strcmp(codigo, delete_code)!=0);

  //Reseta o contador
  linha_deletar = cont;
  cont=1;

  //Verificação da ação do usuário
  printf("\nVocê tem certeza que deseja deletar o Produto %s ? Digite s/n\n",delete_code);

  do{
    scanf(" %c", &conf);
    if(conf == 's'){
    printf("\nDeletando dados...\n");
    sair=1;
    }
    else if(conf == 'n'){
    printf("\nAção de Deleção cancelada!");
    sair=1;
    menu_acao_func();
    return(0);
    }
    else{
    printf("\nDigite um valor válido!\n"); 
    }
  }while(sair==0);

  //Cria um novo arquivo no modo de escrita para colocar apenas as linhas diferentes do produto que deve ser deletado

  rewind(aquivo_banco);
  arquivo_temp = fopen("temp.txt", "w");
  ch = 'O';
  while (ch!=EOF){
      ch = getc(aquivo_banco);

      if (cont!=linha_deletar &&ch!=EOF){
          putc(ch, arquivo_temp);
      }
      if (ch=='\n'){
          cont++;}
  }
  //Fecha os arquivos e Renomeia o arquivo temporário para o nome original do banco de dados
  fclose(aquivo_banco);
  fclose(arquivo_temp);
  remove(filename);
  rename("temp.txt", filename);

  //Printando as Alterações realizadas
  printf("\n Alterações Realizadas com sucesso! Banco de dados atualizado:\n");
  aquivo_banco = fopen(filename, "r");

  printar_banco_func(aquivo_banco,1);
  fclose(aquivo_banco);
  return 0;
}

int printar_banco_func(FILE *aquivo_banco,int back_menu){
  //Função de Consultar o estoque de produtos (Funcionário)

  //Abre o banco de dados de produtos e printa todos os registros encontrados para o funcionário
  printf("\n|-------------- Banco de Dados Estoque - Acesso Funcionário --------------|\n");
  char ch;
  while (ch!=EOF){
      printf("%c", ch);
      ch = getc(aquivo_banco);
  }

  //Parametro que indica se deve mostrar a tela de voltar ao menu de ação após printar
  if(back_menu==1){
    menu_acao_func();
  }
  else{
   return 0; 
  }
  
  return 0;
}

int login_func(char* filename){
  //Função de login do funcionário (Funcionário)
  FILE *Banco;

  //Alocando as string de login e senha inputadas e do banco de dados
  char ch, *login=NULL,*login_digitado=NULL, *senha=NULL,*senha_digitada=NULL;

  login = malloc(sizeof(char)*20);
  login_digitado = malloc(sizeof(char)*20);
  senha = malloc(sizeof(char)*20);
  senha_digitada = malloc(sizeof(char)*20);

  //Procura pelo login inputado no banco de dados
  Banco = fopen(filename ,"r");
  do{
      rewind(Banco);
      printf("\nInsira o seu Usuário: \n");
      scanf("%s", login_digitado);

      do{
          fscanf(Banco,"%s %s", login, senha);
          ch=fgetc(Banco);
      }while((strcmp(login, login_digitado)!=0) && ch!=EOF);

      if(strcmp(login, login_digitado)!=0){
          printf("\nLogin incorreto\n");
      }
  }while(strcmp(login, login_digitado)!=0);

  //Procura pela senha inputada no banco de dados, o funcionario tem 3 chances para acertar
  for(int i=3;i>0;i--){
      printf("\nInsira a sua Senha: \n");
      scanf("%s",senha_digitada);
      //printf("\n");
      if(strcmp(senha, senha_digitada)==0){
          fclose(Banco);
          return(1);
          break;      
      }
      else{
          printf("\nSenha incorreta\n");
          printf("Voce tem mais %i tentativas\n", i-1);
          if(i==1){
              fclose(Banco);
              return(0);
              exit(0);
          }
      } 
    }
    return (0);  
}

void add_produto (char*filename,char*cod_prod,char*nome_prod, float valor, int quantidade){
  FILE *aquivo_banco;
  aquivo_banco = fopen(filename, "a");
  fprintf(aquivo_banco,"%s %s %.2f %d\n", cod_prod, nome_prod, valor, quantidade);
  printf("\nProduto Registrado com Sucesso!\n");
  fclose(aquivo_banco);
  menu_acao_func();
  
}

int menu_acao_func(){
  //Menu de ações disponiveis para o funcionário, chama as demais funcoes
  int acao;
  char*cod_prod=malloc(7*sizeof(char));
  char*nome_prod=malloc(20*sizeof(char));
  float valor;
  int quantidade;

  do{
  printf("\nQual ação você deseja realizar?");
  printf("\n (0) Deletar Produto do Estoque\n (1) Consultar Estoque\n (2) Registar um novo Produto\n (3) Sair\n "); 
  scanf("%d",&acao);

  switch(acao){
    case 0:
      deletar_dados("banco_produtos.txt");
      return acao;
    case 1:
      consulta_dados_func();
      return acao;
    case 2:
      do{
      printf("\nPor Favor, insira o Código do produto a ser Adicionado:\n");
      scanf("%s",cod_prod);
      }while(strlen(cod_prod)!=6);

      do{
      printf("\nPor Favor, insira o Nome do produto a ser Adicionado:\n");
      scanf("%s",nome_prod);
      }while(strlen(nome_prod)<2);
      realloc(nome_prod,strlen(nome_prod)*sizeof(char));

      do{
      printf("\nPor Favor, insira o Valor (R$) do produto a ser Adicionado:\n");
      scanf("%f", &valor);
      }while(valor<=0);

      do{
      printf("\nPor Favor, insira o Estoque disponivel do produto a ser Adicionado:\n");
      scanf("%d", &quantidade);
      }while(quantidade<=0);

      add_produto("banco_produtos.txt",cod_prod,nome_prod,valor,quantidade);
    case 3:
      return acao;
    default:
      printf("\nRecurso em fase de Desenvolvimento!");
  }

  }while(acao!=0||acao!=1||acao!=2||acao!=3);
  return (acao);
}

struct produto_estoque *lista_produtos(int cliente,char *filename){
  //Função para ler os dados do banco e passar para Struct de Cliente ou funcionario

  FILE *aquivo_banco, *arquivo_temp;
  //Abre o banco de produtos no modo de leitura
  aquivo_banco = fopen(filename, "r");
  int tam=1;
  //Varre todas as linhas do banco e conta as quebrar de linhas para saber o total de registros no banco 
  char ch;
  while (ch!=EOF){
      ch = getc(aquivo_banco);
      if (ch == '\n'){
          tam++;}
  }

  //Usa a Struct do produto para armazenar os dados referentes ao produto, obtidos do banco de dados
  struct produto_estoque *produtos_cliente = malloc(sizeof(struct produto_estoque)*tam);

  rewind(aquivo_banco);
  ch='o';
  int produtos_validos=0;
  for(int i=0;i<tam;i++){

    ch = fgetc(aquivo_banco);
    char *cod = malloc(sizeof(char)*(ch+1));
    char *nome = malloc(sizeof(char)*(ch+1));
    float preco;
    int qtd;

    //Lê as informações do produto do banco de dados
    fscanf(aquivo_banco, "%s %s %f %d", cod, nome, &preco, &qtd);

    // e armazena num vetor da struct!
    if (cliente==0){
      produtos_cliente[i].codigo = cod;
      produtos_cliente[i].nome = nome;
      produtos_cliente[i].preco = preco;
      produtos_cliente[i].estoque = qtd;
    }
    else{
      if (qtd>0){
      produtos_cliente[produtos_validos].codigo = cod;
      produtos_cliente[produtos_validos].nome = nome;
      produtos_cliente[produtos_validos].preco = preco;
      produtos_cliente[produtos_validos].estoque = qtd;
      produtos_validos++;
      }
    }
  }
  return (produtos_cliente);
}

int consulta_dados_func(){
  FILE *aquivo_banco;
  int tipo_consulta,tam=1,match=0;
  char *produto_desejado = malloc(sizeof(char)*7);

  struct produto_estoque*produtos_funcio = lista_produtos(0,"banco_produtos.txt");

  printf("\nSelecione o tipo de consulta que deseja fazer: \n(1) Consultar estoque de um produto \n(2) Consultar os produtos com Maior estoque \n(3) Consultar os produtos com Menor Estoque\n ");

  //Abre o banco de produtos no modo de leitura
  char* filename = "banco_produtos.txt";
  aquivo_banco = fopen(filename, "r");

  //Varre todas as linhas do banco e conta as quebrar de linhas para saber o total de registros no banco 
  char ch;
  while (ch!=EOF){
      ch = getc(aquivo_banco);
      if (ch=='\n'){
          tam++;}
  }
  scanf("%d", &tipo_consulta);
  rewind(aquivo_banco);
  fclose(aquivo_banco);

  switch(tipo_consulta){
    case 1:
      for (int i=0;i<tam-1;i++){
      printf("\nCódigo: %s | Produto: %s",produtos_funcio[i].codigo,produtos_funcio[i].nome);
      }
      printf("\nInsira o Código do produto que deve ser consultado\n");
      scanf("%s",produto_desejado);

      for (int i=0;i<tam-1;i++){
        if(strcmp(produtos_funcio[i].codigo,produto_desejado)==0){
          printf("\nCódigo: %s \nProduto: %s \nPreço: R$ %.2f \nQuantidade: %d\n",produtos_funcio[i].codigo,produtos_funcio[i].nome,produtos_funcio[i].preco, produtos_funcio[i].estoque);
          free(produtos_funcio); 
          match = 1;
          break;
        } 
      }
      if (match==0){
        printf("\n Código do Produto Inválido, voltando ao Menu!");
        menu_acao_func();
      }
      else{
       menu_acao_func(); 
       return 0; 
      }
      break;
    case 2:
      bubbleSort(produtos_funcio,tam);
      for (int i=tam-2;i>0;i--){
      printf("\n Produto: %s | Estoque %d",produtos_funcio[i].nome,produtos_funcio[i].estoque);}
      printf("\n");
      free(produtos_funcio); 
      menu_acao_func();
      break;
    case 3:
      bubbleSort(produtos_funcio,tam);
      for (int i=0;i<tam-1;i++){
      printf("\n Produto: %s | Estoque %d",produtos_funcio[i].nome,produtos_funcio[i].estoque);}
      printf("\n");
      free(produtos_funcio);
      menu_acao_func(); 
      break;
    default:
      printf("\nOpção inválida, retornando às opções!");
      consulta_dados_func();
  }
  return 0;
} 

int comprar_cliente(char* filename){
  //Função de Compra do Cliente (Cliente)
  FILE *aquivo_banco, *arquivo_temp;

  //Abre o banco de produtos no modo de leitura
  aquivo_banco = fopen(filename, "r");
  int tam=1;

  //Varre todas as linhas do banco e conta as quebrar de linhas para saber o total de registros no banco 
  char ch;
  while (ch!=EOF){
      ch = getc(aquivo_banco);
      if (ch == '\n'){
          tam++;}
  }
  fclose(aquivo_banco);
  //Usa a Struct do produto para armazenar os dados referentes ao produto, obtidos do banco de dados
  struct produto_estoque *produtos_cliente = lista_produtos(1,"banco_produtos.txt");
  //Printa todos os produtos disponiveis para o cliente, armazenados na struct
  printf("\n|--------- |Magazine-UFABC| Produtos Disponíveis a Pronta-entrega! ---------|\n");

  for (int i=0;i<tam-1;i++){
  printf("\n (%d) Produto: %s | Preço R$ %.2f", i,produtos_cliente[i].nome,produtos_cliente[i].preco);
  }

  int produto_escolhido, quantidade;
  float total;
  //Recebe input do produto desejado pelo cliente
  do{
    printf("\n\n Escolha o Número do Produto de Desejado:\n ");
    scanf("%d", &produto_escolhido);
  }while(produto_escolhido>tam-1||produto_escolhido<0);

  //Recebe input da qtd desejada pelo cliente
  do{
    printf("\n Insira a Quantidade Total de Compra:\n ");
    scanf("%d", &quantidade);
    if(quantidade > produtos_cliente[produto_escolhido].estoque){
      //Verfica a disponibilidade da quantia desejada no estoque
      printf("\n Quantidade Indiponível no Estoque! Insira uma quantidade inferior");
    }
  }while(quantidade > produtos_cliente[produto_escolhido].estoque || quantidade<0);

  //Calcula o total da compra e Mostra um resumo do pedido ao cliente
  total = quantidade*produtos_cliente[produto_escolhido].preco;
  printf("\n Pedido de número #%d realizado com Sucesso!\n |---------------------------------------------|\n Resumo da Compra \n Items Ordenados: %d unidade(s), %s \n Total da Compra: R$ %.2f",rand() % 8000,quantidade,produtos_cliente[produto_escolhido].nome,total);
  free(produtos_cliente);
  return 0;
}

int main(){
  
  int escolha, login;
  //Escolhe se é cliente ou funcionário
  do{
    printf("\n| ******** |Magazine-UFABC| Seja bem-vindo ao Sistema de Controle de Estoque e Interface de Compra da Magazine-UFABC! ********|\n\n Por favor, escolha o seu tipo de Acesso:");

    printf("\n (0) Acesso Funcionário | Controle Estoque\n (1) Acesso Cliente | Interface Compra\n ");

    scanf("%d", &escolha);
  }while(escolha!=0 && escolha!=1);

  if (escolha==1){
  //Cliente
  comprar_cliente("banco_produtos.txt"); 
  }
  else{
  //Funcionario
    login = login_func("Banco.txt");
    if (login==1){
      menu_acao_func();
    }
    else{
      printf("\n Acesso não autorizado!");
    }
  }
  return 0;
}
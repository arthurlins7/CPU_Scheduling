Implementação - CPU_Scheduling

*Código*
<p>
  <ul>
    <li>O código tem como finalidade escalonar a execução de processos.<br />
    <li>Foi implementado na linguagem de programação C.<br />
    <li>Para começar o programa foi feita a leitura do arquivo que o usuário deseja que seja lido.<br />
    <li>E a abertura do arquivo rate_alg2.out para escrita:<br />
   </ul></p>
   
```c
   if ((fptr = fopen(argv[1], "r")) == NULL) {
    printf("Error! opening the input file");
    exit(1);
  if ((fptwf = fopen("rate_alg2.out", "w")) == NULL) {
    printf("Error! opening intermediary file");
    exit(1);  
  }
```
<p>
  <ul>
    <li>Após isso foram feitos diversos tratamentos de erros para caso de algum problema na leitura do arquivo, seja de acessar ele, ou de o usuário ter passado mais de um argumento, ou de dentro do arquivo está com informação a mais ou faltante, um exemplo é:<br />
   </ul></p>

```c
  if(argc > 2){
        fprintf(fptwf, "QUANTIDADE ERRADA DE ARGUMENTOS, TENTE APENAS COM UM ARQUIVO!");
        unlink(ARQUIVO_INTERMEDIARIO);
        exit(0);
	} 
```
<p>
  <ul>
    <li>Então um while loop foi feito para salvar primeiramente o tempo total de execução, e depois em arrays, os valores do processo, do período dele e do burst: <br />
   </ul></p>

```c
  while (fscanf(fptrf, "%s", word) > 0) {
    if (count == 0) {
      total_time = atoi(word);
    } else if (count % 3 == 1) {
      if(!atoi(word)){
        fprintf(fptwf, "ALGUM VALOR NÃO PODE SER CONVERTIDO EM INTEIRO");
        unlink(ARQUIVO_INTERMEDIARIO);
        exit(0);
      }
      else{
        processos[(count / 3)] = atoi(word);
      }
    } else if (count % 3 == 2) {
      if(!atoi(word)){
        fprintf(fptwf, "ALGUM VALOR NÃO PODE SER CONVERTIDO EM INTEIRO");
        unlink(ARQUIVO_INTERMEDIARIO);
        exit(0);
      }
      else{
        periodos[(count / 3)] = atoi(word);
      }
    } else if (count % 3 == 0) {
      if(!atoi(word)){
          fprintf(fptwf, "ALGUM VALOR NÃO PODE SER CONVERTIDO EM INTEIRO");
          unlink(ARQUIVO_INTERMEDIARIO);
          exit(0);
        }
      else{
        cpu_bursts[(count / 3) - 1] = atoi(word);
      }
    }
    count += 1;
  }
```
<p>
  <ul>
    <li>Após isso foi definida a prioridade para execução, já que no escalonamento de CPU quem tem prioridade é quem tem menor período: <br />
   </ul></p>

```c
for(int m = 0; m < count / 3; m++){
      if(init[m] == 1){
        if(periodos[m] < min){
          min = periodos[m];
          processo = processos[m];
          periodo = periodos[m];
          burst = cpu_bursts[m];
          index = m;
        }
      }
    } 
```
 <p>
  <ul>
    <li>Depois foi feito o algorítimo do escalonamento. <br />
   </ul></p>

 <p>
  <ul>
    <li>E após tudo isso foram impressos os resultados:<br />
   </ul></p>

```c
 fprintf(fptwf, "\nLOST DEADLINES\n");
  for(int n = 0; n < count / 3; n++){
      fprintf(fptwf, "[T%d] %d\n", processos[n], lost[n]);
    }
  fprintf(fptwf, "\nCOMPLETE EXECUTION\n");
  for(int n = 0; n < count / 3; n++){
      fprintf(fptwf, "[T%d] %d\n", processos[n], complete[n]);
    }
  fprintf(fptwf, "\nKILLED\n");
  for(int n = 0; n < count / 3; n++){
    if(init[n] == 1){
      fprintf(fptwf, "[T%d] 1\n", processos[n]);
    }
    else{
      fprintf(fptwf, "[T%d] 0\n", processos[n]);
    }
  }
```
     
*Makefile*

<p>
  <ul>
   <li>O Makefile é composto pelos comandos $make, $make run e $make clean.<br />
   <li>O comando $make compila o arquivo tipo C e gera um arquivo binário.<br />
   <li>O $make run usa o arquivo binário para executar o código.<br />
   <li>E o $make clean tem a função de apagar o arquivo binário que foi anteriormente gerado no $make.<br />
</ul></p>

```
all:
	gcc rate.c -o rate

run:
	./rate

clean:
	rm rate


```

*Resultado do escalonamento com os valores do input de exemplo*
```
EXECUTION BY RATE
[T1] for 25 units - F
[T2] for 25 units - H
[T1] for 25 units - F
[T2] for 5 units - L
[T2] for 20 units - H
[T1] for 25 units - F
[T2] for 15 units - F
idle for 10 units
[T1] for 15 units - K

LOST DEADLINES
[T1] 0
[T2] 1

COMPLETE EXECUTION
[T1] 3
[T2] 1

KILLED
[T1] 1
[T2] 1

```

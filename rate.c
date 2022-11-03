#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define _GNU_SOURCE
#define ARQUIVO_INTERMEDIARIO "correct.txt"

int main(int argc, char *argv[]) {
  int min = 1000000;
  char line[10000];
  int n = 0;
  int t = 0;
  FILE *fptr;
  FILE *fptw;
  FILE *fptrf;
  FILE *fptwf;

  if ((fptr = fopen(argv[1], "r")) == NULL) {
    printf("Error! opening the input file");
    exit(1);
  }

  /* CRIANDO UM ARQUIVO INTERMEDIÁRIO PARA ESCREVER AS INFORMAÇÕES DO ARQUIVO DE ENTRADA NELE, LINHA POR LINHA, POIS ACHEI MAIS SIMPLES DE TRABALHAR DESSA FORMA */
  if ((fptw = fopen(ARQUIVO_INTERMEDIARIO, "w")) == NULL) {
    printf("Error! opening intermediary file");
    exit(1);  
  }

  if ((fptwf = fopen("rate_alg2.out", "w")) == NULL) {
    printf("Error! opening intermediary file");
    exit(1);  
  }
  
    if(argc > 2){
        fprintf(fptwf, "QUANTIDADE ERRADA DE ARGUMENTOS, TENTE APENAS COM UM ARQUIVO!");
        unlink(ARQUIVO_INTERMEDIARIO);
        exit(0);
	} 
  
  while (fscanf(fptr, "%s", line) > 0) {
    char *teste = strtok(line, " ");
    n += 1;
    while (teste != NULL) {
      if(t%3 == 1){
        teste = strtok(teste, "T");
      }
      fprintf(fptw, "%s\n", teste);
      teste = strtok(NULL, " ");
      t+=1;
    }
  }
  fclose(fptr);
  fclose(fptw);

  if ((fptrf = fopen(ARQUIVO_INTERMEDIARIO, "r")) == NULL) {
    printf("Error! opening intermediary file");
    exit(1);
  }

  char word[n];
  int count = 0, processo = 0, periodo = 0, burst = 0, idle = 0, units = 0, index = 0, total_time = 0;
  int init[n], processos[n], periodos[n], cpu_bursts[n], complete[n], lost[n], holds[n], hold[n], units_comp[n];

  /* LENDO O ARQUIVO INTERMEDIÁRIO */
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
  if((count - 1)%3 != 0){
    fprintf(fptwf, "ALGUMA INFORMAÇÃO AUSÊNTE NO ARQUIVO DE LEITURA, POR FAVOR TENTAR COM OUTRO!");
    unlink(ARQUIVO_INTERMEDIARIO);
    exit(0);
  }
  
  fclose(fptrf);
  unlink(ARQUIVO_INTERMEDIARIO);

  /* FAZER O ALGORITMO */
  int total_time_cp = 0;
  
  fprintf(fptwf, "EXECUTION BY RATE\n");
  for (int j = 0; j < count / 3; j++) {
    init[j] = 1;
    holds[j] = 0;
    hold[j] = 0;
    complete[j] = 0;
    lost[j] = 0;
    units_comp[j] = 0;
  }
  
  do{
    /* DEFINIR PRIORIDADES */
    int sum = 0;
    int min = 1000000;
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
    for(int i = 0; i < count / 3; i++){
      sum += init[i];
    }

    if(sum != 0){
      if(hold[index] == 1){
        units_comp[index] = holds[index];
        hold[index] = 0;
      }

    /* CASO NENHUM PROCESSO ESTEJA SENDO EXECUTADO */
    if(idle > 0){
      fprintf(fptwf, "idle for %d units\n", idle);
      idle = 0;
      }
  
      total_time_cp += 1;
      units_comp[index] += 1;
  
      if(units_comp[index]%burst == 0){   
        /* CASO O PROCESSO SEJA FINALIZADO */
          fprintf(fptwf, "[T%d] for %d units - F\n", processo, units_comp[index]-holds[index]);
          complete[index] += 1;
          init[index] = 0;
          holds[index] = 0;
          units_comp[index] = 0;   
        } 
  
      else{
        /* CONTAR OS LOST DEADLINES DE PROCESSOS QUE ESTEJAM EXECUTANDO */
        if(total_time_cp%periodos[index] == 0 && units_comp[index] != cpu_bursts[index] && units_comp[index] > 0){
          fprintf(fptwf, "[T%d] for %d units - L\n", processo, units_comp[index] - holds[index]);
                units_comp[index] = 0;
                holds[index] = 0;
                lost[index] += 1;
                init[index] = 1;
            }
        for(int i = 0; i < count / 3; i++){
          
          /* CONTAR OS LOST DEADLINES MESMO QUE O PROCESSO NAO ESTEJA EXECUTANDO */
           if(init[i] == 1 && total_time_cp%periodos[i] == 0 && units_comp[i] != cpu_bursts[i] && i != index && hold[i] == 1){
                units_comp[i] = 0;
                holds[i] = 0;
                hold[i] = 0;
                lost[i] += 1;
                init[i] = 1;
            }
            
           else if(total_time_cp%periodos[i] == 0){
             /* CASO O PROCESSO FIQUE EM HOLD */
              if(init[i] == 0 && i != index && periodos[i] < min && units_comp[index] > 0){
                fprintf(fptwf, "[T%d] for %d units - H\n", processo, units_comp[index] - holds[index]);
                    holds[index] = units_comp[index];
                    hold[index] = 1;
                    units_comp[i] = 0;
                    init[i] = 1;
                }
             else if(total_time_cp%periodos[i] == 0){
               init[i] = 1;
             }
            }       
        }
        if(total_time_cp == total_time){
          fprintf(fptwf, "[T%d] for %d units - K\n", processo, units_comp[index]);
          units_comp[index] = 0;
        }
      }
    }
    else if(sum == 0){
      idle += 1;
      total_time_cp += 1;
       for(int i = 0; i < count / 3; i++){
          if(total_time_cp%periodos[i] == 0){
            init[i] = 1;
          }
       }
    }
  }while(total_time_cp <= total_time);
  
  /* IMPRIMIR RESULTADOS */
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
  
  return 0;
}
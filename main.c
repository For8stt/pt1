#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uvolni(char** polia, int row);

int v(FILE*f){
    //program zistí, či txt súbor už bol otvorený
    if((f= fopen("dataloger.txt","r"))!=NULL){
        printf("súbor už bol otvorený\n");
        fclose(f);
    }else{
        if((f= fopen("dataloger.txt","w"))!=NULL) {
            printf("Subor bol uspesne otvoreny");
            fclose(f);
        }else{
            printf("Neotvoreny subor\n");
        }
    }

    return 0;
}
int n(FILE*f){
    if (f == NULL) {//Ak súbor nie je otvorený(t.j. ešte nebol vykonaný príkaz ‘v’)
        fprintf(stderr,"Neotvoreny subor.\n");
        return 1;
    }

    char line[100];
    int row=0;
    while (fgets(line,sizeof(line),f)!=NULL){
        ++row;
    }
    rewind(f);

    char **polia=NULL;
    if (polia != NULL) { // Uvoľnite pamäť, ak pole už existuje
        //uvolni(polia, row);
        for (int i = 0; i < row; i++) {
            free(polia[i]);
        }
        free(polia);
    }

    polia=(char**)malloc(row*sizeof(line));
    for (int i = 0; i < row; i++) {//Zapís takom poradí, v akom sú v textovom súbore
        fgets(line, sizeof(line), f);
        polia[i] = strdup(line);
    }
    uvolni(polia,row);
    return 0;
}


void uvolni(char**polia,int row){
    for (int i = 0; i < row; ++i) {
        free(polia[i]);
    }
    free(polia);
}
int main() {
    FILE *f=NULL;
    char option;
    scanf("%c", &option);

    if(option=='v'){
        v(f);
    }
    if(option=='n'){
        n(f);
    }
    return 0;
}
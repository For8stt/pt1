#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uvolni(char** polia, int row);

int v(FILE**f, char**polia,int row){
    //
    //program zistí, či txt súbor už bol otvorený
    ///повино провіряти чи файл вже открит після n
    ///помилка мабуть є в цьому
    //
    if(*f==NULL){
        printf("súbor ešte nebol otvorený\n");
        if((*f= fopen("dataloger.txt","r+"))!=NULL) {
            printf("Subor bol uspesne otvoreny\n");
        }else{
            printf("Neotvoreny subor\n");
        }
    }
    if (*f!=NULL){/*V prípade úspešného otvorenia txt súboru*/
        if (polia!=NULL){//ak už boli vytvorené dynamické polia, zapíše sa z nich
            for (int i = 0; i < row; ++i) {
                printf("%s\n",polia[i]);
            }
        }else{//ak nie, bude čítať a zapisovať na obrazovku zo súboru
            char line[100];
            rewind(*f);
            char identifier[6];
            char position[15];
            char measurement[3];
            char hodnota[7];
            char time[5];
            char date[9];


            while (fgets(line, sizeof(line), *f) != NULL) {
                if (line[0] == '\n') {
                    printf("\n");
                    continue;  // Skips empty lines
                }

                sscanf(line, "%5s", identifier);//outputs the ID if the string is not empty

                fgets(line, sizeof(line), *f); // Position reading
                sscanf(line, "%14s", position);

                fgets(line, sizeof(line), *f); // Reading the value type
                sscanf(line, "%2s", measurement);

                fgets(line, sizeof(line), *f); // Reading the value
                sscanf(line, "%6s", hodnota);

                fgets(line, sizeof(line), *f); // Time reading
                sscanf(line, "%4s", time);

                fgets(line, sizeof(line), *f); // Date reading
                sscanf(line, "%8s", date);


                printf("ID. mer. modulu: %s\n", identifier);
                printf("Pozícia modulu: %s\n", position);
                printf("Typ mer. veliciny: %s\n", measurement);
                printf("Hodnota: %s\n", hodnota);
                printf("Cas merania: %s\n", time);
                printf("Datum merania: %s\n", date);


                printf("\n");

            }

            if (feof(*f)) {
                printf("Кінець файлу досягнутий.\n");
            } else if (ferror(*f)) {
                perror("Помилка читання файлу");
            }

        }
    }

    return 0;
}
int n(FILE*f,char ***polia,int* row){
    if (f == NULL) {//Ak súbor nie je otvorený(t.j. ešte nebol vykonaný príkaz ‘v’)
        fprintf(stderr,"Neotvoreny subor.\n");
        return 1;
    }

    char line[100];
    *row=0;
    while (fgets(line,sizeof(line),f)!=NULL){
        ++(*row);
    }
    rewind(f);

    if (*polia != NULL) { // Uvoľnite pamäť, ak pole už existuje
        //uvolni(polia, row);
        for (int i = 0; i < *row; i++) {
            free((*polia)[i]);
        }
        free(*polia);
    }

    *polia=(char**)malloc(*row * sizeof(char*)); ///sizeof(line)
    for (int i = 0; i < *row; i++) {//Zapís takom poradí, v akom sú v textovom súbore
        fgets(line, sizeof(line), f);
        (*polia)[i] = strdup(line);
    }
    rewind(f);
    return 0;
}
int c(){
    int y;
    scanf("%d",&y);//načíta celé číslo Y
    FILE *k;
    k= fopen("ciachovanie.txt","r");

    fclose(k);
}


void uvolni(char**polia,int row){
    for (int i = 0; i < row; ++i) {
        free(polia[i]);
    }
    free(polia);
}
int main() {
    FILE *f=NULL;
    char **polia=NULL;
    int row=0;
    char option;
    int m=1;
    while (m) {
        scanf("%c", &option);

        if (option == 'v') {
            v(&f, polia, row);
        }
        if (option == 'n') {
            n(f, &polia, &row);
        }
        if (option == 'c') {
            c();
        }
        if (option == 'k') {
            fclose(f);
            uvolni(polia, row);
            m=0;
        }
    }
    return 0;
}
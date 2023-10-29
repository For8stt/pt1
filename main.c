#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uvolni(char** polia, int row);

int v(FILE**f, char**polia,int n_l_rec,int records){
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
            int r=0;
            printf("for dinamics pols\n");

            for(int j=0; j<records;j++) {//displays dynamic fields in blocks (10 pcs)
                printf("ID. mer. modulu: %s", polia[r]);
                printf("Pozícia modulu: %s", polia[r+1]);
                printf("Typ mer. veliciny: %s", polia[r+2]);
                printf("Hodnota: %s", polia[r+3]);
                printf("Cas merania: %s", polia[r+4]);
                printf("Datum merania: %s", polia[r+5]);
                printf("\n");
                r+=n_l_rec;
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
            ///провірка для себе чи до кінця прочитався файл
            if (feof(*f)) {
                printf("Кінець файлу досягнутий.\n");
            } else if (ferror(*f)) {
                perror("Помилка читання файлу");
            }
            rewind(*f);
        }
    }

    return 0;
}
int n(FILE*f,char ***polia,int* row,int *records,int *n_l_rec){
    if (f == NULL) {//Ak súbor nie je otvorený(t.j. ešte nebol vykonaný príkaz ‘v’)
        fprintf(stderr,"Neotvoreny subor.\n");
        return 1;
    }

    char line[100];
    *row=0;
    *records = 0;
    while (fgets(line,sizeof(line),f)!=NULL){
        ++(*row);
        if(strlen(line) == 0 || line[0] == '\n'){
            ++(*records);
        }
    }
    // Додаткова перевірка для останнього рядка
    if (strlen(line) > 0 && (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')) {
        ++(*records);

    }
    ///підрахунок та виведення блоків звписів та скільки строк в блоці
    printf("%d\n",(*records));
    printf("%d\n",(*row)+1);
    *n_l_rec=((*row)+1)/(*records);//to read the last empty line
    printf("%d\n",*n_l_rec);
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

int c(char **polia,int records,int n_l_rec){
    int y;
    char line[100];
    int row_k=0;

    //scanf("%d",&y);//načíta celé číslo Y
    FILE *k;
    k= fopen("ciachovanie.txt","r+");
    while (fgets(line,sizeof(line),k)!=NULL){
        row_k+=1;
    }

    rewind(k);
    printf("%d\n",(row_k+1));
    char **polia_k=NULL;
    polia_k=(char**)malloc(row_k * sizeof(char*));
    for (int i = 0; i < row_k; i++) {//Zapís takom poradí, v akom sú v textovom súbore
        fgets(line, sizeof(line), k);
        (polia_k)[i] = strdup(line);
    }


    int r=0;
    int g=0;
    for(int i = 0; i < records; i++) {
        for (int j = 0; j < row_k; j++) {
            if (strcmp(polia[r], polia_k[g]) == 0) {
                printf("Поле %s ідентичне полю в %d %d в ciachovanie.txt\n", polia[r],g ,r);
                break;  // Якщо знайдено ідентичне поле, перейти до наступного поля
            }
            g+=row_k;
        }
        r += n_l_rec;
    }




    for (int i = 0; i < row_k; ++i) {
        free(polia_k[i]);
    }
    free(polia_k);
    fclose(k);
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
    char **polia=NULL;
    int row=0;///перевірити чи потрібні ця зміна в інших комиандах
    int records=0;
    int n_l_rec=0;
    char option;
    int m=1;
    while (m) {
        scanf("%c", &option);

        if (option == 'v') {
            v(&f, polia,n_l_rec,records);
        }
        if (option == 'n') {
            n(f, &polia, &row, &records,&n_l_rec);
        }
        if (option == 'c') {
            c(polia,records,n_l_rec);
        }
        if (option == 'k') {
            fclose(f);
            uvolni(polia, row);
            m=0;
        }
    }
    return 0;
}
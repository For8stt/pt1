#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void uvolni(char** polia, int row);

int v(FILE**f, char**polia,int n_l_rec,int records){
    //
    //program zistí, či txt súbor už bol otvorený
    ///повино провіряти чи файл вже открит після n
    ///помилка мабуть є в цьому
    /// перевірити чи нормально відкриває файл якщо його немає r+ w
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
                printf("ID. mer. modulu: %s\n", polia[r]);
                printf("Pozícia modulu: %s\n", polia[r+1]);
                printf("Typ mer. veliciny: %s\n", polia[r+2]);
                printf("Hodnota: %s\n", polia[r+3]);
                printf("Cas merania: %s\n", polia[r+4]);
                printf("Datum merania: %s\n", polia[r+5]);
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
    printf("records: %d\n",(*records));
    printf("row: %d\n",(*row)+1);
    *n_l_rec=((*row)+1)/(*records);//to read the last empty line
    printf("n_l_rec: %d\n\n",*n_l_rec);
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
        ///обрізання рядка
        char *str=(*polia)[i];
        int start = 0;
        size_t end = strlen(str) - 1;
        // Знаходження індексу першого непробільного символу з початку рядка
        while (isspace(str[start])) {
            start++;
        }
        // Знаходження індексу останнього непробільного символу з кінця рядка
        while (end > start && isspace(str[end])) {
            end--;
        }
        // Зміна рядка, щоб він включав лише символи без пробілів
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
        ///

    }

    rewind(f);
    return 0;
}

int c(char **polia,int records,int n_l_rec){
    int y;
    char line[100];
    int row_k=0;
    int records_k=0;

    if (polia==NULL){//checking for created fields
        fprintf(stderr,"Polia nie su vytvorene\n");
        return 1;
    }

    scanf("%d",&y);//načíta celé číslo Y
    ///створює динамічні рядки та підщитує його рядки та тп.
    y*=100;
    FILE *k;

    k= fopen("ciachovanie.txt","r+");
    while (fgets(line,sizeof(line),k)!=NULL){
        row_k+=1;
        if(strlen(line) == 0 || line[0] == '\n'){
            ++records_k;
        }
    }
    if (strlen(line) > 0 && (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')) {
        ++records_k;
    }
    printf("row_k: %d\n",(row_k+1));
    printf("records_k: %d\n",records_k);
    int n_l_rec_k=(row_k+1)/(records_k);//to read the last empty line
    printf("n_l_rec_k: %d\n\n",n_l_rec_k);

    rewind(k);
    char **polia_k=NULL;
    polia_k=(char**)malloc(row_k * sizeof(char*));
    for (int i = 0; i < row_k; i++) {//Zapís takom poradí, v akom sú v textovom súbore
        fgets(line, sizeof(line), k);
        (polia_k)[i] = strdup(line);
        ///обрізання кінців строк
        char *str_k=(polia_k)[i];
        int start = 0;
        size_t end = strlen(str_k) - 1;
        while (isspace(str_k[start])) {
            start++;
        }
        while (end > start && isspace(str_k[end])) {
            end--;
        }
        memmove(str_k, str_k + start, end - start + 1);
        str_k[end - start + 1] = '\0';
        ///
    }


    int r=0;
    int g=0;
    int all_lines=0;
    int all_lines_correct=0;//string validation
    int number_of_lines[records];
    memset(number_of_lines, 0, sizeof(number_of_lines));//встановив всі зміні масиву на нуль
    int of=0;

    for(int i = 0; i < records; i++) {
        for (int j = 0; j < row_k; j++) {
            if (strcmp(polia[r], polia_k[g]) == 0) {
                if (of < records) {//провірка , можна видалити
                    number_of_lines[of] = r;
                    ++of;
                }
                ++all_lines;
//                printf("Поле %s ідентичне полю %s в %d %d в ciachovanie.txt\n", polia[r], polia_k[g], r, g); провірка для себе, можна видилити при відправці
                char *endptr_k;
                char *endptr;
                ++g;
                r += 5;
                long date_k = strtol(polia_k[g], &endptr_k, 10);
                long date = strtol(polia[r], &endptr, 10);
                r -= 5;
                --g;

                if (date_k < date) {
                    long e = date - date_k;
                    if (e > y) {
                        e /= 100;
                        y /= 100;
                        e = e - y;
                        printf("ID. mer. modulu [%s] má %ld mesiacov po ciachovani\n", polia[r], e);
                        y *= 100;

                    }
//                    printf("%s менше, ніж %s\n", polia_k[g], polia[r]); також провірка, можна видилити до відправки

                } else {
                    ++all_lines_correct;//для провірки чи поля при ciachovani рівні
                }

                break;  // Якщо знайдено ідентичне поле, перейти до наступного поля
            }
            g+=n_l_rec_k;
            if(g>=row_k/*17*/){///код
                break;
            }

        }
        r += n_l_rec;
    }

/////////
    int is_present_7_to_35 = 1;
    int is_present_42_to_56 = 1;
    int is_present_63 = 1;

    for (int i = 0; i <= 35; i+=7) {
        for (int j = 0; j < 6; ++j) {
            if (number_of_lines[j] == i) {
                is_present_7_to_35 = 0;
                break;
            }
        }
    }

    for (int i = 42; i <= 56; i+=7) {
        for (int j = 6; j < 9; ++j) {
            if (number_of_lines[j] == i) {
                is_present_42_to_56 = 0;
                break;
            }
        }
    }

    if (number_of_lines[9] == 63) {
        is_present_63 = 0;
    }


    if (is_present_7_to_35) {
        printf("ID. mer. modulu [%s] nie je ciachovany.\n",polia[7]);
    }
    if (is_present_42_to_56) {
        printf("ID. mer. modulu [%s] nie je ciachovany.\n",polia[42]);
    }
    if (is_present_63) {
        printf("ID. mer. modulu [%s] nie je ciachovany.\n",polia[63]);
    }

//////////
    printf("\n");
    if(number_of_lines[records]!=0) {//випишше що всі Ід коректні якщо всі були ciachovany
        if (all_lines == all_lines_correct) {//checking that all lines are correct
            printf("Data su korektne.");
        }
    }


    for (int i = 0; i < row_k; ++i) {//все звільняє
        free(polia_k[i]);
    }
    free(polia_k);
    fclose(k);
    return 0;
}

int s (char **polia,int row,int n_l_rec,int records){
    if (polia==NULL){ //checking if the fields are created
        fprintf(stderr,"Polia nie su vytvorene.");
        return 1;
    }

    char mermodul[5];
    char typmer[3];
    int cislaid[records];
    int cislaidtyp[records];
    int cislageneral[records];
    int k=0;

    scanf("%s %s",mermodul,typmer);
    printf("Верхній регістр: %s\n", mermodul);
    printf("Нижній регістр: %s\n", typmer);

    for (int i = 0; i < row; i+= n_l_rec) {//finds identical ID. mer. modulu
        if (strcmp(polia[i], mermodul) == 0) {
            cislaid[k]=i;
            k++;
        }
    }
    k=0;
    for (int i = 2; i < row; i+= n_l_rec) {//finds identical Typ mer. veliciny
        if (strcmp(polia[i], typmer) == 0) {
            cislaidtyp[k]=i;
            k++;
        }
    }
    k=0;

    //які блоки знайшло
    int h=0;
    for (int i = 0; i < records; ++i) {//which ID and Typ mer intersect
        for (int j = 0; j < records; ++j) {
            if(cislaidtyp[j]-cislaid[i]==2){
                cislageneral[k]=cislaid[i];
                k++;
                h++;
                printf("%d ",h);
                break;
            }
        }
    }
    printf("%d",h);
    //checks if there are records for input

    if(h==0){
        fprintf(stderr,"Pre dany vstup neexistuju zaznamy");
        return 1;
    }

    FILE *filed;

    if ((filed=fopen("output_S.txt","w"))==NULL){
        fprintf(stderr,"Nepodarilo otvorit");
        return 1;
    }
    //



    char resultpolia[k][strlen(polia[4])+strlen(polia[5])+1];
    for (int i = 0; i < k; ++i) {//datum+cas
        resultpolia[i][0] = '\0';
        strcat(resultpolia[i], polia[cislageneral[i]+5]);
        strcat(resultpolia[i], polia[cislageneral[i]+4]);
    }
    for (int i = 0; i < k-1; i++) {//sorts by value: datum+cas
        for (int j = 0; j < k-i-1; j++) {
            if (strcmp(resultpolia[j], resultpolia[j + 1]) > 0){
                char temp[strlen(polia[4]) + strlen(polia[5]) + 1];
                strcpy(temp, resultpolia[j]);
                strcpy(resultpolia[j], resultpolia[j + 1]);
                strcpy(resultpolia[j + 1], temp);
                int presort=cislageneral[j];
                cislageneral[j]=cislageneral[j+1];
                cislageneral[j+1]=presort;
            }
        }
    }


    char pozmodl[k][15];
    char part1[k][8];
    char part2[k][8];
    char strfloat[k][10];
    float number=0;
    char combined[k][50];
    for (int i = 0; i < k; ++i) {//concatenates sorted strings
        strncpy(pozmodl[i], polia[cislageneral[i]+1], 14);
        pozmodl[i][14] = '\0';// Додаю завершуючий нуль для визначення кінця рядка
        sscanf(pozmodl[i],"%7s %7s",part1[i],part2[i]);
        number=atof(polia[cislageneral[i]+3]);
        sprintf(strfloat[i], "%.5f", number);
        sprintf(combined[i], "%s\t%s\t%s\t %s", resultpolia[i], strfloat[i], part1[i], part2[i]);

    }
    ///
    printf("\n");
    for (int i = 0; i < k; ++i) {
    printf("Об'єднаний рядок: %s\n", combined[i]);
    }
    ///

    if (filed != NULL) {
        for (int i = 0; i < k; ++i) {
            fprintf(filed,"%s\n", combined[i]);
        }
        if (fclose(filed) == 0) {
             printf("Pre dany vstup je vytvoreny txt subor.\n");
        } else {
        printf("súbor sa nepodarilo zatvoriť. Pre dany vstup nie je vytvoreny txt .\n");
         }
    } else {
        printf("chyba pri otváraní súboru\n");
    }

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
        if(option=='s'){
            s(polia,row,n_l_rec,records);
        }
        if (option == 'k') {
            fclose(f);
            uvolni(polia, row);
            m=0;
        }
    }
    return 0;
}
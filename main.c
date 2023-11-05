#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void uvolni(char** polia, int row);

int v(FILE**f, char**polia,int n_l_rec,int records){

    ///повино провіряти чи файл вже открит після n
    ///помилка мабуть є в цьому
    /// перевірити чи нормально відкриває файл якщо його немає r+ w
    if(*f==NULL){//the program detects whether the txt file has already been opened
        printf("súbor ešte nebol otvorený\n");
        if((*f= fopen("dataloger.txt","r+"))!=NULL) {
            printf("Subor bol uspesne otvoreny\n");
        }else{
            printf("Neotvoreny subor\n");
        }
    }
    if (*f!=NULL){/*In case of successful opening of the txt file*/
        if (polia!=NULL){//if dynamic fields have already been created, they will be written from
            int r=0;

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

        }else{//if not, it will read and write to the screen from the file
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
//            if (feof(*f)) {
//                printf("End of file reached.\n");
//            } else if (ferror(*f)) {
//                perror("Error reading file");
//            }
            rewind(*f);
        }
    }

    return 0;
}

int n(FILE*f,char ***polia,int* row,int *records,int *n_l_rec){
    if (f == NULL) {//If the file is not open (i.e. the 'v' command has not yet been executed)
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
    // Additional verification for the last line
    if (strlen(line) > 0 && (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')) {
        ++(*records);

    }
    //counting blocks of entries
    *n_l_rec=((*row)+1)/(*records);//to read the last empty line
    rewind(f);

    if (*polia != NULL) { // Free the memory if the array already exists
        //uvolni(polia, row);
        for (int i = 0; i < *row; i++) {
            free((*polia)[i]);
        }
        free(*polia);
    }

    *polia=(char**)malloc(*row * sizeof(char*)); ///sizeof(line)

    for (int i = 0; i < *row; i++) {//Write them in the order they are in the text file
        fgets(line, sizeof(line), f);
        (*polia)[i] = strdup(line);
        ///cutting the line
        char *str=(*polia)[i];
        int start = 0;
        size_t end = strlen(str) - 1;
        // Finding the index of the first non-whitespace character from the beginning of the line
        while (isspace(str[start])) {
            start++;
        }
        // Finding the index of the last non-whitespace character from the end of the line
        while (end > start && isspace(str[end])) {
            end--;
        }
        // Change the line so that it includes only symbols without spaces
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
    //creates dynamic lines and supports its lines, etc.
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

    int n_l_rec_k=(row_k+1)/(records_k);//to read the last empty line


    rewind(k);
    char **polia_k=NULL;
    polia_k=(char**)malloc(row_k * sizeof(char*));
    for (int i = 0; i < row_k; i++) {//Write them in the order they are in the text file
        fgets(line, sizeof(line), k);
        (polia_k)[i] = strdup(line);
        ///cropping the ends of terms
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
    int found1=0;
    int found2=0;
    int number_of_lines[records];
    memset(number_of_lines, 0, sizeof(number_of_lines));//set all array variables to zero
    int of=0;

    for(int i = 0; i < records; i++) {
        for (int j = 0; j < row_k; j++) {
            if (strcmp(polia[r], polia_k[g]) == 0) {
                char *endptr_k;
                char *endptr;
                ++g;
                r += 5;
                long date_k = strtol(polia_k[g], &endptr_k, 10);
                long date = strtol(polia[r], &endptr, 10);
                r -= 5;
                --g;

                long firstFourDigitsk=date_k/10000;
                long lastFourDigitsk = date_k % 10000;
                long firstFourDigits=date/10000;
                long lastFourDigits = date % 10000;
                ///if there are numbers 20230810 and 20231011, then it will be 1 month after ciachovani if you choose y=1, if there are 20230810 and 20231009, then there will be 0 months after ciachovani
                if (date_k < date) {
                    if (firstFourDigits>firstFourDigitsk){
                        long cislodo=firstFourDigits-firstFourDigitsk;
                        cislodo*=12;
                        long cisloko=(lastFourDigits-lastFourDigitsk)%100+cislodo;
                        if (cisloko > y) {
                            printf("ID. mer. modulu [%s] má %ld mesiacov po ciachovani\n", polia[r], cisloko);
                            found1=1;
                        }
                    } else {
                        long e = date - date_k;
                        if (e > y) {
                            e /= 100;
                            y /= 100;
                            e = e - y;
                            printf("ID. mer. modulu [%s] má %ld mesiacov po ciachovani\n", polia[r], e);
                            y *= 100;
                            found1 = 1;

                        }
                    }

                }
                break;  // If an identical field is found, move to the next field
            }
            g+=n_l_rec_k;
            if(g>=row_k){
                break;
            }

        }
        r += n_l_rec;
    }

    int mas_p[records_k];
    char mer_p[records][6];
    int p=0;
    int s=0;
    for (int i = 0; i < records; ++i) {
        mas_p[i]=records_k;
    }

    for (int i = 0; i < n_l_rec*records; i+=n_l_rec) {//walks on two arrays and looking for Id that are not ciachovani
        for (int j = 0; j < n_l_rec_k*records_k; j+=n_l_rec_k) {
            if (strcmp(polia[i],polia_k[j])!=0){
                mas_p[p]-=1;
            }
        }
        if (mas_p[p]==0){
            strcpy(mer_p[s],polia[i]);
            s++;
        }
        p++;
    }


    for (int i = 0; i < s; ++i) {//prescribes which are not ciachovani
        printf("ID. mer. modulu [%s] nie je ciachovany.\n",mer_p[i]);
        found2=1;
    }

//////////

    if( !found1 && !found2) {//write that all Ids are correct if all were ciachovany and right
        printf("Data su korektne.\n");
    }


    for (int i = 0; i < row_k; ++i) {//frees everything
        free(polia_k[i]);
    }
    free(polia_k);
    fclose(k);
    return 0;
}

int s (char **polia,int row,int n_l_rec,int records){
    if (polia==NULL){ //checking if the fields are created
        fprintf(stderr,"Polia nie su vytvorene.\n");
        return 1;
    }

    char mermodul[6];
    char typmer[3];
    int cislaid[records];
    memset(cislaid, 0, sizeof(cislaid));
    int cislaidtyp[records];
    memset(cislaidtyp, 0, sizeof(cislaidtyp));
    int cislageneral[records];
    memset(cislageneral, 0, sizeof(cislageneral));
    int k=0;

    scanf("%s %s",mermodul,typmer);
    printf("ID. mer. modulu: %s\n", mermodul);
    printf("Typ mer. veliciny: %s\n", typmer);

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

    //which blocks it found
    int h=0;
    for (int i = 0; i < records; ++i) {//which ID and Typ mer intersect
        for (int j = 0; j < records; ++j) {
            if(cislaidtyp[j]-cislaid[i]==2){
                cislageneral[k]=cislaid[i];
                k++;
                h++;
                break;
            }
        }
    }
    //checks if there are records for input

    if(h==0){
        fprintf(stderr,"Pre dany vstup neexistuju zaznamy\n");
        return 1;
    }

    FILE *filed;

    if ((filed=fopen("output_S.txt","w"))==NULL){
        fprintf(stderr,"Nepodarilo otvorit");
        return 1;
    }

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
    ///
    printf("\n");


    char pozmodl[k][15];
    char part1[k][8];
    char part2[k][8];
    char strfloat[k][10];
    float number=0;
    char combined[k][50];
    for (int i = 0; i < k; ++i) {//concatenates sorted strings
        strncpy(pozmodl[i], polia[cislageneral[i]+1], 14);
        pozmodl[i][14] = '\0';// I add a trailing zero to determine the end of the line
        sscanf(pozmodl[i],"%7s %7s",part1[i],part2[i]);
        number=atof(polia[cislageneral[i]+3]);
        sprintf(strfloat[i], "%.5f", number);
        sprintf(combined[i], "%s\t%s\t%s\t %s", resultpolia[i], strfloat[i], part1[i], part2[i]);

    }
    ///for myself
    for (int i = 0; i < k; ++i) {
    printf("Zreťazený reťazec: %s\n", combined[i]);
    }
    printf("\n");
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

int h(char **polia,int row,int n_l_rec,int records){
    if (polia==NULL){ //checking if the fields are created
        fprintf(stderr,"Polia nie su vytvorene.\n");
        return 1;
    }
    char *typeStrings[] = {"RM", "RD", "RO","PI","PE","PA"};
    int numTypes=sizeof(typeStrings) / sizeof(typeStrings[0]);//how many types of records
    int count[records];
    memset(count, 0, sizeof(count));
    int located[sizeof(typeStrings) / sizeof(typeStrings[0])][records];//how many types of records and how many blocks in total

    for (int i = 2; i < row; i+=n_l_rec) {//compares Typ mer. vel counts how many there are and their location
        if (strcmp(typeStrings[0],polia[i])==0){//compares Typ mer. veliciny
            located[0][count[0]]=i;//remembers the location
            count[0]++;//at the same time, it counts the number and changes it to the next slen in the static array
        }
        if (strcmp(typeStrings[1],polia[i])==0){
            located[1][count[1]]=i;
            count[1]++;
        }
        if (strcmp(typeStrings[2],polia[i])==0){
            located[2][count[2]]=i;
            count[2]++;
        }
        if (strcmp(typeStrings[3],polia[i])==0){
            located[3][count[3]]=i;
            count[3]++;
        }
        if (strcmp(typeStrings[4],polia[i])==0){
            located[4][count[4]]=i;
            count[4]++;
        }
        if (strcmp(typeStrings[5],polia[i])==0){
            located[5][count[5]]=i;
            count[5]++;
        }
    }
    // we create the number of min and max records and make rows of 7
    char min[sizeof(typeStrings) / sizeof(typeStrings[0])][7];
    char max[sizeof(typeStrings) / sizeof(typeStrings[0])][7];

    for (int j = 0; j < numTypes; ++j) {//we sort max and min
        if(count[j]!=0){//if there are Typ mer listings. vel
            strcpy(min[j], polia[located[j][0]+1]);
            strcpy(max[j], polia[located[j][0]+1]);

            for (int i = 1; i < count[j]; ++i) {//changes if more or less
                if (strcmp(polia[located[j][i]+1],min[j])<0) {
                    strcpy(min[j], polia[located[j][i]+1]);
                }

                if (strcmp(polia[located[j][i]+1],max[j])>0) {
                    strcpy(max[j], polia[located[j][i]+1]);
                }
            }

        }
    }
    ///виписує записи
    printf("%-15s %-10s %-10s %-10s\n", "Typ mer. vel", "Pocetnost", "Minimum", "Maximum");
    for (int i = 0; i < numTypes; ++i) {
        if (count[i]!=0){
            printf("%-15s %-10d %-10s %-10s\n",typeStrings[i],count[i],min[i],max[i]);
        }
    }
    return 0;

}

int z(char ***polia,int *row, int *records, int n_l_rec){
    char mermodul[6];
    int cislamer[*records];
    memset(cislamer, 0, sizeof(cislamer));

    scanf("%s",mermodul);
    printf("ID. mer. modulu: %s\n", mermodul);

    if (polia==NULL){ //checking if the fields are created
        fprintf(stderr,"Polia nie su vytvorene.\n");
        return 1;
    }

    int k=0;

    for (int i = 0; i < *row; i+= n_l_rec) {//finds identical ID. mer. modulu
        if (strcmp((*polia)[i], mermodul) == 0) {
            cislamer[k]=i;
            k++;

        }
    }
    if(k==0){//check whether ID is found
        fprintf(stderr,"Pre dany vstup neexistuju zaznamy");
        return 1;
    }

    int row_to_remove_start[k];
    int row_to_remove_end[k];

    for (int i = 0; i < k; ++i) {
        row_to_remove_start[i]=cislamer[i];
    }
    for (int i = 0; i < k; ++i) {
        row_to_remove_end[i]=cislamer[i]+6;
    }

    int removed_rows = 0;
    for (int i = 0; i < k; ++i) {
        removed_rows += (row_to_remove_end[i] - row_to_remove_start[i] + 1);
    }

    int new_row_size = *row - removed_rows;
    char **temp_polia = (char **)calloc(new_row_size , sizeof(char *));
    int new_row_index = 0;
    for (int i = 0; i < *row; ++i) {//deletes rows
        int in_removed_range = 0;
        for (int j = 0; j < k; ++j) {
            if (i >= row_to_remove_start[j] && i <= row_to_remove_end[j]) {//if true, it does not overwrite but deletes later in the cycle
                in_removed_range = 1;
                break;
            }
        }

        if (!in_removed_range) {
            temp_polia[new_row_index] = (*polia)[i];
            if (temp_polia[new_row_index] == NULL) {
                fprintf(stderr, "Chyba alokacie pamate.");
                return 1;
            }
            new_row_index++;
        } else {
            // Freeing memory for deleted rows
            free((*polia)[i]);
        }
    }

    for (int i = new_row_index; i < new_row_size; ++i) {
        temp_polia[i] = NULL;
    }


    *polia = temp_polia;
    // Update the number of rows in the main function and records
    *row = new_row_size;
    *records-=k;
    printf("Vymazalo sa: %d zaznamov!\n", k);

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
    int row=0;
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
        if(option=='h'){
            h(polia,row,n_l_rec,records);
        }
        if(option=='z'){
            z(&polia,&row, &records,n_l_rec);
        }
        if (option == 'k') {
            fclose(f);
            uvolni(polia, row);
            m=0;
        }
    }
    return 0;
}
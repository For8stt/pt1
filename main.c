#include <stdio.h>

int v(){
    FILE *f;
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

};
int n(){

    return 0;
}
int main() {
    char option;
    scanf("%c",&option);
    switch (option) {
        case 'v':
            v();
        case 'n':
            n();
            break;
        default:
            printf("Pomylka");
    }
    return 0;

}

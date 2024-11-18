#include <stdlib.h> //for memory allocation
#include <stdio.h> //for input and output
#include <string.h> //for string functions

//File operations

int createFile(){}

int copyFile(){
    /*char *fileInput = argv[1];
        
    char *fileName = malloc(strlen("./") + strlen(fileInput) + 1);
    strcpy(fileName, "./");
    strcat(fileName, fileInput);

    
    
    FILE *fp1 = fopen(fileName,"r"); //may need rb and wb
    FILE *fp2 = fopen("./file2.txt", "w");
    
    int c;    
    c = fgetc(fp1);
    while(c!=EOF){
        //fprintf(fp2, "%c", c);
        fputc(c, fp2);
        c=fgetc(fp1);
    }    
    
    fclose(fp1);
    fclose(fp2);
    free(fileName);*/
    return 0;
}

int deleteFile(){return 0;}

int showFile(){return 0;}

//Line operations

int appendLine(){return 0;}

int deleteLine(){return 0;}

int insertLine(){return 0;}

int showLine(){return 0;}

//General operations

int showChangeLog(){return 0;}

int showNumLines(){return 0;}

//Additional features
//Word count, Char count
//Rename file
//Write to a file
//Something cooler?


int main(){
    printf("---GROUP 127 TEXT EDITOR---\n");
    int input;
    int finished = 0;
    
    //Print options
    printf("\nSelect option: (integer input)\n");
    
    printf("0- Quit\n");

    printf("1- Create File\n");
    printf("2- Copy File\n");
    printf("3- Delete File\n");
    printf("4- Show File\n");

    printf("5- Append Line\n");
    printf("6- Delete Line\n");
    printf("7- Insert Line\n");
    printf("8- Show Line\n");

    printf("9- Show Change Log\n");
    printf("10- Show Number of Lines\n");

    printf("11- ?\n");
    printf("12- ?\n");
    printf("\n");
    
    while (!finished){
        
        printf("--> ");
        char term;
        //scanf returns the number of successfully read items
        //a valid input must be an integer followed by an enter (\n)
        if(scanf("%d%c", &input, &term) != 2 || term != '\n'){
            printf("Input must be an integer. Exiting...\n");
            break;
            }        

        switch(input){
            case 0:
                printf("Thank you!\n");
                finished = 1;
                break;     
            case 1:
                createFile();
                break;
            case 2:
                copyFile();
                break;
            case 3:
                deleteFile();
                break;
            case 4:
                break;
                showFile();
            case 5:
                appendLine();
                break;
            case 6:
                deleteLine();
                break;
            case 7:
                insertLine();
                break;
            case 8:
                showLine();
                break;
            case 9:
                showChangeLog();
                break;
            case 10:
                showNumLines();
                break;
            case 11:
                printf("Extra feature 1\n");
                break;
            case 12:
                printf("Extra feature 2\n");
                break;
            default:
                printf("Not a valid option, try again\n");
                break;
        }
    }
    
    
    return 0;
}
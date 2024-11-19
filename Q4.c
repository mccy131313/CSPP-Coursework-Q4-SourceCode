#include <stdlib.h> //for memory allocation
#include <stdio.h> //for input and output
#include <string.h> //for string functions

//(Fix so that it doesn't bug out if you enter more than 100 chars)
int getFileNameInput(char *fileNameInput){
    //Get file name input    
    printf("Input file name (without .txt): ");
    fgets(fileNameInput, 100, stdin); 
    
    //The fgets functions includes an enter (\n) so need to remove this
    int fileNameInputLen;
    for (int i=0; i<100;i++) {
        if(fileNameInput[i] == 0){
            fileNameInputLen = i-1;
            break;
        }        
    }       
    
    for (int j=0; j<fileNameInputLen-1;j++){
        fileNameInput[j] = fileNameInput[j];
    }
    fileNameInput[fileNameInputLen] = '\0';
    //The input is formatted as a string
    return 0;
}

//General useful functions
int fileExists(char *fileName){
    FILE *testFile = fopen(fileName, "r");
    if (testFile != NULL){        
        fclose(testFile);
        return 1;
    }
    else return 0;
}

//File operations

int createFile(){
    
    //Get file name input
    printf("Creating a file from inputted file name...\n");
    char fileNameInput[100];
    getFileNameInput(fileNameInput);

    //Format file name string
    char *fileName = malloc(strlen("./") + strlen(fileNameInput) + strlen(".txt") + 1);
    strcpy(fileName, "./");
    strcat(fileName, fileNameInput);
    strcat(fileName, ".txt");
        
    //Check if file exists
    if (fileExists(fileName)){
        printf("File already exists!\n");
        free(fileName);
        return -1;
    }
    
    FILE *f1 = fopen(fileName, "w");
    if (f1 == NULL){
        printf("Error with creating this file\n");
        free(fileName);
        return -1;
    }
    printf("File successfully created!\n");
    fclose(f1);
    free(fileName);
    return 0;
}

int copyFile(){
    
    //Get to and from file name input
    printf("File name to copy FROM\n");
    char fileNameInputFROM[100];
    getFileNameInput(fileNameInputFROM);
    
    printf("File name to copy TO\n");
    char fileNameInputTO[100];
    getFileNameInput(fileNameInputTO);

    
    //Format file name strings
    char *fileNameFROM = malloc(strlen("./") + strlen(fileNameInputFROM) + strlen(".txt") + 1);
    strcpy(fileNameFROM, "./");
    strcat(fileNameFROM, fileNameInputFROM);
    strcat(fileNameFROM, ".txt");

    char *fileNameTO = malloc(strlen("./") + strlen(fileNameInputTO) + strlen(".txt") + 1);
    strcpy(fileNameTO, "./");
    strcat(fileNameTO, fileNameInputTO);
    strcat(fileNameTO, ".txt");
    
    //Check if file exists
    if (fileExists(fileNameTO)){
        printf("File already exists!\n");
        printf("You must copy to a new file!\n");
        free(fileNameFROM);
        free(fileNameTO);
        return -1;
    }
            
    FILE *fFROM = fopen(fileNameFROM,"r"); //may need rb and wb    
    FILE *fTO = fopen(fileNameTO, "w");

    if (fFROM == NULL){
        printf("Error with opening this file\n");
        free(fileNameFROM);
        free(fileNameTO);
        return -1;
    }

    if (fTO == NULL){
        printf("Error with creating this file\n");
        free(fileNameFROM);
        free(fileNameTO);
        return -1;
    }
    
    //Go through and copy file character by character
    int c;
    c = fgetc(fFROM);
    while(c!=EOF){        
        fputc(c, fTO);
        c=fgetc(fFROM);
    }

    printf("File successfully copied!\n");
    fclose(fFROM);
    fclose(fTO);
    free(fileNameFROM);
    free(fileNameTO);    
    return 0;
}

int deleteFile(){
    //Get file name input
    printf("Deleting a file from inputted file name...\n");
    char fileNameInput[100];
    getFileNameInput(fileNameInput);

    //Format file name string
    char *fileName = malloc(strlen("./") + strlen(fileNameInput) + strlen(".txt") + 1);
    strcpy(fileName, "./");
    strcat(fileName, fileNameInput);
    strcat(fileName, ".txt");
        
    //Check if file exists
    if (!fileExists(fileName)){
        printf("File doesn't exist!\n");
        free(fileName);
        return -1;
    }
    
    if (remove(fileName) == 0){
        printf("File successfully deleted!\n");
        free(fileName);
        return 0;
    }
    else {
        printf("Error with deleting this file\n");
        free(fileName);
        return -1;
    }
}

int showFile(){
    //Get file name input
    printf("Displaying contents from inputted file name...\n");
    char fileNameInput[100];
    getFileNameInput(fileNameInput);

    //Format file name string
    char *fileName = malloc(strlen("./") + strlen(fileNameInput) + strlen(".txt") + 1);
    strcpy(fileName, "./");
    strcat(fileName, fileNameInput);
    strcat(fileName, ".txt");
        
    //Check if file exists
    if (!fileExists(fileName)){
        printf("File doesn't exist!\n");
        free(fileName);
        return -1;
    }
    
    //Open file
    FILE *f1 = fopen(fileName, "r");
    if (f1 == NULL){
        printf("Error with opening this file\n");
        free(fileName);
        return -1;
    }
    
    //Go through and print file character by character
    printf("\nHere is the file:\n");
    printf("--------------------------------\n");
    int c;
    c = fgetc(f1);
    while(c!=EOF){        
        printf("%c", c);
        c=fgetc(f1);
    }
    printf("\n");
    printf("--------------------------------\n");    
    
    printf("File successfully displayed!\n");
    fclose(f1);
    free(fileName);
    return 0;
}

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
//Show current text files in dir
//Something cooler?

int options(){
    //Print options
    printf("\nOptions: (integer input)\n");
    
    printf("101- Options\n");
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
    }

int main(){
    printf("---GROUP 127 TEXT EDITOR---\n");
    int input;
    int finished = 0;
    
    options();
    
    while (!finished){
        
        printf("\nChoose option--> ");
        char term;
        //scanf returns the number of successfully read items
        //a valid input must be an integer followed by an enter (\n)
        if(scanf("%d%c", &input, &term) != 2 || term != '\n'){
            printf("Input must be an integer. Exiting...\n");
            break;
            }

        switch(input){
            case 101:
                options();
                break;
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
                showFile();
                break;                
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
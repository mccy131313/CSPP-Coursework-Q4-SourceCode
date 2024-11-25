#include <stdlib.h> //for memory allocation
#include <stdio.h> //for input and output
#include <string.h> //for string functions

//(Fix so that it doesn't bug out if you enter more than 100 chars)
int getFileNameInput(char *fileNameInput){
    //Get file name input    
    printf("Input file name (without .txt): ");
    fgets(fileNameInput, 100, stdin); 
    
    
    //The fgets functions includes an enter (\n) so need to remove this
    size_t lastChar = strlen(fileNameInput) - 1;
    if (fileNameInput[lastChar] == '\n')
        fileNameInput[lastChar] = '\0';
    
    /*int fileNameInputLen;
    for (int i=0; i<100;i++) {
        if(fileNameInput[i] == 0){
            fileNameInputLen = i-1;
            break;
        }        
    }       
    
    for (int j=0; j<fileNameInputLen-1;j++){
        fileNameInput[j] = fileNameInput[j];
    }
    fileNameInput[fileNameInputLen] = '\0';*/
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

void randomFileName(char randFileName[], int length){
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //10+26+26 = 62 characters
    int i;
    for(i=0;i<length;i++){
        int index = rand() % 61;
        randFileName[i] = charset[index];        
    }

}

int getNumLines(char *fileName){
    //Open file
    FILE *f1 = fopen(fileName, "r");
    if (f1 == NULL){
        printf("Error with opening this file\n");
        return -1;
    } 

    int numLines = 1;
    int a;
    a = fgetc(f1);
    while(a!=EOF){
        if(a=='\n') numLines++;
        a=fgetc(f1);
    }
    
    return numLines;
}

//Used when I need to delete a specified line number in a file
int progDeleteLine(int lineNum, int numLines, FILE *filePointer, char *fileName){
    
    //Open temp file
    int uniqueFile = 0;
    int numForFile = 0;
    char *tempFileName;
    char randFileName[10];
    //Keep generating a random file name until a unique file name is found
    while(!uniqueFile){
        //Format file name string
        char randFileName[31];
        randFileName[30] = '\0';
        randomFileName(randFileName, 30);                
        tempFileName = malloc(strlen("./") + strlen(randFileName) + strlen(".txt") + 1);
        strcpy(tempFileName, "./");
        strcat(tempFileName, randFileName);
        strcat(tempFileName, ".txt");
            
        //Check if file exists
        if (fileExists(tempFileName)){            
            numForFile++;
            free(tempFileName);            
        }
        else uniqueFile=1;
    }
    //Create the file
    FILE *tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL){
        printf("Error with opening this file\n");
        return -1;
    }     
  
    //Go through and copy file character by character
    //copyOn flag indicates if you should copy or not based on the line number
    int copyOn = 1;
    if(lineNum==1) copyOn = 0;
    int currentLine = 1;
    int s;
    s = fgetc(filePointer);    
    while(s!=EOF){        
        //printf("Line- %d\nChar- {%c}\n", currentLine, s);
        //copyOn is 0 only for the selected linenumber
        if(currentLine == lineNum){
                copyOn = 0;
            }
            else {copyOn = 1;}        
        //change the line number when new line
        if(s=='\n'){
            if(currentLine==lineNum-1 && lineNum==numLines){ //if its the last line and line selected is the last line
                copyOn=0;
            }
            currentLine++;
        }
        //add the character is copyOn        
        //printf("Copy on is %d\n\n", copyOn);
        if(copyOn){            
            fputc(s, tempFile);
        }        
        s=fgetc(filePointer);
    }    
    
    //delete the old file
    if(remove(fileName) != 0){
        printf("Error with this method, sorry!!!\n");
        free(tempFileName);
        return -1;
    }
    
    //rename the new one to the old name
    rename(tempFileName, fileName);
    fclose(tempFile);
    free(tempFileName);
    return 0;
}

//Used when I need to delete a specified line number in a file
int progInsertLine(int lineNum, int numLines, FILE *filePointer, char *fileName, char *insertString){
    
    //Open temp file
    int uniqueFile = 0;
    int numForFile = 0;
    char *tempFileName;
    char randFileName[10];
    //Keep generating a random file name until a unique file name is found
    while(!uniqueFile){
        //Format file name string
        char randFileName[31];
        randFileName[30] = '\0';
        randomFileName(randFileName, 30);                
        tempFileName = malloc(strlen("./") + strlen(randFileName) + strlen(".txt") + 1);
        strcpy(tempFileName, "./");
        strcat(tempFileName, randFileName);
        strcat(tempFileName, ".txt");
            
        //Check if file exists
        if (fileExists(tempFileName)){            
            numForFile++;
            free(tempFileName);            
        }
        else uniqueFile=1;
    }
    //Create the file
    FILE *tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL){
        printf("Error with opening this file\n");
        return -1;
    }     
  

    //get length of the inputted string
    int inputLen = 0;
    int i = 0;
    while (insertString[i] != '\0') {
        inputLen++;
        i++;
    }

    //Go through and copy file character by character
    //Insert new line when you reach it
    int currentLine = 1;
    int s;
    s = fgetc(filePointer);        
    //if file is empty
    if (s==EOF){
        int k;
            for(k=0;k<inputLen;k++){
                fputc(insertString[k], tempFile);                
            }
            fputc('\n', tempFile);
    }
    else{
        while(s!=EOF){            
            if(currentLine==lineNum){            
                int k;
                for(k=0;k<inputLen;k++){
                    fputc(insertString[k], tempFile);                
                }
                fputc('\n', tempFile);
                currentLine++;
            }
            //change the line number when new line
            if(s=='\n'){
                currentLine++;
            }
            fputc(s, tempFile);
            s=fgetc(filePointer);
        }  
    }
    //delete the old file
    if(remove(fileName) != 0){
        printf("Error with this method, sorry!!!\n");
        free(tempFileName);
        return -1;
    }
    
    //rename the new one to the old name
    rename(tempFileName, fileName);
    fclose(tempFile);
    free(tempFileName);
    return 0;
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
    int s;
    s = fgetc(fFROM);
    while(s!=EOF){        
        fputc(s, fTO);
        s=fgetc(fFROM);
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
    int s;
    s = fgetc(f1);
    while(s!=EOF){        
        printf("%c", s);
        s=fgetc(f1);
    }
    printf("\n");
    printf("--------------------------------\n");    
    
    printf("File successfully displayed!\n");
    fclose(f1);
    free(fileName);
    return 0;
}

//Line operations

int appendLine(){
    //Get file name input
    printf("Appending inputted line of contents to inputted file name...\n");
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
    FILE *f1 = fopen(fileName, "a");
    if (f1 == NULL){
        printf("Error with opening this file\n");
        free(fileName);
        return -1;
    }

    //add inputted line
    char inputContents[300];
    printf("Input line you would like to add (max 300 chars):\n");
    printf("--> ");    
    fgets(inputContents, 300, stdin);
    
    //The fgets functions includes an enter (\n) so need to remove this
    size_t lastChar = strlen(inputContents) - 1;
    if (inputContents[lastChar] == '\n')
        inputContents[lastChar] = '\0';

    fprintf(f1, "\n%s", inputContents);    
    printf("Line successfully appended to file!\n");    
    fclose(f1);
    free(fileName);
    return 0;
}

int deleteLine(){ 
    //Get file name input
    printf("Deleting inputted line of contents from inputted file name...\n");
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
    //Get input for line number and check it is valid
    int validLine = 0;
    int lineNum;
    char input[50];
    int numLines = getNumLines(fileName);
    if(numLines==-1) {
        printf("Error opening this file\n");
        free(fileName);
        fclose(f1);
        return -1;
    }

    while(!validLine){
        //GET INPUT
        printf("Enter line number to delete: ");
        fgets(input, 50, stdin); 
        
        //The fgets functions includes an enter (\n) so need to remove this
        size_t lastChar = strlen(input) - 1;
        if (input[lastChar] == '\n')
            input[lastChar] = '\0';
                
        //convert input to integer
        if(*input=='0'){
            printf("Not a valid line number.\n\n");            
        }
        else{
            lineNum=atoi(input);
            if (lineNum==0){
                    printf("Invalid input entered. Must be an integer.\n\n");
            }        
            else{ //input is an integer but is it valid?
                if(!(lineNum > 0 && lineNum <= numLines)){
                    printf("Not a valid line number.\n\n");
                }
                else validLine=1;

            }
        }
    }

    progDeleteLine(lineNum, numLines, f1, fileName);
    printf("Line successfully deleted!\n");
    fclose(f1);
    free(fileName);
    return 0;    
}

//FIX, doesn't work in empty file?
int insertLine(){
    //Get file name input
    printf("Inserting line of contents into inputted file name...\n");
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
    FILE *f1 = fopen(fileName, "rw");
    if (f1 == NULL){
        printf("Error with opening this file\n");
        free(fileName);
        return -1;
    }

    //Get input for line number and check it is valid
    int validLine = 0;
    int lineNum;
    char input[50];
    int numLines = getNumLines(fileName);
    if(numLines==-1) {
        printf("Error opening this file\n");
        free(fileName);
        fclose(f1);
        return -1;
    }
    
    while(!validLine){
        //GET INPUT
        printf("Enter line number to insert at: ");
        fgets(input, 50, stdin); 
        
        //The fgets functions includes an enter (\n) so need to remove this
        size_t lastChar = strlen(input) - 1;
        if (input[lastChar] == '\n')
            input[lastChar] = '\0';
                
        //convert input to integer
        if(*input=='0'){
            printf("Not a valid line number.\n\n");            
        }
        else{
            lineNum=atoi(input);
            if (lineNum==0){
                    printf("Invalid input entered. Must be an integer.\n\n");
            }        
            else{ //input is an integer but is it valid?
                if(!(lineNum > 0 && lineNum <= numLines)){
                    printf("Not a valid line number.\n\n");
                }
                else validLine=1;

            }
        }
    }

    //add inputted line
    char insertString[300];
    printf("Input line you would like to insert (max 300 chars):\n");
    printf("--> ");    
    fgets(insertString, 300, stdin);
    
    //The fgets functions includes an enter (\n) so need to remove this
    size_t lastChar = strlen(insertString) - 1;
    if (insertString[lastChar] == '\n')
        insertString[lastChar] = '\0';
            
    progInsertLine(lineNum, numLines, f1, fileName, insertString);
    printf("Line successfully inserted!\n");
    fclose(f1);
    free(fileName);
    return 0;
}

int showLine(){
    //Get file name input
    printf("Outputting line of contents from inputted file name...\n");
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

    //Get input for line number and check it is valid
    int validLine = 0;
    int lineNum;
    char input[50];
    int numLines = getNumLines(fileName);
    if(numLines==-1) {
        printf("Error opening this file\n");
        free(fileName);
        fclose(f1);
        return -1;
    }
    
    while(!validLine){
        //GET INPUT
        printf("Enter line number to show: ");
        fgets(input, 50, stdin); 
        
        //The fgets functions includes an enter (\n) so need to remove this
        size_t lastChar = strlen(input) - 1;
        if (input[lastChar] == '\n')
            input[lastChar] = '\0';
                
        //convert input to integer
        if(*input=='0'){
            printf("Not a valid line number.\n\n");            
        }
        else{
            lineNum=atoi(input);
            if (lineNum==0){
                    printf("Invalid input entered. Must be an integer.\n\n");
            }        
            else{ //input is an integer but is it valid?
                if(!(lineNum > 0 && lineNum <= numLines)){
                    printf("Not a valid line number.\n\n");
                }
                else validLine=1;

            }
        }
    }

    //Go through and copy file character by character
    //Insert new line when you reach it
    int currentLine = 1;
    int s;
    s = fgetc(f1);
    printf("Line %d: ", lineNum);
    while(s!=EOF){        
        if (currentLine==lineNum){
            if(s!='\n') printf("%c", s);
        }
        //change the line number when new line
        if(s=='\n'){
            currentLine++;
        }        
        s=fgetc(f1);
    }
    printf("\n");
   
    fclose(f1);
    free(fileName);
    return 0;
}

//General operations

int showChangeLog(){
    //Open file
    FILE *f1 = fopen("./changelog.log", "r");
    if (f1 == NULL){
        printf("Error with opening change log\n");        
        return -1;
    }
    
    //Go through and print file character by character
    printf("\nHere is the change log:\n");
    printf("--------------------------------\n");
    int s;
    s = fgetc(f1);
    while(s!=EOF){        
        printf("%c", s);
        s=fgetc(f1);
    }
    printf("\n");
    printf("--------------------------------\n");    

    fclose(f1);
    return 0;
}

int showNumLines(){
    //Get file name input
    printf("Displaying number of lines in specified file...\n");
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
        
    //Go through and copy file character by character
    //Insert new line when you reach it
    int currentLine = 1;
    int s;
    s = fgetc(f1);    
    while(s!=EOF){
        if(s=='\n'){
            currentLine++;
        }        
        s=fgetc(f1);
    }
    
    printf("Number of lines: %d\n", currentLine);

    fclose(f1);
    free(fileName);
    return 0;

}

//Additional features
//Word count, Char count
//Rename file
//Show current text files in dir!!
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
#include <stdlib.h> //for memory allocation
#include <stdio.h> //for input and output
#include <string.h> //for string functions
#include <time.h> //for getting current datetime
#include <dirent.h> //for reading files in directories

//gets the user input for a file name
int getFileNameInput(char *fileNameInput){
    int validInput = 0;
    while(!validInput){
        printf("Input file name (without .txt): ");
        fgets(fileNameInput, 100, stdin);
        
        //The fgets functions includes an enter (\n) so need to remove this
        size_t lastChar = strlen(fileNameInput) - 1;
        if (fileNameInput[lastChar] == '\n')
            fileNameInput[lastChar] = '\0';
        
        //the fileNameInput has a size of 100 so check input is within this
        if(strlen(fileNameInput)>=99){
            printf("Input is max 98 chars!\n");            
            while (getchar() != '\n'); //this "clears" the stdin so input doesnt overflow
        }        
        else validInput=1;
    }            
    return 0;
}

//General useful functions

//checks if an inputted file exists
int fileExists(char *fileName){
    FILE *testFile = fopen(fileName, "r");
    if (testFile != NULL){        
        fclose(testFile);
        return 1;
    }
    else return 0;
}

//generates a random string for creating a temporary file
void randomFileName(char randFileName[], int length){
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //10+26+26 = 62 characters
    int i;
    for(i=0;i<length;i++){
        int index = rand() % 61; //this ensures random number is between 0 and 61
        randFileName[i] = charset[index];        
    }
}

//returns number of lines in a file
int getNumLines(char *fileName){
    FILE *f1 = fopen(fileName, "r");
    if (f1 == NULL){
        printf("Error with opening this file\n");
        return -1;
    } 
    //iterate through each character, incrementing numLines if there is a new line
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
    char *tempFileName;
    char randFileName[10];
    //Keep generating a random file name until a unique file name is found
    while(!uniqueFile){
        //Format file name string
        char randFileName[31];
        randFileName[30] = '\0';
        randomFileName(randFileName, 30); //generates random string
        tempFileName = malloc(strlen("./") + strlen(randFileName) + strlen(".txt") + 1); //allocate memory for this string
        strcpy(tempFileName, "./");
        strcat(tempFileName, randFileName);
        strcat(tempFileName, ".txt");
            
        //Check if file exists
        if (fileExists(tempFileName)){
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
    //copyOn flag is set to 0 when deleting a line
    int copyOn = 1;
    if(lineNum==1) copyOn = 0;
    int currentLine = 1;
    int s;
    s = fgetc(filePointer);    
    while(s!=EOF){
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
        //add the character if copyOn
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
    
    //rename the new one to the old name, close file and free memory
    rename(tempFileName, fileName);
    fclose(tempFile);
    free(tempFileName);
    return 0;
}

//Used when I need to delete a specified line number in a file
int progInsertLine(int lineNum, int numLines, FILE *filePointer, char *fileName, char *insertString){    
    //Open temp file
    int uniqueFile = 0;
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
    int lineInserted = 0; //boolean to test if the line has been inserted
    int s;
    s = fgetc(filePointer);        
    //if file is empty, just insert line
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
                //insert line when lineNum is reached                
                lineInserted=1;
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
        if(!lineInserted){ //to catch error if the user tries to insert on the last line but that line is empty
            int k;
            for(k=0;k<inputLen;k++){
                fputc(insertString[k], tempFile);
            }
            fputc('\n', tempFile);
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

//Used after each operation to write to changelog
int writeChangeLog(char* fileName, char *operation, int numLines){

    FILE *changelog = fopen("./changelog.log", "a");
    if (changelog == NULL){
        printf("Error with opening change log\n");
        return -1;
    }       
    
    //Get datetime
    time_t t = time(NULL); //gets the raw time (seconds since Jan 1 1970)
    struct tm tm = *localtime(&t); //creates a structure which correctly formats the raw time
    int day = tm.tm_mday;
    int month = tm.tm_mon + 1; //month num begins at 0
    int year = tm.tm_year + 1900; //counts years since 1900
    int hour = tm.tm_hour;
    int min = tm.tm_min;
    int sec = tm.tm_sec;    
    char *datetime = malloc(72 + 1); //the max bytes is 72 (i got this info from terminal errors when allocating too little memory)
    sprintf(datetime, "%02d/%02d/%d %02d:%02d:%02d", day, month, year, hour, min, sec); //formate string in correct format

    //format fileName to remove ./ at the start
    char *realFileName = fileName;
    strncpy(realFileName,fileName+2,strlen(fileName)-1);

    fprintf(changelog, "\n\n%s | %s | %s | %d", datetime, fileName, operation, numLines); //add line to change log with info

    fclose(changelog);
    free(datetime);
    return 0;
}

//File operations

//used to create a file, if the filename is new
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
    
    //open file in write mode, therefore creating it
    FILE *f1 = fopen(fileName, "w");
    if (f1 == NULL){
        printf("Error with creating this file\n");
        free(fileName);
        return -1;
    }
    
    fclose(f1); //close and save changes
    
    //add operation to change log
    char operation[] = "Create File";
    writeChangeLog(fileName, operation, getNumLines(fileName));
    
    printf("File successfully created!\n");    
    free(fileName);
    return 0;
}

//copy contents from one inputted file to another new inputted file
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
    
    //create file pointers and check that they are properly initiated
    FILE *fFROM = fopen(fileNameFROM,"r");
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
    //close files and save changes
    fclose(fFROM);
    fclose(fTO);

    //format fileName to remove ./ at the start
    char *realFileName = fileNameTO;
    strncpy(realFileName,fileNameTO+2,strlen(fileNameTO)-1);
    
    //create operation string and add to changelog
    char *operation = malloc(strlen("Copy File to new file: ") + strlen(realFileName) + 1);
    strcpy(operation, "Copy File to new file: ");
    strcat(operation, realFileName);
    writeChangeLog(fileNameFROM, operation, getNumLines(fileNameFROM));

    printf("File successfully copied!\n");    
    free(operation);
    free(fileNameFROM);
    free(fileNameTO);
    return 0;
}

//deletes the inputted file if it exists
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
    //try to remove the file, and catch an error if it occurs
    if (remove(fileName) == 0){
        printf("File successfully deleted!\n");
        char operation[] = "Delete File"; //add operation to changelog
        writeChangeLog(fileName, operation, 0);
        free(fileName);
        return 0;
    }
    else {
        printf("Error with deleting this file\n");
        free(fileName);
        return -1;
    }
}

//displays contents of inputted file
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
    printf("----------------------------------------------------------------\n"); //to separate file contents
    int s;
    s = fgetc(f1);
    while(s!=EOF){        
        printf("%c", s);
        s=fgetc(f1);
    }
    printf("\n");
    printf("----------------------------------------------------------------\n");    
    
    printf("File successfully displayed!\n");
    fclose(f1);
    free(fileName);
    return 0;
}

//Line operations

//add inputted line of contents to inputted file
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

    fprintf(f1, "\n%s", inputContents);//add the input contents on a new line
    fclose(f1);
    
    char operation[] = "Append Line"; //add operation to change log, with updated num lines
    writeChangeLog(fileName, operation, getNumLines(fileName));

    printf("Line successfully appended to file!\n");        
    free(fileName);
    return 0;
}

//delete inputted line number from inputted file
int deleteLine(){ 
    //Get file name input
    printf("Deleting inputted line number from inputted file name...\n");
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
        if(*input=='0'){ //if the user inputs a 0
            printf("Not a valid line number.\n\n");            
        }
        else{
            lineNum=atoi(input); //converts ascii to integer
            if (lineNum==0){ //would equal 0 if character was not an integer
                    printf("Invalid input entered. Must be an integer.\n\n");
            }        
            else{
                if(!(lineNum > 0 && lineNum <= numLines)){ //check line number is in valid range
                    printf("Not a valid line number.\n\n");
                }
                else validLine=1;
            }
        }
    }
    
    progDeleteLine(lineNum, numLines, f1, fileName); //call function to delete inputted line
    fclose(f1);

    //format operation and add to change log
    char *lineNumStr = malloc(strlen(input) + 1);
    sprintf(lineNumStr,"%d", lineNum);
    char *operation = malloc(strlen("Delete Line ") + strlen(lineNumStr) + 1);
    strcpy(operation, "Delete Line ");
    strcat(operation, lineNumStr);
    writeChangeLog(fileName, operation, getNumLines(fileName));
    
    printf("Line successfully deleted!\n");
    free(lineNumStr);
    free(fileName);
    free(operation);
    return 0;    
}

//insert inputted line into inputted file at inputted position
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
    
    //keep looping through until valid line number entered, similar to deleteLine()
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
            else{
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
            
    progInsertLine(lineNum, numLines, f1, fileName, insertString); //call to actually insert the line into the file
    printf("Line successfully inserted!\n");
    fclose(f1);

    //Format and Add operation to changelog
    char *lineNumStr = malloc(strlen(input) + 1);
    sprintf(lineNumStr,"%d", lineNum);    
    char *operation = malloc(strlen("Insert Line at line ") + strlen(lineNumStr) + 1);
    strcpy(operation, "Insert Line at line ");
    strcat(operation, lineNumStr);
    writeChangeLog(fileName, operation, getNumLines(fileName));

    free(lineNumStr);
    free(operation);
    free(fileName);
    return 0;
}

//display contents of inputted line in file
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
    
    //keep looping through until valid line number entered, similar to deleteLine()
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
            else{
                if(!(lineNum > 0 && lineNum <= numLines)){
                    printf("Not a valid line number.\n\n");
                }
                else validLine=1;
            }
        }
    }

    //Go through and display file character by character when desired line is reached
    int currentLine = 1;
    int s;
    s = fgetc(f1);
    printf("Line %d: ", lineNum);
    while(s!=EOF){        
        if (currentLine==lineNum){
            if(s!='\n') printf("%c", s); //don't print a new line character
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

//outputs changelog.log file to the user
int showChangeLog(){
    //Open file
    FILE *f1 = fopen("./changelog.log", "r");
    if (f1 == NULL){
        printf("Error with opening change log\n");        
        return -1;
    }
    
    //Go through and print file character by character
    printf("\nHere is the change log:\n");
    printf("----------------------------------------------------------------\n"); //larger separator as changelog normally has larger lines
    int s;
    s = fgetc(f1);
    while(s!=EOF){        
        printf("%c", s);
        s=fgetc(f1);
    }
    printf("\n");
    printf("----------------------------------------------------------------\n");

    fclose(f1);
    return 0;
}

//display the number of lines in an inputted file
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
        
    int numLines = getNumLines(fileName); //call this function to get num lines in file
    if(numLines == -1){ //if there is an error getting numLines
        return -1;
    }
    
    printf("Number of lines: %d\n", numLines); //output num lines

    free(fileName);
    return 0;

}

//Additional features

//gets current and new file names and renames old file to new file
int renameFile(){
    //Get current file name input and convert to ./filename.txt
    printf("File to RENAME\n");
    char fileNameInput[100];
    getFileNameInput(fileNameInput);
    
    char *fileName = malloc(strlen("./") + strlen(fileNameInput) + strlen(".txt") + 1);
    strcpy(fileName, "./");
    strcat(fileName, fileNameInput);
    strcat(fileName, ".txt");

    if (!fileExists(fileName)){
        printf("File does not exist!\n");        
        free(fileName);
        return -1;
    }
    //Get new file name input and convert to ./filename.txt
    printf("NEW file name\n");
    char newFileNameInput[100];
    getFileNameInput(newFileNameInput);
        
    char *newFileName = malloc(strlen("./") + strlen(newFileNameInput) + strlen(".txt") + 1);
    strcpy(newFileName, "./");
    strcat(newFileName, newFileNameInput);
    strcat(newFileName, ".txt");
    
    if(fileExists(newFileName)){
        printf("File already exists!\n");
        free(fileName);
        free(newFileName);
        return -1;
    }

    rename(fileName, newFileName);
    printf("File renamed successfully!\n");

    //Add operation to changelog
    //format newFileName to remove ./ at the start
    char *realNewFileName = newFileName;
    strncpy(realNewFileName,newFileName+2,strlen(newFileName)-1);
    char *operation = malloc(strlen("Rename to ") + strlen(realNewFileName) + 1);
    strcpy(operation, "Rename to ");
    strcat(operation, realNewFileName);
    writeChangeLog(fileName, operation, getNumLines(realNewFileName));

    free(operation);
    return 0;
}

//lists all of the .txt files in the current directory
int listTextFiles() {
    printf("List of text files in current directory:\n");
    
    DIR *dir = opendir("./"); //pointer to current directory
    struct dirent *dirEntries; //struct to hold directory entries    
    
    if (dir == NULL) {
        printf("Error opening directory\n");
        return -1;
    }
   
    //loop through all each dir entry
    while ((dirEntries = readdir(dir)) != NULL) {
                
        int nameLen = strlen((*dirEntries).d_name); //length of file name in directory

        if(nameLen > 4){ //name must be at least 4 otherwise it cant be .txt
            //check file ends in .txt
            if(strncmp((*dirEntries).d_name + nameLen - 4, ".txt", 4) == 0) {
                printf("-%s\n", (*dirEntries).d_name);
            }
        }    
    }

    // Close the directory
    if (closedir(dir) != 0) {
        printf("Error closing directory\n");
        return -1;
    }

    return 0;
}

//called to print out the user options
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

    printf("11- Rename File\n");
    printf("12- List Text Files\n");
    }

//contains main loop of the editor and runs functions based off user input
int main(){
    printf("---GROUP 127 TEXT EDITOR---\n");    
    int finished = 0;
    
    options(); //print options to user

    //create changelog file if it doesn't already exist in current directory
    if(!fileExists("./changelog.log")){
        FILE *changelog = fopen("./changelog.log", "w");
        if (changelog == NULL){
            printf("Error with creating the change log\n");
            return -1;
        }
        else{
            fprintf(changelog, "CHANGE LOG\nFORMAT: <datetime> | <filename> | <operation> | <numlines>"); //write the syntax and header
        }
        fclose(changelog);
    }    

    while (!finished){        
        int validInput = 0;
        char rawInput[50];
        int input;
        while(!validInput){
            //GET INPUT
            printf("\nChoose option--> ");
            fgets(rawInput, 50, stdin);
            
            //The fgets functions includes an enter (\n) so need to remove this
            size_t lastChar = strlen(rawInput) - 1;
            if (rawInput[lastChar] == '\n')
                rawInput[lastChar] = '\0';
                    
            //convert input to integer
            if(*rawInput=='0'){
                input=0;
                validInput=1;
            }
            else{
                input=atoi(rawInput);
                if (input==0){
                    printf("Invalid input entered. Must be an integer.\n");
                }        
                else validInput=1;
            }
        }
        //switch statement to run correct method based off usre input
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
                renameFile();
                break;
            case 12:
                listTextFiles();
                break;
            default: //default if none of the above numbers are entered
                printf("Not a valid option, try again\n");
                break;
        }
    }        
    
    return 0;
}
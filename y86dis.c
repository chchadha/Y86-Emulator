//Extra Credit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "y86emul.h"
#include <stdbool.h>

/* I created several functions each representing an instruction. My runProgram function checks the opcode and I take in two registers as parameters, as well as a pointer to my struct, and a pointer to the memory, where all my instructions are being stored */


unsigned char opcode;
unsigned char rA;
unsigned char rB;
int value = 0;
bool programcontinue=true;


int compareHexVal(char *addressValue){
    
    if(strcmp(addressValue, "00") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "10") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "20") == 0) {
        return 1;
    }
    
    
    if(strcmp(addressValue, "30") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "40") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "50") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "60") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "61") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "62") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "63") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "64") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "65") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "70") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "71") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "72") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "73") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "74") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "75") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "76") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "80") == 0) {
        return 1;
    }
    
    if(strcmp(addressValue, "90") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "a0") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "b0") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "c0") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "c1") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "d0") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "d1") == 0) {
        return 1;
    }
    if(strcmp(addressValue, "e0") == 0) {
        return 1;
    }
    
    return 0;
}

char * checkRegType(char *addressValue){
    if(strcmp(addressValue, "00") == 0) {
        return "NOP";
    }
    
    if(strcmp(addressValue, "10") == 0) {
        return "HALT";
    }
    
    if(strcmp(addressValue, "20") == 0) {
        return "rrmovl";
    }
    
    if(strcmp(addressValue, "30") == 0) {
        return "irmovl";
    }
    
    if(strcmp(addressValue, "40") == 0) {
        return "rmmovl";
    }
    
    if(strcmp(addressValue, "50") == 0) {
        return "mrmovl";
    }
    
    if(strcmp(addressValue, "60") == 0) {
        return "addl";
    }
    
    if(strcmp(addressValue, "61") == 0) {
        return "subl";
    }
    
    if(strcmp(addressValue, "62") == 0) {
        return "andl";
    }
    
    if(strcmp(addressValue, "63") == 0) {
        return "xorl";
    }
    
    if(strcmp(addressValue, "64") == 0) {
        return "mull";
    }
    
    if(strcmp(addressValue, "65") == 0) {
        return "cmpl";
    }
    
    if(strcmp(addressValue, "70") == 0) {
        return "jmp";
    }
    
    if(strcmp(addressValue, "71") == 0) {
        return "jle";
    }
    
    if(strcmp(addressValue, "72") == 0) {
        return "jl";
    }
    
    if(strcmp(addressValue, "73") == 0) {
        return "je";
    }
    
    if(strcmp(addressValue, "74") == 0) {
        return "jne";
    }
    
    if(strcmp(addressValue, "75") == 0) {
        return "jge";
    }
    
    if(strcmp(addressValue, "76") == 0) {
        return "jg";
    }
    
    if(strcmp(addressValue, "80") == 0) {
        return "call";
    }
    
    if(strcmp(addressValue, "90") == 0) {
        return "return";
    }
    if(strcmp(addressValue, "a0") == 0) {
        return "pushl";
    }
    if(strcmp(addressValue, "b0") == 0) {
        return "popl";
    }
    if(strcmp(addressValue, "c0") == 0) {
        return "readb";
    }
    if(strcmp(addressValue, "c1") == 0) {
        return "readl";
    }
    if(strcmp(addressValue, "d0") == 0) {
        return "writeb";
    }
    if(strcmp(addressValue, "d1") == 0) {
        return "writel";
    }
    if(strcmp(addressValue, "e0") == 0) {
        return "movsbl";
    }
    
  return "error";
}

char* getReg(int n)
{
    switch(n)
    {
        case 0: return "%eax";
            break;
        case 1: return "%ecx";
            break;
        case 2: return "%edx";
            break;
        case 3: return "%ebx";
            break;
        case 4: return "%esp";
            break;
        case 5: return "%ebp";
            break;
        case 6: return "%esi";
            break;
        case 7: return "%edi";
            break;
        default: return "err";
    }
}


char *checkDestination(char addresschar){
    
    if(addresschar == '0'){
        return "%eax";
    }
    if(addresschar == '1'){
        return "%ecx";
    }
    if(addresschar == '2'){
        return "%edx";
    }
    if(addresschar == '3'){
        return "%ebx";
    }
    if(addresschar == '4'){
        return "%esp";
    }
    if(addresschar == '5'){
        return "%ebp";
    }
    if(addresschar == '6'){
        return "%esi";
    }
    if(addresschar == '7'){
        return "%edi";
    }
    
    return "error";
}

void disassembler(unsigned char *memory, int arrayLength, int startingIndex, int lastIndex){
    int counter = startingIndex;
    int last = lastIndex;

    char addressValue[3]; //includes null byte
    int truthcondition;
    char *action = malloc(10);
    char *destinationReg = malloc(10);
    
    while(counter < lastIndex){
        // unsigned int value = memory[counter]; //the hex value at the address in the mem array
        //  strcpy(addressValue, &memory[counter]); //copy the contents of the hex value at that address to the char array. Convert to characters
        sprintf(addressValue,"%x", memory[counter]);

        truthcondition = compareHexVal(addressValue);
        if(truthcondition == 1){ //A hexvalue was matched
            action = checkRegType(addressValue); //it'll return a irmovl
            //Need to print memory[counter] in decimal just use %d
            
            counter++;
            sprintf(addressValue,"%x", memory[counter]); //check the address right next to it
            destinationReg = checkDestination(addressValue[1]); //return ebp
            
            if(strcmp(action, "HALT") == 0){
                printf("Index %d: %s \n", counter-1, action);
            }
            else if(strcmp(destinationReg, "error") == 0){
                printf("Index %d: %s %d \n", counter-1, action, memory[counter]);
            }
            else{
                printf("Index %d: %s $%d %s \n", counter-1, action, memory[counter], destinationReg);
            }
        }
        counter++;
    }
}


void storeInMemory(char *userInput){
    
    FILE * file=fopen(userInput,"rb");
    
    if(file == 0){ //The file could not be located.
        printf("The file could not be opened because it could not be found.\n");
    }
    
    unsigned char * memory;

    int arrayLength = 0;
    
    //Allocated memory for all components of a line.
    char * directive=(char*)malloc(sizeof(char)*10);
    char * hexaddress=(char*)malloc(sizeof(char)*10);
    char * instruction=(char*)malloc(sizeof(char)*16);
    
    //Size will be used to convert the hex value at .size to decimal.
    int size=0;
    
    //Byte and text will be used to create a seperate bytes when reading in as specified by the spec sheet.
    char * text;
    char * byte=(char*)malloc(sizeof(char)*16);
    cpu *assembler=malloc(sizeof(cpu));
    
    
    while(fscanf(file,"%500s",directive)!=EOF){
        //Checks each line's directive and stores based on directive type.
        
        if(strcmp(directive,".size")==0){
            
            fscanf(file,"%500s",directive);
            size=strtol(directive, NULL, 16);
            memory=(unsigned char*)malloc(size*sizeof(unsigned char));

        }else if(strcmp(directive,".text")==0){
            
            text=( char*)malloc(size*sizeof( char));
            fscanf(file,"%500s",hexaddress);
            fscanf(file,"%1000s",text);
            
            int address = strtol(hexaddress, NULL, 16);
            
            assembler->counter=strtol(hexaddress, NULL, 16);
            
            int index=0;
            int len=strlen(text);
            arrayLength = len;
            // printf("%d  length \n", arrayLength);
            
            
            while(index<len){
                byte[0]=text[index+1];
                byte[1]='\0';
                
                storeByte.half.high=strtol(byte, NULL, 16);
                byte[0]=text[index+0];
                byte[1]='\0';
                storeByte.half.low=strtol(byte, NULL, 16);;
                
                memory[address]=storeByte.overbyte;
                
           //printf("This is memory %d %x %d \n", address, memory[address], storeByte.half.high);
                
                index+=2;
                address++;
            }
            assembler->last=address;
           // printf("THIS IS STARTING ADDRESS %d \n", assembler->counter);
            //printf("THIS IS LAST ADDRESS %d \n", assembler->last);
        }
    
        int counter = 0;
        char addressValue[3]; //includes null byte
    
    }
    
    arrayLength = arrayLength / 2;
    //printf("ARRAY LENGTH FINAL %d", arrayLength);
    
    //I need the array where everything is stored, I need to convert the stuff in the array back to character to decipher each character and then check the opcode. If I find a matched opcode then I can check the last character of the next address to find the destination. Skip two memory locations when I find a match.
     disassembler(memory, arrayLength, assembler->counter, assembler->last);
    // runProgram(memory,assembler,);
}


int main(int argc, char **argv){
    
    //We need to grab the user input and allocate seperate each line of the file to check the directive, address, and instruciton to properly store the instruction.
    
    char * input= argv[1];
    
    //If the user doesn't enter an argument then an error is returned
    if (argv[1] == NULL){
        printf("Error: File name was not designated by name \n");
    }
    
    if(strcmp(input,"-h")==0){
        printf("This program takes in a .y86 file\n");
        exit(0);
    }
    
    storeInMemory(input);
    
    return 0;
}






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



void rrmovl(unsigned char rA, unsigned char rB, cpu *assembler, unsigned char *memory){
    
    assembler -> regs[rB] = assembler-> regs[rA]; //register rB stores register rA
    assembler->counter+=2; //increment program counter by 2
}


void irmovl(unsigned char rB, cpu *assembler, unsigned char *memory){
    
    int computation = memory[assembler->counter + 2] + (memory[assembler->counter + 3] << 8) + (memory[assembler-> counter+4] << 16) + (memory[assembler->counter +5] << 24);
    
    assembler->regs[rB] = computation; //store computation in rB
    assembler-> counter+=6;
    
}

void rmmovl(unsigned char rA, unsigned char rB, cpu *assembler, unsigned char *memory){
    
    int computation = memory[assembler->counter+2] + (memory[assembler->counter+3] << 8) + (memory[assembler->counter + 4] << 16) + (memory[assembler->counter + 5] << 24);
    
    memory[assembler->regs[rB] + computation] = (assembler->regs[rA] & 0x000000ff);
    memory[assembler-> regs[rB] + computation + 1] = (assembler->regs[rA] & 0x0000ff00)>> 8;
    memory[assembler->regs[rB] + computation + 2] = (assembler->regs[rA] & 0x00ff0000) >> 16;
    memory[assembler->regs[rB] + computation + 3] = (assembler -> regs[rA] & 0xff000000)>>24;
    assembler->counter+=6;
}

void mrmovl(unsigned char rA, unsigned char rB, cpu *assembler, unsigned char *memory){
    int computation = memory[assembler->counter+2] + (memory[assembler->counter+3] << 8) + (memory[assembler->counter + 4] << 16) + (memory[assembler->counter + 5] << 24);
    
    assembler->regs[rA] = (memory[assembler->regs[rB] + computation]) + (memory[assembler->regs[rB] + computation + 1] << 8) + (memory[assembler->regs[rB] + 2] << 16) + (memory[assembler->regs[rB]+3] << 24);
    
    assembler->counter+=6;
    
}


void addl(unsigned char rA, unsigned char rB, cpu *assembler){
    int computation = (assembler->regs[rA]) + (assembler->regs[rB]);
    
    /*set all flags to false and then check which condition is true*/
    
    assembler->of=false;
    assembler->zf=false;
    assembler->sf=false;
    
    
    if((assembler->regs[rA] > 0 && assembler->regs[rB] > 0 && computation < 0)||
       (assembler->regs[rA] < 0 && assembler->regs[rB] < 0 && computation > 0)){
        
        assembler->of=true;
    }
    if(computation<0){
        assembler->sf=true;
    }
    if(computation==0){
        assembler->zf=true;
    }
    
    assembler->regs[rB]=computation; //assign the addition computation to register B and increment counter by 2
    
    assembler->counter+=2;
}
void subl(unsigned char rA, unsigned char rB, cpu *assembler){
    
    int value = (assembler->regs[rB]) - (assembler->regs[rA]); //subtract two registers
    
    assembler->of=false;
    assembler->zf=false;
    assembler->sf=false;
    
    if((assembler->regs[rA] < 0 && assembler->regs[rB]> 0 && value <0)||
       (assembler->regs[rA] > 0 && assembler->regs[rB] < 0 && value >0)){
        assembler->of=true;
    }
    
    if(value<0){
        assembler->sf=true;
    }
    
    if(value==0){
        assembler->zf=true;
    }
    
    assembler->regs[rB]=value;
    assembler->counter+=2;
    
}

void andl(unsigned char rA, unsigned char rB, cpu *assembler){
    
    int value = (assembler->regs[rA]) & (assembler->regs[rB]);
    
    assembler->zf=false;
    assembler->sf=false;
    
    if(value<0){
        assembler->sf=true;
    }
    
    if(value==0){
        assembler->zf=true;
    }
    
    assembler->counter+=2;
}
void xorl(unsigned char rA, unsigned char rB, cpu *assembler){
    
    int value= assembler->regs[rA] ^ assembler->regs[rB];
    
    assembler->zf=false;
    assembler->sf=false;
    
    if(value<0){
        assembler->sf=true;
    }
    
    if(value==0){
        assembler->zf=true;
    }
    
    assembler->counter+=2;
}

void mull(unsigned char rA, unsigned char rB, cpu *assembler){
    
    int value = (assembler->regs[rA]) * (assembler->regs[rB]);
    
    assembler->of=false;
    assembler->zf=false;
    assembler->sf=false;
    
    if((assembler->regs[rA] > 0 && assembler->regs[rB] > 0 && value<0)||
       (assembler->regs[rA] < 0 && assembler->regs[rB] < 0 && value<0)||
       (assembler->regs[rA] > 0 && assembler->regs[rB] < 0 && value>0)||
       (assembler->regs[rA] < 0 && assembler->regs[rB] > 0 && value>0)){
        
        assembler->of=true;
    }
    if(value<0){
        assembler->sf=true;
    }
    if(value==0){
        assembler->zf=true;
    }
    
    assembler->regs[rB]=value;
    assembler->counter+=2;
}

void cmpl(unsigned char rA, unsigned char rB, cpu *assembler){
    
    if(assembler->regs[rA] == assembler->regs[rB]){
        assembler->zf=true;
    }
    else{
        assembler->zf=false;
    }
    if(assembler->regs[rB]-assembler->regs[rA]<0){
        assembler->sf=true;
    }
    else{
        assembler->sf=false;
    }
    
    assembler->counter+=2;
}

void jmp(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1])  + (memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    assembler->counter = computation;
    
}

void jle(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1]) + (memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if(assembler->zf == 1||(assembler->sf != assembler->of)){
        assembler->counter=computation;
    }else{
        assembler->counter+=5;
    }
    
}
void jl(cpu *assembler, unsigned char *memory){
    int computation = (memory[assembler->counter+1]) + (memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if((assembler->sf != assembler->of)){
        
        assembler->counter=computation;
        
    }else{
        
        assembler->counter+=5;
    }
}

void je(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1]) + (memory[assembler->counter+2]<<8) +(memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if(assembler->zf == 1){
        assembler->counter = computation;
    }else{
        assembler->counter+=5;
    }
    
}

void jne(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1]) +(memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if(assembler->zf == 0){
        assembler->counter = computation;
        
    }else{
        assembler->counter+=5;
    }
}

void jge(cpu *assembler, unsigned char *memory){
    int computation = (memory[assembler->counter+1]) + (memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if(assembler->zf == assembler->of){
        
        assembler->counter = computation;
        
    }else{
        assembler->counter+=5;
    }
    
}
void jg(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1]) + (memory[assembler->counter+2]<<8) + (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    if(assembler->zf == 0 && (assembler->sf == assembler->of)){
        
        assembler->counter=computation ;
        
    }else{
        assembler->counter+=5;
    }
}

void call(cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+1])+ (memory[assembler->counter+2]<<8)+ (memory[assembler->counter+3]<<16) + (memory[assembler->counter+4]<<24);
    
    assembler->regs[ESP]-=4;
    
    assembler->instructionp = assembler->counter+5;
    memory[assembler->regs[ESP]] = (assembler->instructionp & 0x000000ff);
    memory[assembler->regs[ESP]+1] = (assembler->instructionp & 0x0000ff00)>>8;
    memory[assembler->regs[ESP]+2] = (assembler->instructionp & 0x00ff0000)>>16;
    memory[assembler->regs[ESP]+3] = (assembler->instructionp & 0xff000000)>>24;
    
    assembler->counter=computation;
}

void ret(cpu *assembler, unsigned char *memory){
    
    assembler->counter = (memory[assembler->regs[ESP]])+
    (memory[assembler->regs[ESP]+1]<<8)+
    (memory[assembler->regs[ESP]+2]<<16)+
    (memory[assembler->regs[ESP]+3]<<24);
				
    assembler->regs[ESP]+=4;
}


void pushl(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    assembler->regs[ESP]-=4;
    
    memory[assembler->regs[ESP]] = (assembler->regs[rA] & 0x000000ff);
    memory[assembler->regs[ESP]+1] = (assembler->regs[rA] & 0x0000ff00)>>8;
    memory[assembler->regs[ESP]+2] = (assembler->regs[rA] & 0x00ff0000)>>16;
    memory[assembler->regs[ESP]+3] = (assembler->regs[rA] & 0xff000000)>>24;
    
    assembler->counter+=2;
}

void popl(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    assembler->regs[rA] =(memory[assembler->regs[ESP]])+
    (memory[assembler->regs[ESP]+1]<<8)+
    (memory[assembler->regs[ESP]+2]<<16)+
    (memory[assembler->regs[ESP]+3]<<24);
				
    assembler->regs[ESP]+=4;
    assembler->counter+=2;
}

void readb(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+2]) + (memory[assembler->counter+3]<<8) + (memory[assembler->counter+4]<<16) + (memory[assembler->counter+5]<<24);
    
    if(EOF==scanf("%c",&(assembler->instructioncp))){
        assembler->zf=true;
    }
    else{
        assembler->zf=false;
    }
    
    memory[assembler->regs[rA]+computation] = assembler->instructioncp;
    assembler->counter+=6;
    
}

void readl(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+2]) +(memory[assembler->counter+3]<<8) +(memory[assembler->counter+4]<<16) +(memory[assembler->counter+5]<<24);
    
    if(EOF==scanf("%i",&(assembler->instructionp))){
        assembler->zf=true;
        
    }else{
        assembler->zf=false;
    }
    
    memory[assembler->regs[rA]+computation] = (assembler->instructionp & 0x000000ff);
    memory[assembler->regs[rA]+1+computation] = (assembler->instructionp & 0x0000ff00)>>8;
    memory[assembler->regs[rA]+2+computation] = (assembler->instructionp & 0x00ff0000)>>16;
    memory[assembler->regs[rA]+3+computation] = (assembler->instructionp & 0xff000000)>>24;
    
    assembler->counter+=6;
}

void writeb(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    int computation = (memory[assembler->counter+2]) + (memory[assembler->counter+3]<<8) + (memory[assembler->counter+4]<<16) + (memory[assembler->counter+5]<<24);
    
    printf("%c",memory[assembler->regs[rA]+computation]);
    
    assembler->counter+=6;
}


void writel(unsigned char rA, cpu *assembler, unsigned char *memory){
    
    
    int computation = (memory[assembler->counter+2]) +(memory[assembler->counter+3]<<8)
    +(memory[assembler->counter+4]<<16) +(memory[assembler->counter+5]<<24);
    
    int finalval =(memory[assembler->regs[rA]+computation])+
    (memory[assembler->regs[rA]+1+computation]<<8)+
    (memory[assembler->regs[rA]+2+computation]<<16)+
    (memory[assembler->regs[rA]+3+computation]<<24);
    
    printf("%i",finalval);
    assembler->counter+=6;
}

void movsbl(unsigned char rA, unsigned char rB, cpu *assembler, unsigned char *memory){
    int computation = (memory[assembler->counter+2]) +(memory[assembler->counter+3]<<8) +(memory[assembler->counter+4]<<16) +(memory[assembler->counter+5]<<24);
    
    
    
    int rAval =(memory[assembler->regs[rB]+computation])+(memory[assembler->regs[rB]+computation +1]<<8)+(memory[assembler->regs[rB]+2]<<16)+(memory[assembler->regs[rB]+3]<<24);
    
    int rBcomp = rAval & 0x000000ff;
    
    assembler->regs[rA]=rBcomp;
    assembler->counter+=6;
}



void runProgram(unsigned char *memory,cpu *assembler){
    
    while(programcontinue == true){
        
        opcode = memory[assembler->counter];
       
        switch(opcode){
            case 0x00:
                assembler -> counter++;
                break;
            case 0x10:
                programcontinue = false;
                break;
            case 0x20:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                rrmovl(rA,rB,assembler,memory);
                break;
            case 0x30:
                rB=memory[assembler->counter+1]&0x0f;
                irmovl(rB,assembler,memory);
                break;
            case 0x40:
               	rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                rmmovl(rA,rB,assembler,memory);
                break;
            case 0x50:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                mrmovl(rA,rB,assembler,memory);
                break;
            case 0x60:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                addl(rA,rB,assembler);
                break;
            case 0x61:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                subl(rA,rB,assembler);
                break;
            case 0x62:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                andl(rA,rB,assembler);
                break;
            case 0x63:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                xorl(rA,rB,assembler);
                break;
            case 0x64:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                mull(rA,rB,assembler);
                break;
            case 0x65:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                cmpl(rA,rB,assembler);
                break;
            case 0x70:
                jmp(assembler,memory);
                break;
            case 0x71:
                jle(assembler,memory);
                break;
            case 0x72:
                jl(assembler,memory);
                break;
            case 0x73:
                je(assembler,memory);
                break;
            case 0x74:
                jne(assembler,memory);
                break;
            case 0x75:
                jge(assembler,memory);
                break;
            case 0x76:
                jg(assembler,memory);
                break;
            case 0x80:
                call(assembler,memory);
                break;
            case 0x90:
                ret(assembler,memory);
                break;
            case 0xa0:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                pushl(rA,assembler,memory);
                break;
            case 0xb0:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                popl(rA,assembler,memory);
                break;
            case 0xc0:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                readb(rA,assembler,memory);
                break;
            case 0xc1:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                readl(rA,assembler,memory);
                break;
            case 0xd0:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                writeb(rA,assembler,memory);
                break;
            case 0xd1:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                writel(rA,assembler,memory);
                break;
            case 0xe0:
                rA=memory[assembler->counter+1]>>4&0xff;
                rB=memory[assembler->counter+1]&0x0f;
                movsbl(rA,rB,assembler,memory);
                break;
            default:
                printf("error\n");
                programcontinue=false;
        }
        rA=0;
        rB=0;
    }
}


void storeInMemory(char *userInput){
    
    FILE * file=fopen(userInput,"rb");
    
    if(file == 0){ //The file could not be located.
        printf("The file could not be opened because it could not be found.\n");
    }
    
    unsigned char * memory;
    
    
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
            
        }else if(strcmp(directive,".string")==0){
            
            unsigned char character;
            fscanf(file,"%500s",hexaddress);
            int address= (strtol(hexaddress, NULL, 16))- 2;
            
            
            while(character!= EOF){
                character = fgetc (file);
                
                if(character!='"'){
                    memory[address]=character;
                }
                if (character == '\n'){
                    break;
                }
                
                address++;
            }
            
        }else if(strcmp(directive,".long")==0){
            
            fscanf(file,"%500s",hexaddress);
            fscanf(file,"%500s",instruction);
            
            long instruct =atoi(instruction);
            storeLong.num = instruct;
            
            int address = strtol(hexaddress, NULL, 16);
            memory[address]=storeLong.byte[0];
            
        }else if(strcmp(directive,".byte")==0){
            
            fscanf(file,"%500s",hexaddress);
            fscanf(file,"%500s",instruction);
            
            byte[0]=instruction[1];
            byte[1]='\0';
            storeByte.half.high=strtol(byte, NULL, 16);
            byte[0]=instruction[0];
            byte[1]='\0';
            
            storeByte.half.low= strtol(byte, NULL, 16);
            int address = strtol(hexaddress, NULL, 16);
            memory[address]=storeByte.overbyte;
            
        }else if(strcmp(directive,".text")==0){
            
            text=( char*)malloc(size*sizeof( char));
            fscanf(file,"%500s",hexaddress);
            fscanf(file,"%1000s",text);
            
            int address = strtol(hexaddress, NULL, 16);
            
            assembler->counter=strtol(hexaddress, NULL, 16);
            
            int index=0;
            int len=strlen(text);
            
            while(index<len){
                byte[0]=text[index+1];
                byte[1]='\0';
                
                storeByte.half.high=strtol(byte, NULL, 16);
                byte[0]=text[index+0];
                byte[1]='\0';
                storeByte.half.low=strtol(byte, NULL, 16);;
                
                memory[address]=storeByte.overbyte;
                
                index+=2;
                address++;
            }
            assembler->last=address;
        }
    }
    runProgram(memory,assembler);
    
    
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






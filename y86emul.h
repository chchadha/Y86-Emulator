
#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7


//In order to use boolean values on my flags I created a struct consisting of both true and false which would be assigned to each flag.

typedef enum boolean {
    true,
    false
}boolean;

//My struct consists of all the necessary infomration needed included the array for my registers, a program counter, a holder for my text address (where program needs to begin) and over flow flags.

typedef struct processor{
    
    int regs[8];
    int counter;
    int textAddress;
    int last;
    int instructionp;
    char instructioncp;
    boolean of;
    boolean zf;
    boolean sf;
    
}cpu;



//Store provides the ability to store a long integer into an unsigined character array. I needed to use this union for my .long directive which owuld allow me to store my long in a byte array of 4 bytes
union store{
    
    long num;
    unsigned char byte[4];
    
}storeLong;


//Byte will allow to store a byte into a 4 bit integer.
//The union Byte allows me to store a byte into a 4 bit integer. High and low are used to represent the higher and lower halves of the byte.
union Byte{
    
    struct regpair{
        unsigned int high:4;
        unsigned int low:4;
    }half;
    
    unsigned char overbyte;
    
}storeByte;










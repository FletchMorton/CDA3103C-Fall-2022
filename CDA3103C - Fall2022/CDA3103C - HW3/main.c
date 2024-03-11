//Fletcher Morton
//HW3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structs
typedef struct Instruction Instruction;
struct Instruction {
    int opCode;
    int deviceOrAddress;
};

//Prototypes
void fetch();
void load();
void add();
void store();
void sub();
void in();
void out();
void end();
void jump();
void skip();


//Global Variables
Instruction IM[100];  //Instruction Memory
Instruction IR;       //Instruction register
Instruction MBR;      //Memory data register
int DM[10];           //Data memory
int MAR = 0;          //Memory address register
int PC  = 0;          //Program counter
int AC  = 0;          //Accumulator
int run = 1;          //Determines if the program should be running. Initialized to true


int main(int argc, char **argv)
{
    //Check if the file is provided correctly
    if(argc != 2) {
        printf("\nFile not found.\n");
        exit(0);
    }

    //Variables that will help read the input file
    FILE *inputFile;
	char delimiter[] = " ";
	char *dummyPtr;
	char *subString1;
	char *subString2;
	int opCode;
	int address;

    //Opens 'input.txt' in read mode and creates a buffer string for reading
    inputFile = fopen(argv[1], "r");
    char buffer[5];

    printf("\nAssembling the program...\n");

    //Read contents of file into the Instruction Memory
    for(int i = 0;; i++) {
        if(feof(inputFile)) break; //If the end of the file is reached, break the while loop

        //Get user input and tell STRTOK where it should split it up
        fgets(buffer, 5, inputFile);
        char *snippet = strtok(buffer, delimiter);

        //Assign each snippet of the string to a new string
        subString1 = snippet;
        snippet = strtok(NULL, delimiter);
        subString2 = snippet;

        //STRTOL will convert the string integer into a long int (here casted to an integer).
        //STRTOL takes the string, a pointer to separate the int and the rest of the string, and the base of the number (here base 10)
        opCode  = (int)strtol(subString1, &dummyPtr, 10);
        address = (int)strtol(subString2, &dummyPtr, 10);

        IM[i].opCode = opCode;
        IM[i].deviceOrAddress = address;
    }

    printf("Program successfully assembled\n\nRun.\n\n");
    printf("PC = 10 | A = NULL | DM = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n\n");

    //Fetch, Decode, and Execute
    while(run == 1) {
        //--------------------------------FETCH-------------------------------

        fetch();

        //-------------------------------DECODE-------------------------------

        switch(IR.opCode) {
            case 1:
                //Execute
                printf("> Loading data into the Accumulator...\n");
                load();
                break;

            case 2:
                //Execute
                printf("> Adding data to the value in the Accumulator...\n");
                add();
                break;

            case 3:
                //Execute
                printf("> Storing data memory...\n");
                store();
                break;

            case 4:
                //Execute
                printf("> Subtracting data from the value in the Accumulator...\n");
                sub();
                break;

            case 5:
                //Execute
                printf("> Enter your value:\n> ");
                in();
                break;

            case 6:
                //Execute
                printf("> Outputting the value in the Accumulator...\n");
                out();
                break;

            case 7:
                //Execute
                printf("> Goodbye\n");
                end();
                break;

            case 8:
                //Execute
                printf("> Jumping to address...\n");
                jump();
                break;

            case 9:
                //Execute
                printf("> Skipping next instruction...\n");
                skip();
                break;

            default:
                ;
        }//Switch statement

        //Displaying the current state of the Program Counter, Accumulator, and Data Memory
        printf("\nPC = %d | A = %d | DM = [%d", PC+10, AC, DM[0]);
        for(int i = 1; i < 10; i++) printf(", %d", DM[i]);
        printf("]\n\n");

    }//while statement

    //Close the file
    fclose(inputFile);

    //return
    return 0;
}

//______________________________________________________________________________________

//Sends the program counter to memory to fetch the memory address (index) of the next instruction in IM
void fetch()
{
    //PC passes the memory address to MAR
    MAR = PC;

    //PC is updated
    PC++;

    //MAR passes the opCode and deviceOrAddress to MBR
    MBR.opCode = IM[MAR].opCode;
    MBR.deviceOrAddress = IM[MAR].deviceOrAddress;

    //MBR passes the opCode and deviceOrAddress to IR
    IR.opCode = MBR.opCode;
    IR.deviceOrAddress = MBR.deviceOrAddress;
}

//______________________________________________________________________________________

//Load data into the AC from the data memory at the address the instruction register currently has access to
void load()
{
    //IR passes the memory address its holding to MAR
    MAR = IR.deviceOrAddress;

    //The data in DM at the memory address provided by MAR is stored into MBR
    MBR.deviceOrAddress = DM[MAR];

    //MAR passes the deviceNumber to the accumulator
    AC = MBR.deviceOrAddress;
}

//______________________________________________________________________________________

//Adds the value of the specified data in the DM to the value currently stored in the accumulator
void add()
{
    //IR passes the memory address its holding to MAR
    MAR = IR.deviceOrAddress;

    //The data in DM at the memory address provided by MAR is stored into MBR
    MBR.deviceOrAddress = DM[MAR];

    //MAR's deviceNumber value is added to the value in the accumulator
    AC += MBR.deviceOrAddress;
}

//______________________________________________________________________________________

//Takes the value currently occupying the AC, and stores it in data memory at the address the instruction register currently has access to
void store()
{
    //IR passes the memory address its holding to MAR
    MAR = IR.deviceOrAddress;

    //AC passes its deviceNumber to MBR
    MBR.deviceOrAddress = AC;

    //MBR stores the deviceNumber from AC into data memory at the memory address stored in MAR
    DM[MAR] = MBR.deviceOrAddress;
}

//______________________________________________________________________________________

//Subtracts the value of the specified data in the DM from the value currently stored in the accumulator
void sub()
{
    //IR passes the memory address its holding to MAR
    MAR = IR.deviceOrAddress;

    //The data in DM at the memory address provided by MAR is stored into MBR
    MBR.deviceOrAddress = DM[MAR];

    //MAR's deviceNumber value is subtracted from the value in the accumulator
    AC -= MBR.deviceOrAddress;
}

//______________________________________________________________________________________

//Takes input from the user and stores it in the AC
void in()
{
    //Takes input from the user
    int deviceNumber;
    scanf("%d", &deviceNumber);

    //Pass it to the Accumulator
    AC = deviceNumber;
}

//______________________________________________________________________________________

//Prints the value in AC to the screen
void out()
{
    printf("> %d\n", AC);
}

//______________________________________________________________________________________

//Halts the program
void end()
{
    run = 0;
}

//______________________________________________________________________________________

//Changes the program counter's value, effectively skipping (or jumping) over all the indices between the old and new values
void jump()
{
    //Set the value of the program counter to the address stored in the instruction register
    PC = IR.deviceOrAddress;
}

//______________________________________________________________________________________

//Skips the current index if the value in the AC is equal to zero
void skip()
{
    //If the value in the Accumulator is zero, increment the program counter, effectively skipping an instruction
    if(AC == 0) PC++;
}

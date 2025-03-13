/*****************************************************************************
TITLE: Claims                                                                                                                               
AUTHOR: MD.Khaled Hussain
ROLL NO.: 2301CS24
Declaration of Authorship
This cpp file, asm.cpp, is part of the assignment of CS2102 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
using namespace std;
#define ll long long
#define MOD 1000000007LL
#define all(x) x.begin(), x.end() 
#define pb push_back
#define endl "\n"
#define sz(x) x.size()
#define fr first
#define sc second

// Convert integer to hexadecimal
string int_to_hex(int i) {
    char buffer[9]; // 8 characters for hex + 1 for null terminator
    sprintf(buffer, "%08x", i); // Format integer as hex
     //%x formats the integer as a hexadecimal.
    // 08 implies a minimum width of 8 characters(add zeroes if not there)
    return string(buffer);
}

vector<int> memory(100005);
int PC,SP,A,B;

vector<int> machcodes;
// stores machine codes

struct reading {
    int add; // memory address that was read
    int value; // value that was read from memory address
};

struct writing {
    int add; // memory address that was written to
    int prev; // value that was stored at the memory address before the write operation
    int present; // value that was stored at the memory address after the write operation
};
vector<reading>read_1;
vector<writing>write_1;

// Print Instruction set architecture
void ISA() {
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}

// MOT table
map<int,string>mot;
void mot_init() {
    // map op codes to mnemonics
    mot[0] = "ldc";
    mot[1] = "adc";
    mot[2] = "ldl";
    mot[3] = "stl";
    mot[4] = "ldnl";
    mot[5] = "stnl";
    mot[6] = "add";
    mot[7] = "sub";
    mot[8] = "shl";
    mot[9] = "shr";
    mot[10] = "adj";
    mot[11] = "a2sp";
    mot[12] = "sp2a";
    mot[13] = "call";
    mot[14] = "return";
    mot[15] = "brz";
    mot[16] = "brlz";
    mot[17] = "br";
    mot[18] = "HALT";
    mot[19] = "data";
    mot[20] = "SET";
}
//seoarate opcode and operand
void instr_to_operand_opcode(int instr,int &operand,int &opcode) {
    opcode=instr &0xff;
//0xff(11111111) & operation gives only the least significant 8 of instr(opcode) (last byte)(xy)
    operand=instr & 0xffffff00;
    //0xffffff00(11111111 11111111 11111111 00000000) & operation gives only the top significant 24 of instr(operand)
    //most significant 3 bytes(abcdef00)
    operand>>=8;
    //rightmost 8 bits are discarded(abcdef)

}
// making all functions
void ldc(int operand) {
    B = A;
    A = operand;
}
void adc(int operand) {
    A += operand;
}

void ldl(int operand) {
    B = A;
    A = memory[SP + operand];
    read_1.push_back({SP + operand, A});
}

void stl(int operand) {
    int previ = memory[SP + operand];
    memory[SP + operand] = A;
    A = B;
    write_1.push_back({SP + operand, previ, memory[SP + operand]});
}

void ldnl(int operand) {
    int previ = A;
    A = memory[A + operand];
    read_1.push_back({previ + operand, A});
}

void stnl(int operand) {
    int previ = memory[A + operand];
    memory[A + operand] = B;
    write_1.push_back({A + operand, previ, B});
}

void add() {
    A += B;
}

void sub() {
    A = B - A;
}
void shl() {
    A = B << A;
}
void shr() {
    A = B >> A;
}

void adj(int operand) {
    SP += operand;
}

void a2sp() {
    SP = A;
    A = B;
}

void sp2a() {
    B = A;
    A = SP;
}
void call(int operand) {
    B = A;
    A = PC;
    PC += operand;
}
void ret() {
    PC = A;
    A = B;
}
void brz(int operand) {
    if (A == 0) PC += operand;
}
void brlz(int operand) {
    if (A < 0) PC += operand;
}
void br(int operand) {
    PC += operand;
}

int trace(int &PC, int pos, ofstream& trcfile) {
    // pos(target position where execution should stop)
    //(trcfile) output file stream that will be used to log the trace information to a file)
    int cnt = 0;
    while (PC < pos) {
        int old_pc = PC;//to check for infinite loops
        int32_t operand, operation;
        int32_t instr = memory[PC];//get the instr from memory at address pC
        instr_to_operand_opcode(instr, operand, operation);

        //printing the  current state of(PC,SP,A,B)

        cout << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: "
             << int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << mot[operation]
             << " " << operand << endl << endl;

             // write trace info to trcfile

        trcfile << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: "
                << int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << mot[operation]
                << " " << operand << endl << endl;

        if (operation == 0) {
            ldc(operand);
        } else if (operation == 1) {
            adc(operand);
        } else if (operation == 2) {
            ldl(operand);
        } else if (operation == 3) {
            stl(operand);
        } else if (operation == 4) {
            ldnl(operand);
        } else if (operation == 5) {
            stnl(operand);
        } else if (operation == 6) {
            add();
        } else if (operation == 7) {
            sub();
        } else if (operation == 8) {
            shl();
        } else if (operation == 9) {
            shr();
        } else if (operation == 10) {
            adj(operand);
        } else if (operation == 11) {
            a2sp();
        } else if (operation == 12) {
            sp2a();
        } else if (operation == 13) {
            call(operand);
        } else if (operation == 14) {
            ret();
        } else if (operation == 15) {
            brz(operand);
        } else if (operation == 16) {
            brlz(operand);
        } else if (operation == 17) {
            br(operand);
        } else if (operation == 18) {
            cout << cnt << " number of instructions executed" << endl << endl;
            return 0;
        }

        PC++;
        if (old_pc == PC) {
            // pC not changed after execution(stuck (same instrc))
            cout << "Infinite loop detected" << endl;
            break;
        }
        cnt++;
    }
    cout << cnt << " number of instructions executed" << endl;
    return 0;
}


int main() {
    string nameoffile;
    cout << "Enter the file name (with extension): ";
    cin >> nameoffile;

    string trace_filename = nameoffile;
    // check for .o and make .trace
    if (trace_filename.size() > 2 && trace_filename.substr(trace_filename.size() - 2) == ".o") {
        trace_filename.replace(trace_filename.size() - 2, 2, ".trace");
    }

    ifstream file(nameoffile, ios::in | ios::binary); //read file
    ofstream trcfile(trace_filename);//writing trace into this

    unsigned int cur;
    int counter = 0;
    //reading machine code into memory
    while (file.read((char*)&cur, sizeof(int))) {
        machcodes.push_back(cur);
        memory[counter++] = cur;
    }
    int size = machcodes.size();
    mot_init();
 // menu loop
    while (true) {
        cout << "MENU\n";
        cout << "1: To get trace\n";
        cout << "2: To display ISA\n";
        cout << "3: To show memory dump before execution (only before running trace)\n";
        cout << "4: To show memory dump after execution (only after running trace)\n";
        cout << "5: Show memory reads\n";
        cout << "6: Show memory writes\n";
        cout << "7: Show registers and SP values\n";
        cout << "Any number >=8 to exit\n\n";
        cout << "Enter the type of instruction you want to execute: ";

        int type;
        cin >> type;

        if (type == 1) {
            trace(PC, size, trcfile);
        } else if (type == 2) {
            ISA();
        } else if (type == 3) {
            cout << "Memory dump before execution" << endl;
            for (int i = 0; i < size; i += 4) {
                cout << int_to_hex(i) << " ";
                for (int j = i; j < min(size, i + 4); ++j) {
                    cout << int_to_hex(machcodes[j]) << " ";
                }
                cout << "\n";
            }
        } else if (type == 4) {
            cout << "Memory dump after execution" << endl;
            for (int i = 0; i < size; i += 4) {
                cout << int_to_hex(i) << " ";
                for (int j = i; j < min(size, i + 4); ++j) {
                    cout << int_to_hex(memory[j]) << " ";
                }
                cout << "\n";
            }
        } else if (type == 5) {
            // display memory reads
            if (read_1.empty()) {
                cout << "There is no memory read instruction" << endl;
            } else {
                for (const auto& read : read_1) {
                    cout << "Read memory[" << int_to_hex(read.add) << "] found " << int_to_hex(read.value) << "\n";
                }
            }
        } else if (type == 6) {
             // display memory writes
            if (write_1.empty()) {
                cout << "There is no memory write instruction" << endl;
            } else {
                for (const auto& write : write_1) {
                    cout << "Wrote memory[" << int_to_hex(write.add) << "] was " << int_to_hex(write.prev)
                         << " now " << int_to_hex(write.present) << "\n";
                }
            }
        } else if (type == 7) {
            // display registers
            cout << "PC : " << int_to_hex(PC) << "\tSP : " << int_to_hex(SP) << "\tA : " << int_to_hex(A) << "\tB : " << int_to_hex(B) << endl;
        } else {
            break;
        }
    }
    return 0;
}

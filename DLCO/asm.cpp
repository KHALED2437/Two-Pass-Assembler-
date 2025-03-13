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


// structure for symbols
struct symbol{
    string name;
    int address;
    bool set;//flag (whether symbol has been assigned a specific value)
    int set_value;

};
// structure for literals(numerical const needs to be stored in memory)
struct literal{
    int literal;
    int address;
};
// map b/w mnemonics and hexadecimal machine codes,// map b/w instructions and opcode
unordered_map<string,string>mot;
void init(){
mot = {
    {"ldc", "00"},
    {"adc", "01"},
    {"ldl", "02"},
    {"stl", "03"},
    {"ldnl", "04"},
    {"stnl", "05"},
    {"add", "06"},
    {"sub", "07"},
    {"shl", "08"},
    {"shr", "09"},
    {"adj", "0a"},
    {"a2sp", "0b"},
    {"sp2a", "0c"},
    {"call", "0d"},
    {"return", "0e"},
    {"brz", "0f"},
    {"brlz", "10"},
    {"br", "11"},
    {"HALT", "12"},
    {"data", "13"},
    {"SET", "14"}
};
}






bool correct_label(string &s) {
    int k = s.size();
    // empty label
    if (k == 0) {
        return false;
    }
    // check if the first character is a letter
    if (!isalpha(s[0])) {
        return false;
    }
    // check remaining characters for alphanumeric or underscore(for a test case)
    for (int i = 1; i < k; ++i) {
        if (!isalnum(s[i]) && s[i] != '_') {
            return false;
        }
    }
    return true;
}

// literal and symbol tables
vector<symbol>symbol_table;
vector<literal>literal_table;


//     handles both decimal and hexadecimal formats,
// checking if a given string represents a valid integer number
bool is_number(const string &s){
    int k=s.size();
  // empty string case
    if(s.empty()){
        return false;
    }
    // skip if sign is present
    int start=0;
    if(s[0]=='-' || s[0]=='+'){
        start=1;
    }
    // check for hexadecimal prefix of "0x"or "0X"

    if((k>start+1)&& (s[start]=='0') &&((s[start+1]=='x') || (s[start+1]=='X'))){
       // checking whether all remaining characters are valid hexadecimal digits
     return (s.size()>start + 2) && all_of(s.begin() + start + 2, s.end(), [](char c) { return isxdigit(c); });
 // lambda function to each character starting from the position after "0x"

    }
    // ensure all remaining characters are digits, // if not hexadecimal
    return all_of(s.begin() + start, s.end(), [](char c) { return isdigit(static_cast<unsigned char>(c)); });

}

//  function to deal with white spaces
string trim(string s) {
    // Find the first character that is not a whitespace(tabs,newline..etc)
    size_t start = s.find_first_not_of("\t\n\r\f\v ");
    
    // If non-whitespace character is not found, return an empty string
    if (start == string::npos) {
        return "";
    }
    
    // Find the last character that is not a whitespace
    size_t end = s.find_last_not_of("\t\n\r\f\v ");
    
    // If no non-whitespace character is found at the end, return the substring from start
    if (end == string::npos) {
        return s.substr(start);
    }
    
    // Otherwise, return the substring from start to end (inclusive)
    return s.substr(start, end - start + 1);
}



// string to number into given base
long long tonum(const string& s, int base) {
    if (base < 2 || base > 36) {
        throw invalid_argument("Base must be between 2 and 36.");
    }

    ll result = 0;// storing num
    bool isNegative = false;
    size_t start = 0;

    // Handle sign
    if (s[start] == '-') {
        isNegative = true;
        start = 1;
    } else if (s[start] == '+') {
        start = 1;
    }

    // Detect "0x" or "0X" and switch to base 16 if it's not already
    if (s.size() > start + 1 && s[start] == '0' && (s[start + 1] == 'x' || s[start + 1] == 'X')) {
        base = 16;  // Override to hexadecimal
        start =start+2; // Skip "0x" or "0X" prefix
    }

    // Iterate over the string, starting from the relevant position
    for (int i = start; i < s.size(); ++i) {
        char c = s[i];

        int value;
        if (isdigit(c)) {
            value = c - '0'; // Convert character '0'-'9' to 0-9
        } else if (isalpha(c)) {
            value = toupper(c) - 'A' + 10; // Convert 'A'-'Z' to 10-35
        } else {
            throw invalid_argument("Invalid character for base " + std::to_string(base) + ": " + c);
        }

        if (value >= base) {
            throw invalid_argument("Invalid character for base " + std::to_string(base) + ": " + c);
        }

        result = result * base + value; // Base conversion
    }

    // Apply the negative sign if needed
    if (isNegative) {
        result = -result;
    }
    return result;
}
// to check for elements in symbol table
bool contain(const string& name)
{
   // lambda funct to iterate over symbol table and check for name
   return any_of(symbol_table.begin(), symbol_table.end(), [&](const symbol& sym) { return sym.name == name; });
}
// convert integer to hexadecimal
string int_to_hex(int i) {
    char buffer[9]; // 8 characters for hex + (1 for null terminator,\0)
    sprintf(buffer, "%08x", i); // Format integer as hex
    //%x formats the integer as a hexadecimal.
    // 08 implies a minimum width of 8 characters(add zeroes if not there)
    return string(buffer);
}



// Function to check for the presence of a symbol in the symbol table
bool Is_Symbol_Present(const vector<symbol>& table, const string& symbol) {
    for (auto entry = table.begin(); entry != table.end(); ++entry) {
        if (entry->name == symbol) {
            return true; // Symbol found
        }
    }
    return false; // Symbol not found
}






// Helper functions
// does not require any operand
bool is_no_operand_operation(const string& op) {
    // Operations that don't require operands
    static const unordered_set<string> no_operand_ops = {
        "add", "sub", "shl", "shr", "a2sp", "sp2a", "return", "HALT"
    };
    // Return true if the operation is in the set
    return no_operand_ops.find(op) != no_operand_ops.end();
}
bool is_branching_operation(const string& op) {
    // Operations that represent branching instructions
    static const unordered_set<string> branching_ops = {
        "call", "brz", "brlz", "br"
    };
    // Return true if the operation is in the set
    return branching_ops.find(op) != branching_ops.end();
}





// Reading instructions and adding into symbol and literal tables and checking for errors and warnings
string instruction_to_table(string instr, int& PC, int line)
{
    int loc = PC;  //line(for line num for detecting error),PC(current memory location for instructions)
    string errors = "";
    

    // Identify label and variables  (label:instrc)
    size_t colon_pos = instr.find(':');
    if (colon_pos != string::npos)  // assumed to have a label
    {      
        string label = trim(instr.substr(0, colon_pos));  // string before colon

        // Check for duplicate labels
        if (Is_Symbol_Present(symbol_table, label))
        {
            string error_message = "ERROR: Duplicate Label at line " + to_string(line);
            cout << error_message << endl;
              errors += error_message + "\n"; // Append error message
        }

        // Check label format
        if (!correct_label(label))
        {
            string warning_message = "WARNING: Incorrect label format at line " + to_string(line);
            cout << warning_message << endl;
            errors += warning_message + "\n"; // Append warning message
        }

        // If there is instruction after the label
        if (colon_pos != instr.length() - 1)
        {
            //part of instrc after colon
            string subs = trim(instr.substr(colon_pos + 1));
            // sub operation,operand
            string sub_op, sub_val;
             // find space to separate operation,operand
            size_t space_pos = subs.find(" ");
            if (space_pos != string::npos) {
                sub_op = trim(subs.substr(0, space_pos));
                sub_val = trim(subs.substr(space_pos + 1));
            } else {
                sub_op = trim(subs);
                sub_val = "";
            }

            // Dealing with SET instructions
            // assigns value to symbol
            if (sub_op == "SET")
            {
                if (is_number(sub_val)) {
                    //add to symbol table
                    symbol_table.push_back({label, loc, true, static_cast<int>(tonum(sub_val,10))});
                }
                else {
                    string error_message = "ERROR: SET operand is not a number at line " + to_string(line);
                    cout << error_message << endl;
                   errors += error_message + "\n"; // Append error message
                }
            }
            else
            {
                // Symbol without SET operation
                // symbol doesnt have any set value (-1)
                symbol_table.push_back({label, loc, false, -1});
            }
        }
        else
        {
            // If there’s nothing after the colon (Labels Without Instructions)
            PC--;    //  label doesn’t occupy memory(only label is stored)
            symbol_table.push_back({trim(instr.substr(0, colon_pos)), loc, false, -1});
        }
    }

    // Identify literals and constants
    else // If there’s no label
    // operation with operand
    {
        // Considering the second part of the instruction as the operand
        size_t space_pos = instr.find(" ");
        if (space_pos != string::npos && space_pos + 1 < instr.length()) {
            // operand is stored in subs
            string subs = trim(instr.substr(space_pos + 1));

            // Check for numeric literals
            if (is_number(subs))
            {
                literal_table.push_back({static_cast<int>(tonum(subs,10)), -1});
            }
        }
    }

    

    return errors;
    // errors+ warnings
}

//read assembly file and instructions,update symbol and literal tables,store,errors and warnings
// assign address to literals

void analyse(string &file,ofstream& logfile){
    //(file for input file  logfile for errors)


//temporary string for storing each line
string s;
// PC,tracks memory locations for each counter
int PC=0,line_cnt=1;

    // Read  input file
    ifstream MyFile(file);

    while (getline(MyFile, s)){
         
        
        string  instr = s.substr(0, s.find(";", 0));  // Remove comments(anything after; are comments)
        instr = trim(instr);
        //trim extra spaces

          // Skip empty lines
        if (instr.empty())
        {
            line_cnt+=1;
            continue;
        }
           // Log the output of instruction_to_table for the current instruction
           // write to logfile
        logfile << instruction_to_table(instr, PC, line_cnt++);

        // Increment PC after processing each instruction
        PC++;
    }

       // Assign addresses to literals with unassigned locations
    for (auto& lit : literal_table)
    {
        if (lit.address == -1)
        {
            lit.address = PC++;
        }
    }

    // Closing file after reaching end
    MyFile.close();

    

}

// Function to encode instructions into machine code
// also stores errors f mnemonics,operands,symbols and stores in string and log file
void instruction_to_code(string instr, int& PC, int line, string& encoding, string& errors, string& machine_code) {
    //line(for line num for detecting error),PC(current memory location for instructions)
    // encoding(used to hold machine code)
    
    int loc = PC;
    
    // Start the encoding with the current location in hex format
    encoding = int_to_hex(loc) + " ";
    string encoding_ = ""; // Temporary encoding for this instruction

    // Local vector to hold error messages
    string local_errors;

    // If the instruction contains a label (indicated by ':')
    size_t colon_pos = instr.find(':');
    if (colon_pos != string::npos) {
        // Find the label and check if there is an actual instruction after the colon
        if (colon_pos != instr.length() - 1) {
            // Extract the part of the instruction after the label
            string instruction_part = trim(instr.substr(colon_pos + 1));

            // Recursively call the function to process the instruction after the label
            string sub_machine_code;  // No need to collect errors here
            instruction_to_code(instruction_part, PC, line, encoding_, local_errors, sub_machine_code);
            
            // Append the generated encoding to the result (skip address part)
            if(encoding_.size()>9){
            encoding += encoding_.substr(9);}
             // Assuming address part is 9 characters
             else{
                encoding+=encoding_;
             }
                     errors += local_errors;
             machine_code+=sub_machine_code;
        } else
        
         {
         
            encoding += "         ";
            // 9 spaces(empty instrution)
            PC--;  // Decrement PC as the label itself doesn't consume memory
        }
        encoding += instr + "\n"; // Add the original instruction to encoding
    } 
    else {
        // For instructions without labels, process the operation and operand
        size_t space_pos = instr.find(" ");
        string operation, operand;

        if (space_pos != string::npos) {
            operation = instr.substr(0, space_pos);
            operand = instr.substr(space_pos + 1);
        } else {
            operation = instr;  // If no space is found, assume the whole string is the operation
        }

        // Trim operation and operand to remove any leading/trailing spaces
        operation = trim(operation);
        operand = trim(operand);

        // Check if the operation is defined in the Mnemonic Table (MOT)
        auto op_iter = mot.find(operation);
        if (op_iter == mot.end()) {
            string error_msg = "ERROR: Undefined mnemonic '" + operation + "' at line " + to_string(line);
             local_errors += error_msg + "\n";  // Add error to local string
            cout << error_msg << endl;
        }
        // Handle instructions that don't take operands
        else if (is_no_operand_operation(operation)) {
           // encoding += "000000" + op_iter->second + " ";  // Use operation's code from MOT
            // machine_code += "000000" + op_iter->second + " ";  // Same for machine code
            if (!operand.empty()) {  // Check for unexpected operand
                string error_msg = "ERROR: Operand not expected for '" + operation + "' at line " + to_string(line);
                local_errors += error_msg + "\n";  // Add error to local string
                cout << error_msg << endl;
            }else{
                // [padding]
                encoding += "000000" + op_iter->second + " ";
               machine_code += "000000" + op_iter->second + " "; 
            }
        }
        // Handle numeric operands
        else if (is_number(operand)) {
            int numeric_value = tonum(operand,10);
            string hex_operand = int_to_hex(numeric_value);

            // Special case for 'SET' or 'data' where operand is used directly
            if (operation == "SET" || operation == "data") {
                encoding += hex_operand + " ";
                machine_code += hex_operand + " ";
            } else {
                // // For other operations, ensure the operand fits as an address (6 hex digits)
                // if (hex_operand.length() >= 6) {
                //     encoding += hex_operand.substr(hex_operand.length() - 6) + op_iter->second + " ";
                //     machine_code += hex_operand.substr(hex_operand.length() - 6) + op_iter->second + " ";
                // } else {
                //     // Pad the operand with leading zeros if it's shorter than 6 digits
                //     encoding += string(6 - hex_operand.length(), '0') + hex_operand + op_iter->second + " ";
                //     machine_code += string(6 - hex_operand.length(), '0') + hex_operand + op_iter->second + " ";
                // }

                  encoding += hex_operand.substr(2,hex_operand.length() - 2) + op_iter->second + " ";
                     machine_code += hex_operand.substr(2,hex_operand.length() - 2) + op_iter->second + " ";


        }
        }
        // Handle symbol operands (variables)
      else {
    bool symbol_found = false;  // Flag to check if the operand matches a symbol
    string hex_value;

    // Loop through the symbol table using an iterator
    for (auto it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        const auto& symbol_entry = *it;  // Dereference the iterator to get the symbol entry

        if (symbol_entry.name == operand) {
            symbol_found = true;

            // Handle SET variables
            if (symbol_entry.set) {
                hex_value = int_to_hex(symbol_entry.set_value);
            }
            // Handle branching operations (offset calculation)
            else if (is_branching_operation(operation)) {
                hex_value = int_to_hex(symbol_entry.address - loc - 1);
            }
            // Default case: Use the symbol's address
            else {
                hex_value = int_to_hex(symbol_entry.address);
            }

            // Ensure hex value is 6 digits, pad if necessary
            if (hex_value.length() >= 6) {
                encoding += hex_value.substr(hex_value.length() - 6) + op_iter->second + " ";
                machine_code += hex_value.substr(hex_value.length() - 6) + op_iter->second + " ";
            } else {
                encoding += string(6 - hex_value.length(), '0') + hex_value + op_iter->second + " ";
                machine_code += string(6 - hex_value.length(), '0') + hex_value + op_iter->second + " ";
            }
            break;  // Break after the symbol is found
        }
    }

    // If the symbol was not found, log an error
    if (!symbol_found) {
        string error_msg = "ERROR: Unknown Symbol '" + operand + "' as operand at line " + to_string(line);
        local_errors += error_msg + "\n";  // Add error to local string
        cout << error_msg << endl;
    }
}

    }

    encoding += instr + "\n";  // Add the original instruction to encoding

  // After processing, append the accumulated errors to the global errors string
    errors += local_errors;
}




void synthesize(string &file, ofstream& outfile, ofstream& logfile, ofstream& objfile)
{
    // file(input),outfile(.L),logfile(.log),objfile(.o)
    // Temporary string to read each line
    string line;

    // Program Counter, Line counter
    int PC = 0, line_cnt = 1;

    // Read the input file
    ifstream MyFile(file);

    // Vector to store errors
    string errors;

    while (getline(MyFile, line)) 
    {
        // trim extra spaces
        string instr = line.substr(0, line.find(";", 0));
        instr = trim(instr);

        // Skip empty lines
        if (instr.empty()) 
        {
            line_cnt++;
            continue;
        }

        // Write Encoded instruction into listing file
        string encoding, machine_code;
        
        // Call inst_to_code with references to encoding, errors , and machine_code
        instruction_to_code(instr, PC, line_cnt++, encoding, errors, machine_code);
        
        // Write encoded instruction to listing file
        outfile << encoding;
        
        // If machine code is available, write to object file
        if (!machine_code.empty()) 
        {
            // Convert machine code hex string to an integer
            uint32_t temp;
            stringstream ss;
            ss << hex << trim(machine_code);
            ss >> temp;
            // write binary machine code to obj file
            objfile.write(reinterpret_cast<char*>(&temp), sizeof(temp));
        }

        // Write errors to log file
        for (const auto& error : errors) 
        {
            logfile << error;
        }

        // Increment the location counter (Program Counter)
        PC++;
    }
}

int main(){

    
// initializing machine operation table
    init();
  
    // Specify the input file name directly
string in_file;
cout << "Enter the name of the assembly file: ";
    cin >> in_file;

    // Extracting  base name by finding the position of the first dot (.) 
size_t dot_pos=in_file.find('.');
string base_name;
if(dot_pos!=string::npos){
    base_name=in_file.substr(0,dot_pos);
}else{
    base_name=in_file;
}

    // list file(assembly code + addresses+machine code)
    string out_file=base_name+".L";

    // log file(wrnings+errors+other actions)
    string log_file=base_name+".log";

    // obj file(generated machine code)
    string obj_file=base_name+".o";
       // Open the output file (listing file) for writing
    ofstream outfile(out_file);
    if (!outfile) {
    cout<<"Error:Could not open output file"<<out_file<<endl;
        return 1;
    }
     // Open the log file for writing
    ofstream logfile(log_file);
    if (!logfile) {
    cout<<"Error:Could not open log file"<<log_file<<endl;
        return 1;
    }
      // Open the object file for writing in binary mode
    ofstream objfile(obj_file,ios::out|ios::binary);
    if (!objfile) {
    cout<<"Error:Could not open object file"<<obj_file<<endl;
        return 1;
    }
       // Performing Pass-1 of assembly:The analysis phase
    analyse(in_file, logfile);
   // Perform Pass-2 of assembly:The synthesis phase
    synthesize(in_file, outfile, logfile, objfile);
    // Close files
    outfile.close();
    logfile.close();
    objfile.close();

    return 0;
}























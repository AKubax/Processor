#include "Processor.h"
#include <stdexcept>

const int MAX_CMD_LEN = 512;

enum REG_CODE{
    REG_TRASH = 0, REG_RAX, REG_RBX, REG_RCX, REG_RDX
};

class SyntaxError : public std::runtime_error{
public:
    SyntaxError(const char* what):
        std::runtime_error (what)
    {}
};

class Compiler{
private:
    FILE* file = 0;
    FILE* outFile = 0;

    void deReg();

public:
    Compiler(const char* fn, const char* ofn = "proc_input.akc"):
        file (fopen(fn, "r")),
        outFile (fopen(ofn, "w"))
    {}

    ~Compiler(){
        fclose(file);
        fclose(outFile);
    }

    void compile();
};

void Compiler::deReg(){
    char arg_c[MAX_CMD_LEN] = "";
    if(!fscanf(file, "%s", &arg_c)) throw SyntaxError("Wrong argument");


    if(strcmp(arg_c, "RAX") == 0){
        fprintf(outFile, "%d ", REG_RAX);
    }

    else if(strcmp(arg_c, "RBX") == 0){
        fprintf(outFile, "%d ", REG_RBX);
    }

    else if(strcmp(arg_c, "RCX") == 0){
        fprintf(outFile, "%d ", REG_RCX);
    }

    else if(strcmp(arg_c, "RDX") == 0){
        fprintf(outFile, "%d ", REG_RDX);
    }

    else{
        throw SyntaxError("Wrong argument");
    }
}


void Compiler::compile(){
    while(!feof(file)){
//        printf("Before\n");
        char cmd[MAX_CMD_LEN] = "";
        if(!fscanf(file, "%s", cmd)) throw SyntaxError("Wrong command");

//        printf("Got cmd = \"%s\"\n", cmd);

        fprintf(outFile, "\n");

        if(strcmp(cmd, "") == 0){
            continue;
        }

        #define DEF_CMD(name, num, code, proc_code)              \
        else if(strcmp(cmd, #name) == 0){             \
            fprintf(outFile, "%d ", PROC_ ## name);   \
            code                                      \
        }

        #include "Commands.h"
        #undef DEF_CMD

    }
}

int main(){
    Compiler("comp_input.akc").compile();
    parse();
}

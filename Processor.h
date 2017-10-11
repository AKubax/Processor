#include "../TXLib.h"

#include "../AKStack/AKStack.h"

#define DEF_CMD(name, num) name = num,
enum CODES{
    #include "Commands.h"
    NUM_CMD
};
#undef DEF_CMD

const int Number_Registers = 5;

class Processor
{
private:
    AKStack<int> stk;

    void FUNC_ADD();

    void FUNC_SUB();

    void FUNC_MUL();

    void FUNC_DIV();

    void FUNC_PUSH(int var);

    void FUNC_POP();

    void FUNC_OUT();



    int REG[Number_Registers];
public:
    Processor();

    void execute(AKVector<int>& vec);

    void dump();
    bool ok();
};

Processor::Processor():
    stk (),
    REG ({})
{}

void Processor::FUNC_ADD(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();
    int second = stk.top();
    stk.pop();
    stk.push (first + second);

    return i;
}

void Processor::FUNC_SUB(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();
    int second = stk.top();
    stk.pop();
    stk.push (first - second);

    return i;
}

void Processor::FUNC_MUL(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();
    int second = stk.top();
    stk.pop();
    stk.push (first * second);

    return i;
}

void Processor::FUNC_DIV(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();
    int second = stk.top();
    stk.pop();
    stk.push (first / second);

    return i;
}

void Processor::FUNC_PUSH(int arg1, int arg2, int i){
    if(arg1){
        stk.push(REG[arg2]);
    }
    else stk.push (arg2);

    return i;
}

void Processor::FUNC_POP(int arg1, int arg2, int i){
    REG[arg2] = stk.top();
    stk.pop();

    return i;
}

void Processor::FUNC_OUT(int arg1, int arg2, int i){
    printf ("%d\n", stk.top());
    stk.pop();

    return i;
}

void Processor::FUNC_JMP(int arg1, int arg2, int i){
    i = arg2;
    return i;
}

void Processor::FUNC_JE(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();

    int second = stk.top();
    stk.pop();

    if(first == second) i = arg2;
    else printf("%d != %d\n", first, second);

    return i;
}

void Processor::FUNC_JNE(int arg1, int arg2, int i){
    int first = stk.top();
    stk.pop();

    int second = stk.top();
    stk.pop();

    if(first != second) i = arg2;
    else printf("%d == %d\n", first, second);

    return i;
}


void Processor::execute(AKVector<int>& vec){
    for(int i = 0; i < vec.size();){
        int cmd  = vec[i++];
        int arg1 = vec[i++];
        int arg2 = vec[i++];

//        printf("i = %3d; cmd == %3d; arg1 == %3d; arg2 == %3d; RAX = %3d; RBX = %3d; RCX = %3d; RDX = %3d\n", i - 3, cmd, arg1, arg2, REG[1], REG[2], REG[3], REG[4]);

        switch (cmd){
            case EXIT:
                return;
                break;

            case ADD:
                i = FUNC_ADD(arg1, arg2, i);
                break;

            case SUB:
                i = FUNC_SUB(arg1, arg2, i);
                break;

            case MUL:
                i = FUNC_MUL(arg1, arg2, i);
                break;

            case DIV:
                i = FUNC_DIV(arg1, arg2, i);
                break;

            case PUSH:
                i = FUNC_PUSH(arg1, arg2, i);
                break;

            case POP:
                i = FUNC_POP(arg1, arg2, i);
                break;

            case OUT:
                i = FUNC_OUT(arg1, arg2, i)
                break;

            case JMP:

                break;

            case JE:{

                break;
            }

            case JNE:{

                break;
            }
        }
    }
}

void Processor::dump(){
    printf("Processor dump\n");
    stk.dump();

    for(int i = 0; i < Number_Registers; i++){
        printf("REG[%d] = %d", i, REG[i]);
    }
}

bool Processor::ok(){
    return stk.ok();
}

int parse(){
    try{
        Processor proc;
        const char* fname = "";
        AKVector<int> vec;

        fname = "proc_input.akc";

        FILE* file = fopen(fname, "r");

        while(!feof(file)){
            int value = 0;
            fscanf(file, "%d", &value);
//            printf("value = %d\n", value);
            vec.push_back(value);
        }

        proc.execute(vec);

        fclose(file);
    }
    catch(StackUnderflow){
        printf("\n\nUnderflow of processor memory");
        throw;
        return 1;
    }
    catch(std::exception){
        printf("\n\nAn exception caught");
        throw;
        return 1;
    }
}

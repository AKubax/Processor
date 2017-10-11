#include "../TXLib.h"

#include "../AKStack/AKStack.h"

#define DEF_CMD(name, num, code, proc_code) PROC_ ## name = num,
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

    #define DEF_CMD(name, num, code, proc_code) int FUNC_ ## name (int arg1, int arg2, int i);

    #include "Commands.h"

    #undef  DEF_CMD


    int REG[Number_Registers];
public:
    Processor();

    int execute(AKVector<int>& vec);

    void dump();
    bool ok();
};

Processor::Processor():
    stk (),
    REG ({})
{}

#define DEF_CMD(name, num, code, proc_code) int Processor::FUNC_ ## name(int arg1, int arg2, int i){ {proc_code}; return i; }

#include "Commands.h"

#undef DEF_CMD



//int Processor::FUNC_EXIT(int arg1, int arg2, int i){
//    return -1;
//}
//
//int Processor::FUNC_ADD(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//    int second = stk.top();
//    stk.pop();
//    stk.push (first + second);
//
//    return i;
//}
//
//int Processor::FUNC_SUB(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//    int second = stk.top();
//    stk.pop();
//    stk.push (first - second);
//
//    return i;
//}
//
//int Processor::FUNC_MUL(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//    int second = stk.top();
//    stk.pop();
//    stk.push (first * second);
//
//    return i;
//}
//
//int Processor::FUNC_DIV(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//    int second = stk.top();
//    stk.pop();
//    stk.push (first / second);
//
//    return i;
//}
//
//int Processor::FUNC_PUSH(int arg1, int arg2, int i){
//
//
//    return i;
//}
//
//int Processor::FUNC_POP(int arg1, int arg2, int i){
//    REG[arg2] = stk.top();
//    stk.pop();
//
//    return i;
//}
//
//int Processor::FUNC_OUT(int arg1, int arg2, int i){
//    printf ("%d\n", stk.top());
//    stk.pop();
//
//    return i;
//}
//
//int Processor::FUNC_JMP(int arg1, int arg2, int i){
//    i = arg2;
//    return i;
//}
//
//int Processor::FUNC_JE(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//
//    int second = stk.top();
//    stk.pop();
//
//    if(first == second) i = arg2;
//    else printf("%d != %d\n", first, second);
//
//    return i;
//}
//
//int Processor::FUNC_JNE(int arg1, int arg2, int i){
//    int first = stk.top();
//    stk.pop();
//
//    int second = stk.top();
//    stk.pop();
//
//    if(first != second) i = arg2;
//    else printf("%d == %d\n", first, second);
//
//    return i;
//}


int Processor::execute(AKVector<int>& vec){
    for(int i = 0; i < vec.size();){
        int cmd  = vec[i++];
        int arg1 = vec[i++];
        int arg2 = vec[i++];

//        printf("i = %3d; cmd == %3d; arg1 == %3d; arg2 == %3d; RAX = %3d; RBX = %3d; RCX = %3d; RDX = %3d\n", i - 3, cmd, arg1, arg2, REG[1], REG[2], REG[3], REG[4]);

        #define DEF_CMD(name, num, code, proc_code)    case PROC_ ## name:                          \
                                                i = FUNC_##name(arg1, arg2, i); \
                                                break;                          \


        switch (cmd){
//            case EXIT:
//                i = FUNC_EXIT(arg1, arg2, i);
//                break;
//
//            case ADD:
//                i = FUNC_ADD(arg1, arg2, i);
//                break;
//
//            case SUB:
//                i = FUNC_SUB(arg1, arg2, i);
//                break;
//
//            case MUL:
//                i = FUNC_MUL(arg1, arg2, i);
//                break;
//
//            case DIV:
//                i = FUNC_DIV(arg1, arg2, i);
//                break;
//
//            case PUSH:
//                i = FUNC_PUSH(arg1, arg2, i);
//                break;
//
//            case POP:
//                i = FUNC_POP(arg1, arg2, i);
//                break;
//
//            case OUT:
//                i = FUNC_OUT(arg1, arg2, i)
//                break;
//
//            case JMP:
//
//                break;
//
//            case JE:{
//
//                break;
//            }
//
//            case JNE:{
//
//                break;
//            }

            #include "Commands.h"

            #undef DEF_CMD
        }

        if(i == -1){
            return 0;
        }
    }

    return 0;
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

    return 0;
}

#include <TXLib.h>
#include "../AKStack/AKStack.h"


class Processor{
public:
    AKStack<double> mainStack;

//public:
    #define DEF_CMD(name, num, proc_capacity, comp_code, proc_code)     char* FUNC_ ## num (char* instr_ptr){      \
                                                                            {proc_code}                            \
                                                                            return instr_ptr;                      \
                                                                        }                                          \

    #include "Commands.h"
    #undef DEF_CMD

};


#define EMIT_BYTE(var) compOut.push_back(var);
#define EMIT_DBL(var)  compOut.resize(compOut.size() + sizeof(double)); *((double*) (compOut.data() + compOut.size() - sizeof(double))) = var;


#define PROC_MATH_FUNC(sign)    double first = mainStack.top();                      \
                                mainStack.pop();                                     \
                                                                                     \
                                double second = mainStack.top();                     \
                                mainStack.pop();                                     \
                                                                                     \
                                mainStack.push(first sign second);                   \
                                                                                     \
                                instr_ptr++;                                         \


#define COMP_GETLABEL_FUNC()    Token& elem = inFile[++i];                                                                           \
                                                                                                                                        \
                                if(!elem.isString())            throw std::runtime_error("Wrong syntax after jump cmd");                \
                                if(elem.str_token[0] != ':')    throw std::runtime_error("Statement without ':' after jump cmd");       \
                                                                                                                                        \
                                                                                                                                        \
                                                                                                                                        \
                                printf("size of compOut before resize() == %u\n", compOut.size());                                     \
                                                                                                                                        \
                                compOut.resize (compOut.size() + sizeof(unsigned short));                                               \
                                                                                                                                        \
                                printf("size of compOut after resize() == %lu; [size - 2] == %02X; [size - 1] == %02X\nelem.str_token = \"%s\"; labels[elem.str_token] == %d", \
                                        compOut.size(), compOut[compOut.size() - 2], compOut[compOut.size() - 1], elem.str_token.c_str(), labels[elem.str_token]);\
                                                                                                                                        \
                                *((unsigned short*) (compOut.data() + compOut.size() - sizeof(short))) = labels[elem.str_token];                                            \
                                                                                                                                          \
                                printf("after insert: [size - 2] == %02X; [size - 1] == %02X\n", compOut[compOut.size() - 2], compOut[compOut.size() - 1]); \
                                /**((unsigned short*) (compOut.data() + compOut.size())) = labels[elem.str_token];  */                      \



#define PUSH_COMP_FUNC()        Token& elem = inFile[++i];                                                                                              \
                                                                                                                                                          \
                                if(elem.isString()){                                                                                                      \
                                    compOut.push_back(1);                                                                                                 \
                                                                                                                                                          \
                                    double regNum = 0;                                                                                                    \
                                                                                                                                                          \
                                    if      (elem.str_token == "DEL") throw std::runtime_error("The PUSH operation from DEL register is prohibited");     \
                                                                                                                                                          \
                                    else if (elem.str_token == "RAX") regNum = 1;                                                                         \
                                    else if (elem.str_token == "RBX") regNum = 2;                                                                         \
                                    else if (elem.str_token == "RCX") regNum = 3;                                                                         \
                                    else if (elem.str_token == "RDX") regNum = 4;                                                                         \
                                                                                                                                                          \
                                    compOut.resize(compOut.size() + sizeof(double));                                                                      \
                                    *((double*) (compOut.data() + compOut.size() - sizeof(double))) = regNum;                                             \
                                }                                                                                                                         \
                                                                                                                                                          \
                                else{                                                                                                                     \
                                    compOut.push_back(0);                                                                                                 \
                                                                                                                                                          \
                                    printf("Pushing dbl: %lf\n", elem.dbl_token);                                                                          \
                                                                                                                                                          \
                                    compOut.resize(compOut.size() + sizeof(double));                                                                      \
                                    *((double*) (compOut.data() + compOut.size() - sizeof(double))) = elem.dbl_token;                                     \
                                }                                                                                                                         \



//DEF_CMD (EXIT, 0, 1,    { printf("Executing EXIT compilator func\n");  }, { i = -1; })

//Math funcs:

DEF_CMD (ADD,  1, 1,   {  }, { PROC_MATH_FUNC(+); } )

DEF_CMD (SUB,  2, 1,   {  }, { PROC_MATH_FUNC(-); } )

DEF_CMD (MUL,  3, 1,   {  }, { PROC_MATH_FUNC(*); } )

DEF_CMD (DIV,  4, 1,   {  }, { PROC_MATH_FUNC(/); } )

//DEF_CMD (PUSH, 5, 1 + 1 + sizeof(double),    { PUSH_COMP_FUNC() },
//{
//    if(arg1){ stk.push(REG[(int) arg2]); }
//    else      stk.push    (arg2);
//})
//
//DEF_CMD (POP,  6, 2,    { compOut.push_back((unsigned char) inFile[++i].dbl_token); },
//{
//    instr_ptr++;
//
//    REG[*instr_ptr] = stk.top();
//    stk.pop();
//
//    instr_ptr++;
//})
//
//
//DEF_CMD (OUT,  7, 1,   {  },
//{
//    cout << stk.top() << std::endl;
//    stk.pop();
//
//    instr_ptr++;
//})
//
//
//
//DEF_CMD (JMP,  8, 3,    {COMP_GETLABEL_FUNC()}, { i = arg2; } )
//
//DEF_CMD (JE,   9, 3,   {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(==); } )
//
//DEF_CMD (JNE,  10, 3,   {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(!=); } )
//
//DEF_CMD (JA,  11, 3,  {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(>); } )
//
//DEF_CMD (JEA,  12, 3,   {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(>=); } )
//
//DEF_CMD (JB,  13, 3,  {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(<); } )
//
//DEF_CMD (JEB,  14, 3,  {COMP_GETLABEL_FUNC()}, { PROC_JUMP_FUNC(<=); } )
//
//
//DEF_CMD (CALL, 15, 3,   {COMP_GETLABEL_FUNC()},   {ret.push(i);/* printf("The return point is %d\n", i);*/ i = arg2; /*printf("i == %d; arg2 == %d\n", i, arg2);*/})
//
//DEF_CMD (RET, 16, 1, {},  {i = ret.top(); ret.pop();})
//
//DEF_CMD (MEOW, 17, 1, {}, { $sy printf("MEOW \a\n"); })
//
//DEF_CMD (DUP, 18, 1, {}, { stk.push(stk.top()); })
//
//DEF_CMD (INP, 19, 1, {}, { int var = 0; cin >> var; stk.push(var); })
//
//DEF_CMD (SUPERCOMMAND38, 38, 1, {}, { for(int k = 0; k < 38; k++) printf("BUTERBROD!!! ");  printf("\n"); })
//
//DEF_CMD (SUPERCOMMAND3802, 3802, 1, {}, { for(int k = 0; k < 3802; k++) printf("BUTERBROD!!! ");  printf("\n"); })
//
//DEF_CMD (SQRT, 20, 1, {}, { prdata_t first = stk.top(); stk.pop(); stk.push(sqrt(first));})
//
//DEF_CMD (NOP, 0xEE, 1, {}, { })
//
//
//DEF_CMD (db, 21,              1,    { compOut.pop_back(); EMIT_BYTE(inFile[++i].dbl_token); }, { })
//
//
//DEF_CMD (dq, 22, sizeof(double),    { compOut.pop_back(); EMIT_DBL (inFile[++i].dbl_token); }, { })


/*


    db 5 # PUSH 5
    dq 5

    db 21 # SIN

    db 18 # DUP
    db 3  # MUL

    db 5  # PUSH 5
    dq 5

    db 22 # COS

    db 18 # DUP
    db 3  # MUL

    db 1  # ADD

    db 7  # OUT


....
:x var 7

....





*/

#define EMIT(value) fprintf(outFile, "%d ", value)
#define GET(var)    fscanf(file, "%d", &var)

#define EMIT_ZERO   EMIT(0)
#define EMIT_ONE    EMIT(1)
#define EMIT_INPUT  {int arg = 0; GET(arg); EMIT(arg);}

#define PROC_MATH_FUNC(sign)    int first = stk.top();          \
                                stk.pop();                      \
                                int second = stk.top();         \
                                stk.pop();                      \
                                stk.push (first sign second);   \

#define PROC_JUMP_FUNC(sign)    int first = stk.top();                           \
                                stk.pop();                                       \
                                int second = stk.top();                          \
                                stk.pop();                                       \
                                if(first sign second) i = arg2;                  \
                                else printf("%d != %d\n", first, second);        \


DEF_CMD (EXIT, 0,    { EMIT_ZERO; EMIT_ZERO; }, { i = -1; })

DEF_CMD (ADD,  1,    { EMIT_ZERO; EMIT_ZERO; }, { PROC_MATH_FUNC(+); } )

DEF_CMD (SUB,  2,    { EMIT_ZERO; EMIT_ZERO; }, { PROC_MATH_FUNC(-); } )

DEF_CMD (MUL,  3,    { EMIT_ZERO; EMIT_ZERO; }, { PROC_MATH_FUNC(*); } )

DEF_CMD (DIV,  4,    { EMIT_ZERO; EMIT_ZERO; }, { PROC_MATH_FUNC(/); } )

DEF_CMD (PUSH, 5,    { int arg_i = 0; if(GET(arg_i)) { EMIT_ZERO; EMIT(arg_i); }  else { EMIT_ONE; deReg(); } },
{
    if(arg1){ stk.push(REG[arg2]); }
    else      stk.push    (arg2);
})

DEF_CMD (POP,  6,    { EMIT_ONE;  deReg(); },
{
    REG[arg2] = stk.top();
    stk.pop();
})

DEF_CMD (OUT,  7,    { EMIT_ZERO; EMIT_ZERO; },
{
    printf ("%d\n", stk.top());
    stk.pop();
})

DEF_CMD (JMP,  8,    { EMIT_ZERO; EMIT_INPUT; }, { PROC_JUMP_FUNC(|| 1 ||); } )

DEF_CMD (JE,   9,    { EMIT_ZERO; EMIT_INPUT; }, { PROC_JUMP_FUNC(==); } )

DEF_CMD (JNE,  10,   { EMIT_ZERO; EMIT_INPUT; }, { PROC_JUMP_FUNC(!=); } )

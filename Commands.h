#define EMIT(value) fprintf(outFile, "%d ", value)
#define GET(var)    fscanf(file, "%d", &var)

#define EMIT_ZERO   EMIT(0)
#define EMIT_ONE    EMIT(1)
#define EMIT_INPUT  {int arg = 0; GET(arg); EMIT(arg);}



DEF_CMD (EXIT, 0,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (ADD,  1,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (SUB,  2,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (MUL,  3,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (DIV,  4,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (PUSH, 5,   {   int arg_i = 0; if(GET(arg_i)) { EMIT_ZERO; EMIT(arg_i); }  else { EMIT_ONE; deReg(); }   })

DEF_CMD (POP,  6,    EMIT_ONE;  deReg();  )

DEF_CMD (OUT,  7,    EMIT_ZERO; EMIT_ZERO;)

DEF_CMD (JMP,  8,    EMIT_ZERO; EMIT_INPUT;)

DEF_CMD (JE,   9,    EMIT_ZERO; EMIT_INPUT;)

DEF_CMD (JNE,  10,   EMIT_ZERO; EMIT_INPUT;)

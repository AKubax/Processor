#include <TXLib.h>

#include <stdio.h>
#include <map>
#include <stdexcept>
#include <vector>
#include <string>
#include <string.h>
#include <sys/stat.h>

//namespace std{
//    std::string to_string(int num);
//    std::string to_string(int num){
//        char str[64] = "";
//        sprintf(&(str[0]), "%d", num);
//        return std::string(str);
//    }
//}

#define $lp {printf("Line: %d; File: \"%s\"\n", __LINE__, __FILE__);}


#define DEF_CMD(name, num, proc_capacity, comp_code, proc_code) CMD_ ## name,

enum CMDS{
    #include "Commands.h"
    NUM_CMDS

    #undef DEF_CMD
};


struct Token{
    enum Type{
        TT_NOT_INITIALIZED, TT_DOUBLE, TT_STRING
    };
    Type type;

    std::string     str_token;
    const double    dbl_token;

    int lineNumber = 0;


    Token():
        type (TT_NOT_INITIALIZED),
        str_token (""),
        dbl_token (0)
    {}
    Token(const char* str_t, int ln):
        type (TT_STRING),
        str_token (str_t),
        dbl_token (0),
        lineNumber (ln)
    {}
    Token(const double dbl_t, int ln):
        type (TT_DOUBLE),
        str_token (""),
        dbl_token (dbl_t),
        lineNumber (ln)
    {}

    bool isString() const{
        return type == TT_STRING;
    }

    void dump() const{
        switch(type){
            case TT_STRING:
                printf("\nCurrent type = \"std::string\"; var = \"%s\"\n", str_token.c_str());
                break;

            case TT_DOUBLE:
                printf("\nCurrent type = \"double\"; var = %.20lf\n", dbl_token);
                break;

            case TT_NOT_INITIALIZED:
                printf("\nCurrent type is not initialized\n");

            default:
                throw std::runtime_error("What is that? Default case worked in dump()\n");
        }
    }
};




class Tokenizator{
private:
    int lineNumber;

    std::vector<Token> res;
public:

    char* skipSpaces(char* str){
//        printf("Begin of func skipSpaces()\n");

        bool isComment = false;
        while(isspace(str[0]) || isComment){
            if(str[0] == '\n') lineNumber++;

            str++;

//            printf("str[0] == '%c'; str[1] == '%c'; isComment == %s \n", str[0], str[1], isComment? "true" : "false");

            if      (str[0] == '/' && str[1] == '*'){
                isComment = true;
//                printf("isComment on\n");
            }
            else if (str[0] == '*' && str[1] == '/'){
                isComment = false;
//                printf("isComment off\n");
                str += 2;
            }

//            printf("isComment after if block == %s\n\n", isComment? "true" : "false");
        }
        return str;
    }

    Tokenizator(char* str):
        lineNumber (1),
        res        ()
    {
        int curLen = 0;

        const char* str0 = str;

        while(1){

            printf("str[begin: 0x%p; end: 0x%p] == \"%s\"\n", str, str + strlen(str), str);

            str = skipSpaces(str);
            printf("Parsing. \n");

            char c[128] = {};
            double    d =  0;

            int scanfRes = sscanf(str, "%lf%n", &d, &curLen);

            printf("scanfRes == %d; d[0x%p] == %lf; c == \"%s\"\n", scanfRes, &d, d, c);

            if      (scanfRes == EOF){
                printf("Got EOF. Ending parse\n");
                break;
            }

            else if (scanfRes != 0){
                printf("Got dbl_token = %lf\n", d);

                res.push_back(Token(d, lineNumber));
                str += curLen;
            }
            else if (sscanf(str, "%s%n", c, &curLen) != 0){
                printf("Started string if block. curLen = %d; str - str0 == %d\n", curLen, str - str0);

                if((str + curLen)[0] != '\0'){                        $lp
                    (str + curLen)[0] = '\0';                         $lp
                }

                res.push_back(Token(str, lineNumber));                $lp

                str += curLen;                                        $lp

                str += 1;                                             $lp

                res.back().dump();                                    $lp
            }

        }

    }

    std::vector<Token> getRes(){
        return res;
    }
};


class Compiler{
public:
    std::vector<unsigned char> compOut;

    std::map<std::string, unsigned short> labels;

    struct Header{
        unsigned short signature = 'ka';

        unsigned short ver = 1;

        unsigned int sz = 0;
    };

//public:

    Compiler():
        compOut (),
        labels  ()
    {

    }

    void compile(std::vector<Token> inFile){
        unsigned short curLabelPoint = 0;

        for(unsigned short i = 0; i < inFile.size(); i++){
            printf("i = %u; inFile.size() = %u; inFile[i].str_token = \"%s\"\n", i, inFile.size(), inFile[i].str_token.c_str());

            if(!(inFile[i].isString())){
                throw std::runtime_error(std::string() + "Wrong statement \"" + std::to_string(inFile[i].dbl_token)  +  "\" instead of command (Line: " + std::to_string((int) inFile[i].lineNumber) + ")");
            }

            if(inFile[i].str_token[0] == ':'){
                printf("Got label: '%s', curLabelPoint = %d\n", inFile[i].str_token.c_str(), curLabelPoint);

                labels[inFile[i].str_token] = curLabelPoint;

                dump();
                continue;
            }

            printf("compOut = [ ");
            for(auto& value : compOut){
                printf("%d, ", value);
            }
            printf("]\n");

            #define DEF_CMD(name, num, proc_capacity, comp_code, proc_code) \
                else if(inFile[i].str_token == #name) { printf("Got cmd = \"%s\"\n", #name); compOut.push_back((unsigned char) num); {comp_code}; curLabelPoint += proc_capacity; }

            if(0);
            #include "Commands.h"

            #undef DEF_CMD

            else{
                char msg[64] = "";
                sprintf(&(msg[0]), "Command \"%s\" does not exist (Line: %d)", inFile[i].str_token.c_str(), inFile[i].lineNumber);

                throw std::runtime_error(msg);
            }

            dump();
        }
    }

    void dump(){
        $sg

        printf("--BEGIN OF DUMP--\nlabels{\n");
        for(const auto& i : labels){
            printf("\t'%s' : %d\n", i.first.c_str(), i.second);
        }
        printf("}\n\n");

        int ctr = 1;

        $y
        printf("00 01 02 03 04 05 06 07\n");
        $g

        for(const auto& elem : compOut){
            printf("%02x ", elem);

            if(++ctr % 2 == 0) printf("| ");
            if(ctr   % 8 == 0) printf("\n");
        }
        printf("\n--END OF DUMP--\n");
    }


    void fileOut(const char* fname){
        FILE* file = fopen(fname, "wb");

        Header h;
        h.sz = compOut.size();
        fwrite(&h, sizeof(h), sizeof(char), file);

        fwrite(compOut.data(), compOut.size(), sizeof(compOut.data()[0]), file);

        fclose(file);
    }
};


void compile(const char* filename, const char* outFile);
void compile(const char* filename, const char* outFile){
    FILE* inpFile = fopen(filename, "r");

    if(inpFile == NULL) throw std::runtime_error("The file to compile does not exist");

    struct stat st = {};
    stat(filename, &st);

    char* inp = (char*) calloc(st.st_size, sizeof(char));
    fread(inp, st.st_size, sizeof(char), inpFile);

    fclose(inpFile);

    Compiler comp;
    comp.compile(Tokenizator(inp).getRes());

    comp.fileOut(outFile);
}


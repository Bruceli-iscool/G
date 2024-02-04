#include <iostream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>


// split code into tokens
enum Token {
    tok_eof = -1,

    tok_def = -2,
    tok_extern = -3,
    tok_identifier = -4,
    tok_number = -5,
};

static std::string indentifierStr;
static double NumVal;
// procces tokens
static int getok() {
    static int lastChar = ' ';
    // skip spaces
    while (isspace(lastChar))
      lastChar = getchar();
    // reconize keywords
    if(isalpha(lastChar)) {
        indentifierStr = lastChar;
        while (isalnum((lastChar = getchar())))
          indentifierStr += lastChar;
        if (indentifierStr == "function")
           return tok_def;
        if (indentifierStr == "global")
           return tok_extern;
        return tok_identifier;
    }
    // numbers
    if (isdigit(lastChar) || lastChar == '.') {
        std::string numstr;
        do {
            numstr += lastChar;
            lastChar = getchar();
        } while (isdigit(lastChar) || lastChar == '.');
        NumVal = strtod(numstr.c_str(), 0);
        return tok_number;
    }
    // comments handling
    if (lastChar == '/') {
        do
         lastChar = getchar();
        while (lastChar != EOF && lastChar != '\n')
    }
}
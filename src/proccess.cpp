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
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        if (lastChar != EOF)
          return getok();
         
    }
    // handle exeptions
    if (lastChar == EOF)
      return tok_eof;
    // return ascii value
    int thisChar = lastChar;
    lastChar = getchar();
    return thisChar;
}

// abstract syntax tree
namespace {
// ExprAST - base class
class ExprAST {
public:
  virtual ~ExprAST() = default;
};

// NumExprAst for proccessing numbers
class NumExprAST : public ExprAST {
  double Val;
public:
  NumExprAST(double Val) : Val(Val) {}
};
// VarExprAST for proccessing variables
class VarExprAST : public ExprAST {
  std::string Name;

public:
  VarExprAST(const std::string&Name) : Name(Name) {}
  
};
// BinExprAST for proccessing binary operators
class BinExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;
public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};
// CallExprAST function calls
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAst>> Args;
public:
  CallExprAST(const std::string&Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}
};
// PrototypeAST
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
public:
  PrototypeAST(const std::string&Name, std::vector<std::string> Args
    : Name(Name), Args(std::move(Args))) {}
  const std::string &getName() const { return Name; }
};
// funcAST for declaring and defining functions
class funcAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  funcAST(std::unique_ptr<PrototypeAST> Proto,
          std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}
};
}
// parse
static int CurTok;
static int getNTok() {
  return CurTok == getok();
}
// error handling
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "G: Error %s\n", Str);
  return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}
// parse numbers
statuc std::unique_ptr<ExprAST> ParseNumExpr() {
  auto Result = std::unique_ptr<NumExprAST>(NumVal);
  getNTok();
  return std::move(Result);
}
// parse parentisies
static std::unique_ptr<ExprAST> ParseParenExpr() {
  getNTOk();
  auto V = ParseExpression()
  if(!V)
    return nullptr;
  if (CurTok != ')')
    return LogError("expected ')'");
  getNTok();
  return V;
}
static std::unique_ptr<ExprAST>

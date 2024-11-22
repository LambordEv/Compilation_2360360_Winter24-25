%{
/*Declaration Section*/
/*------- Include Section -------*/
#include <stdio.h>
#include "tokens.hpp"
#include "output.hpp"
%}

%option yylineno
%option noyywrap

whitespace                              ([\t\r\n ])
decimalDigit                            ([0-9])
hexDigit                                ([0-9a-fA-F])
letter                                  ([a-zA-Z])

voidToken                               (void)
intToken                                (int)
byteToken                               (byte)
boolToken                               (bool)
andToken                                (and)
orToken                                 (or)
notToken                                (not)
trueToken                               (true)
falseToken                              (false)
returnToken                             (return)
ifToken                                 (if)
elseToken                               (else)
whileToken                              (while)
breakToken                              (break)
continueToken                           (continue)
semicolonToken                          (;)
commaToken                              (,)
lParenToken                             (\()
rParenToken                             (\))
lBraceToken                             (\{)
rBraceToken                             (\})
assignToken                             (\=)

relopSign                               (==|!=|<|>|<=|>=)
binopSign                               (\+|\-|\*|\/)

commentLexema                           (\/\/.*)

idLexema                                ({letter}+{decimalDigit}*{letter}*)

numLexema                               ([1-9]+{decimalDigit}*)

byteNumLexema                           ({numLexema}b)

%x STRING
stringLexemaEnter                       (\".*\")
stringUnclosed                          (\".*\\n)

%%

{voidToken}                             { return VOID; }
{intToken}                              { return INT; }
{byteToken}                             { return BYTE; }
{boolToken}                             { return BOOL; }
{andToken}                              { return AND; }
{orToken}                               { return OR; }
{notToken}                              { return NOT; }
{trueToken}                             { return TRUE; }
{falseToken}                            { return FALSE; }
{returnToken}                           { return RETURN; }
{ifToken}                               { return IF; }
{elseToken}                             { return ELSE; }
{whileToken}                            { return WHILE; }
{breakToken}                            { return BREAK; }
{continueToken}                         { return CONTINUE; }
{semicolonToken}                        { return SC; }
{commaToken}                            { return COMMA; }
{lParenToken}                           { return LPAREN; }
{rParenToken}                           { return RPAREN; }
{lBraceToken}                           { return LBRACE; }
{rBraceToken}                           { return RBRACE; }
{assignToken}                           { return ASSIGN; }

{relopSign}                             { return RELOP; }
{binopSign}                             { return BINOP; }
{commentLexema}                         { return COMMENT; }
{idLexema}                              { return ID; }
{numLexema}                             { return NUM; }
{byteNumLexema}                         { return NUM_B; }

{stringLexemaEnter}                     { return STRING; }
{stringUnclosed}                        { output::errorUnclosedString(); return 0; }

{whitespace}                            ;
.                                       { output::errorUnknownChar(*yytext); return 0; }

%%
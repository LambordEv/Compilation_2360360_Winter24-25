#include "tokens.hpp"
#include "output.hpp"
#include <stdlib.h>
#include <stdio.h>

#define IS_HEX_VALUE(val)       ((('0' <= val && '9' >= val) || ('a' <= val && 'f' >= val) || ('A' <= val && 'F' >= val)) ? true : false)

int accumalatedStrLen;
char accumalatedString[2096];

bool isAPrintableChar(char givenHexValue)
{
    bool result = false;
    result = ((0x20 <= givenHexValue) && (givenHexValue <= 0x7E));
    result = result || (0x09 == givenHexValue); //tab charecter
    result = result || (0x0A == givenHexValue); //new line charecter
    result = result || (0x0D == givenHexValue); //carriage return charecter (another way of new line charecter)
    
    return result;
}

int checkGivenStringLexema(const char* givenStr)
{
    const int givenStrLen = accumalatedStrLen;
    char resultString[1025] = {0};
    int resultLen = 0;
    
    // printf("Given String is - %s\n", givenStr);
    int i = 0;
    for(; i < givenStrLen - 1; i++)
    {
        // printf("Current Charecter is - %c\n", givenStr[i]);
        if('\\' == givenStr[i])
        {
            i++;
            switch(givenStr[i])
            {
            case '\\':
                resultString[resultLen] = '\\';
                break;
            case '"':
                resultString[resultLen] = '"';
                break;
            case 'n':
                resultString[resultLen] = '\n';
                break;
            case 'r':
                resultString[resultLen] = '\r';
                break;
            case 't':
                resultString[resultLen] = '\t';
                break;
            case '0':
                resultString[resultLen] = '\0';
                break;
            case 'x': //hex form of a valid char of format xDD
                if(((i + 1) < (givenStrLen - 1)) && IS_HEX_VALUE(givenStr[i+1]))
                {
                    if(IS_HEX_VALUE(givenStr[i+2]))
                    {
                        int receivedHexValue = strtol(&givenStr[i+1], NULL, 16);
                        if(isAPrintableChar(receivedHexValue)) //The given value is printable?
                        {
                            resultString[resultLen] = receivedHexValue;
                        }
                        else //Given Hex charecter is not printable
                        {
                            char hexEscapeErr[4] = {'x', givenStr[i+1], givenStr[i+2], '\0'};
                            output::errorUndefinedEscape(hexEscapeErr);
                        }
                        i = i + 2;
                    }
                    else // The second charecter is not a hex value.
                    {
                        char hexEscapeErr[4] = {'x', givenStr[i+1], givenStr[i+2], '\0'};
                        if(((i + 2) == (givenStrLen - 1)))
                        {
                            hexEscapeErr[2] = '\0';
                        }
                        output::errorUndefinedEscape(hexEscapeErr);
                    }
                }
                else // The first charecter is not a hex value.
                {
                    char hexEscapeErr[4] = {'x', givenStr[i+1], givenStr[i+2], '\0'};
                    if(((i + 1) == (givenStrLen - 1)))
                    {
                        hexEscapeErr[1] = '\0';
                    }
                    else if (((i + 2) == (givenStrLen - 1)))
                    {
                        hexEscapeErr[2] = '\0';
                    }
                    output::errorUndefinedEscape(hexEscapeErr);
                }
                break;
            default: // The charecter after the escape char '\' is not valid
                char hexEscapeErr[2] = {givenStr[i], '\0'};
                output::errorUndefinedEscape(hexEscapeErr);
            }
        }
        else
        {
            if('"' == givenStr[i])
            {
                //The String has a '"' charecter in the middle of it without an escape charecter
                output::errorUnclosedString();
            }

            resultString[resultLen] = givenStr[i];
        }
        resultLen++;
    }

    // The String is not closed with '"' as supposed to
    if((i == 0) || ('"' != givenStr[i]))
    {
        // printf("%d\n", i);
        output::errorUnclosedString();
    }
    else
    {
        output::printToken(yylineno, STRING, resultString);
        accumalatedStrLen = 0;
    }

    return 0;
}

int main() {
    enum tokentype token;

    // read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex()))) {
        switch(token)
        {
        case VOID:
        case INT:
        case BYTE:
        case BOOL:
        case AND:
        case OR:
        case NOT:
        case TRUE:
        case FALSE:
        case RETURN:
        case IF:
        case ELSE:
        case WHILE:
        case BREAK:
        case CONTINUE:
        case SC:
        case COMMA:
        case LPAREN:
        case RPAREN:
        case LBRACE:
        case RBRACE:
        case ASSIGN:
        case RELOP:
        case BINOP:
        case COMMENT:
        case ID:
        case NUM:
        case NUM_B:
            output::printToken(yylineno, token, yytext);
            break;

        case STRING:
            // printf("Given String in case - %s\n", accumalatedString);
            checkGivenStringLexema(accumalatedString);
            break;

        default:
            output::errorUnknownChar(*yytext);
            break;
        }
    }
    return 0;
}
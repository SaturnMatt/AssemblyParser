
#include <iostream>


#include "parser.h"

using namespace std;

int main() {

    auto parseInt = Parser("     34.35%$ fd   ").parseInteger();
    if (parseInt) cout << "parseInteger: " << parseInt->value << endl;
    else cout << "parseInteger <none>" << endl;

    auto parseIdent = Parser("        hello123%#@ fd   ").parseIdentifier();
    if (parseIdent) cout << "parseIdentifier: " << parseIdent->value << endl;
    else cout << "parseIdentifier <none>" << endl;

    auto parseComment = Parser("       ;hello123%#@ fd   t").parseComment();
    if (parseComment) cout << "parseComment: " << parseComment->value << endl;
    else cout << "parseComment <none>" << endl;

    auto parseLabel = Parser("        hello:123%#@ fd   ").parseLabel();
    if (parseLabel) cout << "parseLabel: " << parseLabel->value.value << endl;
    else cout << "parseLabel <none>" << endl;

    auto parseOperand = Parser("      hello:123%#@ fd   ").parseOperand();
    if (parseOperand) {
        if (parseOperand->integer) cout << "parseOperand (integer): " << parseOperand->integer->value << endl;
        else if (parseOperand->identifier) cout << "parseOperand (identifier): " << parseOperand->identifier->value << endl;
        else cout << "parseOperand <none>" << endl;
    }

    auto parseOpcode = Parser("        movi:123%#@ fd   ").parseOpcode();
    if (parseOpcode) {
        auto opCodeName = Opcodes::nameFromOpCode(parseOpcode->value);
        if (opCodeName) cout << "parseOpcode: " << *opCodeName << endl;
        else cout << "parseOpcode <none>" << endl;
    }
    else cout << "parseOpcode <none>" << endl;

    auto parseInstruction = Parser("        movi 123 matt test%&$#*(   ").parseInstruction();
    if (parseInstruction) {
        auto opCodeName = Opcodes::nameFromOpCode(parseInstruction->opcode.value);
        if (opCodeName) {
            cout << "parseInstruction (opcode): " << *opCodeName << endl;
            for (auto& operand : parseInstruction->operands) {
                if (operand.integer) cout << "parseInstruction (operand integer): " << operand.integer->value << endl;
                else if (operand.identifier) cout << "parseInstruction (operand identifier): " << operand.identifier->value << endl;
                else cout << "parseInstruction (operand): <unknown>" << endl;
            }
        }
        else cout << "parseInstruction <unknown opcode>" << endl;
    }
    else cout << "parseInstruction <none>" << endl;





    return 0;
}
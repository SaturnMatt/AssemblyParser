
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "parser.h"

using namespace std;

void printStatement(const Statement& statement) {
    if (statement.label) cout << "[lbl]" << statement.label->value.value;
    else if (statement.instruction) {
        auto opcodeName = Opcodes::nameFromOpCode(statement.instruction->opcode.value);
        if (opcodeName) cout << "[op]" << *opcodeName << " ";
        else cout << "parseStatement opcode <unknown> ";
        for (auto& operand : statement.instruction->operands) {
            if (operand.integer) cout << "[int]" << operand.integer->value << " ";
            else if (operand.identifier) cout << "[id]" << operand.identifier->value << " ";
            else cout << "parseStatement operand <unknown> ";
        }
    }
    if (statement.comment) cout << "[com]" << statement.comment->value;
    cout << endl;
}

void printProgram(const Program& program) {
    for (auto& statement : program.statements) {
        printStatement(statement);
    }
}


string loadFile(const string& fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    else {
        throw runtime_error("Could not open file " + fileName);
    }
}

int main() {

    try {
        Program program = Parser().parse(loadFile("test.asm"));
        printProgram(program);
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Unknown error occurred." << endl;
        return 1;
    }


    return 0;
}
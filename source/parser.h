#pragma once

#include <optional>
#include <string>
#include <vector>
#include "progtree.h"
#include "opcodes.h"

using namespace std;


class Parser {
public:

    string input;
    size_t position;

    Parser(const string& input) : input(input), position(0) {}

    optional<Program> parse(const string& input) {
        this->input = input;
        this->position = 0;
        return parseProgram();
    }

    //<program> :: = [<vws>] <statement> {<vws> <statement>}[<vws>]
    optional<Program> parseProgram() {
        parseVerticalWhitespace();
        Program program;
        while (true) {
            auto statement = parseStatement();
            if (statement) program.statements.push_back(statement.value());
            else break;
            if (parseVerticalWhitespace() == false) break;
        }
        parseVerticalWhitespace();
        parseHorizontalWhitespace();
        if (position < input.size()) return {};
        return program;
    }

    //<statement> ::= <label> [<comment>] | <instruction> [<comment>] | <comment>
    optional<Statement> parseStatement() {
        parseHorizontalWhitespace();
        auto label = parseLabel();
        if (label) {
            Statement statement;
            statement.label = label;
            auto comment = parseComment();
            if (comment) statement.comment = comment;
            return statement;
        }
        auto instruction = parseInstruction();
        if (instruction) {
            Statement statement;
            statement.instruction = instruction;
            auto comment = parseComment();
            if (comment) statement.comment = comment;
            return statement;
        }
        auto comment = parseComment();
        if (comment) {
            Statement statement;
            statement.comment = comment;
            return statement;
        }
        return {};
    }

    //<instruction> ::= <opcode> {<hzws> <operand>}
    optional<Instruction> parseInstruction() {
        parseHorizontalWhitespace();
        auto result = parseOpcode();
        if (result) {
            Instruction instruction;
            instruction.opcode = result.value();
            while (parseHorizontalWhitespace()) {
                auto result = parseOperand();
                if (result) instruction.operands.push_back(result.value());
                else break;
            }
            return instruction;
        }
        return {};
    }

    //<opcode> :: = 'mov' | 'add' | 'sub' | 'mul' | 'div' | etc...
    optional<Opcode> parseOpcode() {
        size_t revertPosition = position;
        auto result = parseIdentifier();
        if (result) {
            auto opCode = Opcodes::opCodeFromName(result->value);
            if (opCode) return Opcode{ *opCode };
        }
        position = revertPosition;
        return {};
    }

    //<operand> ::= <integer> | <identifier>
    optional<Operand> parseOperand() {
        auto integer = parseInteger();
        if (integer) return Operand{ *integer, {} };
        auto identifier = parseIdentifier();
        if (identifier) return Operand{ {}, *identifier };
        return {};
    }

    // if the parse method fails, position should not change
    //<label> :: = <identifier>':'
    optional<Label> parseLabel() {
        parseHorizontalWhitespace();
        size_t revertPosition = position;
        auto result = parseIdentifier();
        if (result && position < input.size() && input[position] == ':') {
            position++;
            return Label{ *result };
        }
        position = revertPosition;
        return {};
    }

    // <comment> :: = ';'{<any character not a new line>}
    optional<Comment> parseComment() {
        parseHorizontalWhitespace();
        string result;
        if (position < input.size() && input[position] == ';') {
            position++;
        }
        else return {};
        while (position < input.size() && isVerticalWhitespace() == false) {
            result += input[position];
            position++;
        }
        return Comment{ result };
    }

    // <identifier> ::= <letter>{<letter>|<digit>}
    optional<Identifier> parseIdentifier() {
        parseHorizontalWhitespace();
        string result;
        if (position < input.size() && isalpha(input[position])) {
            result += input[position];
            position++;
        }
        else return {};
        while (position < input.size() && (isalpha(input[position])) || isdigit(input[position])) {
            result += input[position];
            position++;
        }
        return Identifier{ result };
    }

    // <integer> ::= <digit>{<digit>}
    optional<Integer> parseInteger() {
        parseHorizontalWhitespace();
        int16_t result = 0;
        if (position < input.size() && isdigit(input[position])) {
            result = input[position] - '0';
            position++;
        }
        else return {};
        while (position < input.size() && isdigit(input[position])) {
            result = result * 10 + (input[position] - '0');
            position++;
        }
        return Integer{ result };
    }

    bool isVerticalWhitespace() {
        if (position < input.size() && (input[position] == '\n' || input[position] == '\r')) return true;
        else return false;
    }

    bool parseVerticalWhitespace() {
        parseHorizontalWhitespace();
        bool result = false;
        while (isVerticalWhitespace()) {
            result = true;
            position++;
            parseHorizontalWhitespace();
        }
        return result;
    }

    bool isHorizontalWhitespace() {
        if (position < input.size() && (input[position] == ' ' || input[position] == '\t' || input[position] == ',')) return true;
        else return false;
    }

    bool parseHorizontalWhitespace() {
        bool result = false;
        while (isHorizontalWhitespace()) {
            result = true;
            position++;
        }
        return result;
    }



};
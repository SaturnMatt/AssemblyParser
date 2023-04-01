#pragma once

#include <optional>
#include <string>
#include <vector>

using namespace std;

class Integer {
public:
    int16_t value;
};

class Identifier {
public:
    string value;
};

class Comment {
public:
    string value;
};

class Label {
public:
    Identifier value;
};

class Operand {
public:
    optional<Integer> integer;
    optional<Identifier> identifier;
};

class Opcode {
public:
    int16_t value;
};

class Instruction {
public:
    Opcode opcode;
    vector<Operand> operands;
};

class Statement {
public:
    optional<Label> label;
    optional<Instruction> instruction;
    optional<Comment> comment;
};

class Program {
public:
    vector<Statement> statements;
};

#pragma once

#include <string>
#include <map>

using namespace std;

class Opcodes {
public:
    enum : int16_t {
        halt, push, pop, peek, mov, movi,
        add, addi, sub, subi,
        jmp, jz, jnz, call, ret,
    };

    static optional<int> argumentCount(int16_t opcode) {
        auto it = opCodeMap().find(opcode);
        if (it == opCodeMap().end()) return {};
        return it->second.argCount;
    }

    static optional<string> nameFromOpCode(int16_t opcode) {
        auto it = opCodeMap().find(opcode);
        if (it == opCodeMap().end()) return {};
        return it->second.name;
    }

    static optional<int16_t> opCodeFromName(const std::string& opName) {
        auto it = nameToOpCodeMap().find(opName);
        if (it == nameToOpCodeMap().end()) return {};
        return it->second;
    }

    bool isOpCode(int16_t opcode) {
        return opCodeMap().find(opcode) != opCodeMap().end();
    }

    bool isOpCode(const std::string& opName) {
        return nameToOpCodeMap().find(opName) != nameToOpCodeMap().end();
    }

private:
    struct OpCodeInfo {
        std::string name;
        int argCount;
    };

    static const std::map<int16_t, OpCodeInfo>& opCodeMap() {
        static const std::map<int16_t, OpCodeInfo> opCodeMap = {
            {halt, {"halt", 0}}, {push, {"push", 1}}, {pop, {"pop", 1}}, {peek, {"peek", 1}}, {mov, {"mov", 2}}, {movi, {"movi", 2}},
            {add, {"add", 2}}, {addi, {"addi", 2}}, {sub, {"sub", 2}}, {subi, {"subi", 2}},
            {jmp, {"jmp", 1}}, {jz, {"jz", 2}}, {jnz, {"jnz", 2}}, {call, {"call", 1}}, {ret, {"ret", 0}},
        };
        return opCodeMap;
    }

    static const std::map<std::string, int16_t>& nameToOpCodeMap() {
        static std::map<std::string, int16_t> nameToOpCodeMap;
        if (nameToOpCodeMap.empty()) {
            for (const auto& [op, info] : opCodeMap()) {
                nameToOpCodeMap[info.name] = op;
            }
        }
        return nameToOpCodeMap;
    }
};

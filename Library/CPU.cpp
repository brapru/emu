#include <CPU.h>
#include <MMU.h>
#include <Utils/Format.h>

CPU& CPU::the()
{
    static CPU s_the;
    return s_the;
}

/*
    TODO: As we implement the instructions, will want to have a cleaner
    way to lookup opcodes. 
    Ideas: 
        opcode -> opcode_map_lookup -> execute function
        Instruction class with function pointer and name members
*/
void CPU::cycle()
{
    auto opcode = MMU::read(m_pc.value());
    m_pc.increment();

    switch (opcode) {
    case 0x00:
        outln("Executing nop: {0:x}", opcode);
        break;
    default:
        outln("OPCODE NOT IMPLEMENTED: {0:x}", opcode);
        exit(1);
    }
}
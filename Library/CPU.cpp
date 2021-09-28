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
    auto opcode = fetch_byte();

    switch (opcode) {
    case 0x00:
        outln("Instruction: NOP");
        break;
    case 0xc3: {
        auto address = fetch_word();
        outln("Instruction: JP {:X}, PC {:X}", address, m_pc.value());
        m_pc.set(address);
        break;
    }
    default:
        outln("OPCODE NOT IMPLEMENTED: {0:x}", opcode);
        exit(1);
    }
}

uint8_t CPU::fetch_byte()
{
    uint8_t byte = MMU::read(m_pc.value());
    m_pc.increment();

    return byte;
}

uint16_t CPU::fetch_word()
{
    uint8_t lo = fetch_byte();
    uint8_t hi = fetch_byte();

    return (hi << 8) | lo;
}
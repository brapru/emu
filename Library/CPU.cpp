#include <CPU.h>
#include <MMU.h>
#include <Utils/Bitwise.h>
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

    out("PC: 0x{:04X}, OP: 0x{:02X} INSTR: ",
        m_pc.value() - 1,
        opcode);

    switch (opcode) {
    case 0x00:
        instruction_nop();
        break;
    case 0xAF: {
        instruction_xor();
        break;
    }
    case 0xc3: {
        instruction_jp();
        break;
    }
    case 0xF3: {
        instruction_di();
        break;
    }
    default:
        outln("OPCODE NOT IMPLEMENTED: {:X}", opcode);
        exit(1);
    }

    outln("A: 0x{:02X}, B: 0x{:02X}, C: 0x{:02X}, Flag: {:08B}",
        m_a.value(),
        m_b.value(),
        m_c.value(),
        m_flags.value());
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
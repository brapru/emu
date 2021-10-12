#include <CPU.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

CPU::CPU(MMU& mmu)
    : m_mmu(mmu)
{
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

    execute_instruction(opcode);

    outln("SP: 0x{:02x}, AB: 0x{:02X}{:02X}, CD: 0x{:02X}{:02X}, HL: 0x{:02X}, Flag: {:08B}",
        m_sp.value(),
        m_a.value(),
        m_b.value(),
        m_c.value(),
        m_d.value(),
        m_hl.value(),
        m_flags.value());
}

uint8_t CPU::fetch_byte()
{
    uint8_t byte = m_mmu.read(m_pc.value());
    m_pc.increment();

    return byte;
}

uint16_t CPU::fetch_word()
{
    uint8_t lo = fetch_byte();
    uint8_t hi = fetch_byte();

    return (hi << 8) | lo;
}
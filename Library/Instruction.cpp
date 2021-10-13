#include <CPU.h>
#include <Utils/Format.h>

void CPU::instruction_nop(void)
{
    out("NOP ");
}

void CPU::instruction_di(void)
{
    out("DI ");
    m_interrupt_master_enable = false;
}

void CPU::instruction_ld(WordRegister& reg)
{
    auto value = CPU::fetch_word();
    reg.set(value);
}

void CPU::instruction_ld(WholeRegister& reg)
{
    auto value = CPU::fetch_word();
    reg.set(value);
}

void CPU::instruction_ld(ByteRegister& reg)
{
    auto value = CPU::fetch_byte();
    out("LD A, 0x{:02X} ", value);
    reg.set(value);
}

void CPU::instruction_ld(ByteRegister& reg, ByteRegister& from_reg)
{
    reg.set(from_reg.value());
}

void CPU::instruction_ld_reg_to_addr(ByteRegister& reg)
{
    auto address = CPU::fetch_word();
    out("LD NN A 0x{:04X} ", address);
    CPU::m_mmu.write(address, reg.value());
}

void CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    out("JP 0x{:04X} ", address);
    m_pc.set(address);
}

void CPU::instruction_xor(void)
{
    auto value = fetch_byte();
    out("XOR A 0x{:02X}", value);

    auto result = m_a.value() ^ value & 0xFF;

    m_f.set_zero_flag(result == 0);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    m_a.set(result);
}

void CPU::execute_instruction(uint8_t opcode)
{

    switch (opcode) {
    case 0x00:
        instruction_nop();
        break;
    case 0x0E:
        instruction_ld(m_c);
        break;
    case 0x11:
        instruction_ld(m_de);
        break;
    case 0x21:
        instruction_ld(m_hl);
        break;
    case 0x31:
        instruction_ld(m_sp);
        break;
    case 0x47:
        instruction_ld(m_b, m_a);
        break;
    case 0xAF:
        instruction_xor();
        break;
    case 0x3E:
        instruction_ld(m_a);
        break;
    case 0xEA:
        instruction_ld_reg_to_addr(m_a);
        break;
    case 0xc3:
        instruction_jp();
        break;
    case 0xF3:
        instruction_di();
        break;
    default:
        outln("OPCODE NOT IMPLEMENTED: {:X}", opcode);
        exit(1);
    }
}
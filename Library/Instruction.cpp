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

void CPU::instruction_ld_inc(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());
    m_a.set(value);
    reg.increment();
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

void CPU::instruction_ld(ByteRegister& reg, WholeRegister& from_reg)
{
    auto value = m_mmu.read(from_reg.value());
    reg.set(from_reg.value());
}

void CPU::instruction_ld_reg_to_addr(ByteRegister& reg)
{
    auto address = CPU::fetch_word();
    out("LD NN A 0x{:04X} ", address);
    CPU::m_mmu.write(address, reg.value());
}

void CPU::instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg)
{
    auto address = whole_reg.value();
    CPU::m_mmu.write(address, reg.value());
}

void CPU::instruction_ldh_a_to_memory()
{
    auto lo = fetch_byte();
    auto address = ((uint16_t)0xFF << 8) | lo;
    CPU::m_mmu.write(address, m_a.value());
}

void CPU::instruction_inc(ByteRegister& reg)
{
    reg.increment();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((reg.value() & 0x10) == 0x10) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_inc(WholeRegister& reg)
{
    reg.increment();
}

void CPU::instruction_inc(WordRegister& reg)
{
    reg.increment();
}

void CPU::instruction_dec(ByteRegister& reg)
{
    reg.decrement();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    ((reg.value() & 0x10) == 0x10) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_call(void)
{
    auto address = CPU::fetch_word();
    stack_push(m_pc);
    m_pc.set(address);
}

void CPU::instruction_push(WholeRegister& reg)
{
    stack_push(reg);
}

void CPU::instruction_pop(WholeRegister& reg)
{
    auto value = stack_pop();
    reg.set(value);
}

void CPU::instruction_ret(void)
{
    auto address = stack_pop();
    m_pc.set(address);
}

void CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    out("JP 0x{:04X} ", address);
    m_pc.set(address);
}

void CPU::instruction_jr(void)
{
    auto jumps = static_cast<int8_t>(fetch_byte());
    m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));
}

void CPU::instruction_jr(uint8_t const& opcode)
{
    bool should_jp;

    switch (opcode) {
    case 0x20: // NZ
        should_jp = !m_f.zero_flag();
        break;
    }

    if (should_jp) {
        auto jumps = static_cast<int8_t>(fetch_byte());
        m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));
    } else {
        fetch_byte();
    }
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
    case 0x01:
        instruction_ld(m_bc);
        break;
    case 0x02:
        instruction_ld_reg_to_addr(m_bc, m_a);
        break;
    case 0x03:
        instruction_inc(m_bc);
        break;
    case 0x06:
        instruction_ld(m_b);
        break;
    case 0x0A:
        instruction_ld(m_a, m_bc);
        break;
    case 0x0D:
        instruction_dec(m_c);
        break;
    case 0x0E:
        instruction_ld(m_c);
        break;
    case 0x11:
        instruction_ld(m_de);
        break;
    case 0x12:
        instruction_ld_reg_to_addr(m_de, m_a);
        break;
    case 0x13:
        instruction_inc(m_de);
        break;
    case 0x14:
        instruction_inc(m_d);
        break;
    case 0x16:
        instruction_ld(m_d);
        break;
    case 0x18:
        instruction_jr();
        break;
    case 0x1A:
        instruction_ld(m_a, m_de);
        break;
    case 0x1C:
        instruction_inc(m_e);
        break;
    case 0x1E:
        instruction_ld(m_e);
        break;
    case 0x20:
        instruction_jr(opcode);
        break;
    case 0x21:
        instruction_ld(m_hl);
        break;
    case 0x23:
        instruction_inc(m_hl);
        break;
    case 0x26:
        instruction_ld(m_h);
        break;
    case 0x2A:
        instruction_ld_inc(m_hl);
        break;
    case 0x2E:
        instruction_ld(m_l);
        break;
    case 0x31:
        instruction_ld(m_sp);
        break;
    case 0x33:
        instruction_inc(m_sp);
        break;
    case 0x3E:
        instruction_ld(m_a);
        break;
    case 0x40:
        instruction_ld(m_b, m_b);
        break;
    case 0x41:
        instruction_ld(m_b, m_c);
        break;
    case 0x42:
        instruction_ld(m_b, m_d);
        break;
    case 0x43:
        instruction_ld(m_b, m_e);
        break;
    case 0x44:
        instruction_ld(m_b, m_h);
        break;
    case 0x45:
        instruction_ld(m_b, m_l);
        break;
    case 0x46:
        instruction_ld(m_b, m_hl);
        break;
    case 0x47:
        instruction_ld(m_b, m_a);
        break;
    case 0x48:
        instruction_ld(m_c, m_b);
        break;
    case 0x49:
        instruction_ld(m_c, m_c);
        break;
    case 0x4A:
        instruction_ld(m_c, m_d);
        break;
    case 0x4B:
        instruction_ld(m_c, m_d);
        break;
    case 0x4C:
        instruction_ld(m_c, m_h);
        break;
    case 0x4D:
        instruction_ld(m_c, m_l);
        break;
    case 0x4E:
        instruction_ld(m_c, m_hl);
        break;
    case 0x4F:
        instruction_ld(m_c, m_a);
        break;
    case 0x50:
        instruction_ld(m_d, m_b);
        break;
    case 0x51:
        instruction_ld(m_d, m_c);
        break;
    case 0x52:
        instruction_ld(m_d, m_d);
        break;
    case 0x53:
        instruction_ld(m_d, m_e);
        break;
    case 0x54:
        instruction_ld(m_d, m_h);
        break;
    case 0x55:
        instruction_ld(m_d, m_l);
        break;
    case 0x56:
        instruction_ld(m_d, m_hl);
        break;
    case 0x57:
        instruction_ld(m_d, m_a);
        break;
    case 0x58:
        instruction_ld(m_e, m_b);
        break;
    case 0x59:
        instruction_ld(m_e, m_c);
        break;
    case 0x5A:
        instruction_ld(m_e, m_d);
        break;
    case 0x5B:
        instruction_ld(m_e, m_e);
        break;
    case 0x5C:
        instruction_ld(m_e, m_h);
        break;
    case 0x5D:
        instruction_ld(m_e, m_l);
        break;
    case 0x5E:
        instruction_ld(m_e, m_hl);
        break;
    case 0x5F:
        instruction_ld(m_e, m_a);
        break;
    case 0x60:
        instruction_ld(m_h, m_b);
        break;
    case 0x61:
        instruction_ld(m_h, m_c);
        break;
    case 0x62:
        instruction_ld(m_h, m_d);
        break;
    case 0x63:
        instruction_ld(m_h, m_e);
        break;
    case 0x64:
        instruction_ld(m_h, m_h);
        break;
    case 0x65:
        instruction_ld(m_h, m_l);
        break;
    case 0x66:
        instruction_ld(m_h, m_hl);
        break;
    case 0x67:
        instruction_ld(m_h, m_a);
        break;
    case 0x68:
        instruction_ld(m_l, m_b);
        break;
    case 0x69:
        instruction_ld(m_l, m_c);
        break;
    case 0x6A:
        instruction_ld(m_l, m_d);
        break;
    case 0x6B:
        instruction_ld(m_l, m_e);
        break;
    case 0x6C:
        instruction_ld(m_l, m_h);
        break;
    case 0x6D:
        instruction_ld(m_l, m_l);
        break;
    case 0x6E:
        instruction_ld(m_l, m_hl);
        break;
    case 0x6F:
        instruction_ld(m_l, m_a);
        break;
    case 0x70:
        instruction_ld_reg_to_addr(m_hl, m_b);
        break;
    case 0x71:
        instruction_ld_reg_to_addr(m_hl, m_c);
        break;
    case 0x72:
        instruction_ld_reg_to_addr(m_hl, m_d);
        break;
    case 0x73:
        instruction_ld_reg_to_addr(m_hl, m_e);
        break;
    case 0x74:
        instruction_ld_reg_to_addr(m_hl, m_h);
        break;
    case 0x75:
        instruction_ld_reg_to_addr(m_hl, m_l);
        break;
    case 0x77:
        instruction_ld_reg_to_addr(m_hl, m_a);
        break;
    case 0x78:
        instruction_ld(m_a, m_b);
        break;
    case 0x79:
        instruction_ld(m_a, m_c);
        break;
    case 0x7A:
        instruction_ld(m_a, m_d);
        break;
    case 0x7B:
        instruction_ld(m_a, m_e);
        break;
    case 0x7C:
        instruction_ld(m_a, m_h);
        break;
    case 0x7D:
        instruction_ld(m_a, m_l);
        break;
    case 0x7E:
        instruction_ld(m_a, m_hl);
        break;
    case 0x7F:
        instruction_ld(m_a, m_a);
        break;
    case 0xAF:
        instruction_xor();
        break;
    case 0xC1:
        instruction_pop(m_bc);
        break;
    case 0xC3:
        instruction_jp();
        break;
    case 0xC5:
        instruction_push(m_bc);
        break;
    case 0xCD:
        instruction_call();
        break;
    case 0xC9:
        instruction_ret();
        break;
    case 0xD1:
        instruction_pop(m_de);
        break;
    case 0xD5:
        instruction_push(m_de);
        break;
    case 0xE0:
        instruction_ldh_a_to_memory();
        break;
    case 0xE1:
        instruction_pop(m_hl);
        break;
    case 0xE5:
        instruction_push(m_hl);
        break;
    case 0xEA:
        instruction_ld_reg_to_addr(m_a);
        break;
    case 0xF1:
        instruction_pop(m_af);
        break;
    case 0xF3:
        instruction_di();
        break;
    case 0xF5:
        instruction_push(m_af);
        break;
    default:
        outln("OPCODE NOT IMPLEMENTED: {:X}", opcode);
        exit(1);
    }
}
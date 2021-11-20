#include <CPU.h>
#include <Utils/Format.h>

unsigned long CPU::m_execute_instruction(uint8_t opcode)
{
    switch (opcode) {
    case 0x00:
        return instruction_nop();
    case 0x01:
        return instruction_ld(m_bc);
    case 0x02:
        return instruction_ld_reg_to_addr(m_bc, m_a);
    case 0x03:
        return instruction_inc(m_bc);
    case 0x04:
        return instruction_inc(m_b);
    case 0x05:
        return instruction_dec(m_b);
    case 0x06:
        return instruction_ld(m_b);
    case 0x07:
        return instruction_rlca();
    case 0x08:
        return instruction_ld_reg_to_addr(m_sp);
    case 0x09:
        return instruction_add_hl(m_bc);
    case 0x0A:
        return instruction_ld(m_a, m_bc);
    case 0x0B:
        return instruction_dec(m_bc);
    case 0x0C:
        return instruction_inc(m_c);
    case 0x0D:
        return instruction_dec(m_c);
    case 0x0E:
        return instruction_ld(m_c);
    case 0x0F:
        return instruction_rrca();
    case 0x11:
        return instruction_ld(m_de);
    case 0x12:
        return instruction_ld_reg_to_addr(m_de, m_a);
    case 0x13:
        return instruction_inc(m_de);
    case 0x14:
        return instruction_inc(m_d);
    case 0x15:
        return instruction_dec(m_d);
    case 0x16:
        return instruction_ld(m_d);
    case 0x17:
        return instruction_rla();
    case 0x18:
        return instruction_jr();
    case 0x19:
        return instruction_add_hl(m_de);
    case 0x1A:
        return instruction_ld(m_a, m_de);
    case 0x1B:
        return instruction_dec(m_de);
    case 0x1C:
        return instruction_inc(m_e);
    case 0x1D:
        return instruction_dec(m_e);
    case 0x1E:
        return instruction_ld(m_e);
    case 0x1F:
        return instruction_rra();
    case 0x20:
        return instruction_jr(Condition::NZ);
    case 0x21:
        return instruction_ld(m_hl);
    case 0x22:
        return instruction_ld_reg_to_addr(m_hl, m_a, RegisterOperation::Increment);
    case 0x23:
        return instruction_inc(m_hl);
    case 0x24:
        return instruction_inc(m_h);
    case 0x25:
        return instruction_dec(m_h);
    case 0x26:
        return instruction_ld(m_h);
    case 0x28:
        return instruction_jr(Condition::Z);
    case 0x29:
        return instruction_add_hl(m_hl);
    case 0x2A:
        return instruction_ld(m_hl, RegisterOperation::Increment);
    case 0x2B:
        return instruction_dec(m_hl);
    case 0x2C:
        return instruction_inc(m_l);
    case 0x2D:
        return instruction_dec(m_l);
    case 0x2E:
        return instruction_ld(m_l);
    case 0x2F:
        return instruction_cpl();
    case 0x30:
        return instruction_jr(Condition::NC);
    case 0x31:
        return instruction_ld(m_sp);
    case 0x32:
        return instruction_ld_reg_to_addr(m_hl, m_a, RegisterOperation::Decrement);
    case 0x33:
        return instruction_inc(m_sp);
    case 0x34:
        return instruction_inc_hl_address();
    case 0x35:
        return instruction_dec_hl_address();
    case 0x36:
        return instruction_ld_hl();
    case 0x37:
        return instruction_scf();
    case 0x38:
        return instruction_jr(Condition::C);
    case 0x39:
        return instruction_add_hl(m_sp);
    case 0x3A:
        return instruction_ld(m_hl, RegisterOperation::Decrement);
    case 0x3B:
        return instruction_dec(m_sp);
    case 0x3C:
        return instruction_inc(m_a);
    case 0x3D:
        return instruction_dec(m_a);
    case 0x3E:
        return instruction_ld(m_a);
    case 0x3F:
        return instruction_ccf();
    case 0x40:
        return instruction_ld(m_b, m_b);
    case 0x41:
        return instruction_ld(m_b, m_c);
    case 0x42:
        return instruction_ld(m_b, m_d);
    case 0x43:
        return instruction_ld(m_b, m_e);
    case 0x44:
        return instruction_ld(m_b, m_h);
    case 0x45:
        return instruction_ld(m_b, m_l);
    case 0x46:
        return instruction_ld(m_b, m_hl);
    case 0x47:
        return instruction_ld(m_b, m_a);
    case 0x48:
        return instruction_ld(m_c, m_b);
    case 0x49:
        return instruction_ld(m_c, m_c);
    case 0x4A:
        return instruction_ld(m_c, m_d);
    case 0x4B:
        return instruction_ld(m_c, m_e);
    case 0x4C:
        return instruction_ld(m_c, m_h);
    case 0x4D:
        return instruction_ld(m_c, m_l);
    case 0x4E:
        return instruction_ld(m_c, m_hl);
    case 0x4F:
        return instruction_ld(m_c, m_a);
    case 0x50:
        return instruction_ld(m_d, m_b);
    case 0x51:
        return instruction_ld(m_d, m_c);
    case 0x52:
        return instruction_ld(m_d, m_d);
    case 0x53:
        return instruction_ld(m_d, m_e);
    case 0x54:
        return instruction_ld(m_d, m_h);
    case 0x55:
        return instruction_ld(m_d, m_l);
    case 0x56:
        return instruction_ld(m_d, m_hl);
    case 0x57:
        return instruction_ld(m_d, m_a);
    case 0x58:
        return instruction_ld(m_e, m_b);
    case 0x59:
        return instruction_ld(m_e, m_c);
    case 0x5A:
        return instruction_ld(m_e, m_d);
    case 0x5B:
        return instruction_ld(m_e, m_e);
    case 0x5C:
        return instruction_ld(m_e, m_h);
    case 0x5D:
        return instruction_ld(m_e, m_l);
    case 0x5E:
        return instruction_ld(m_e, m_hl);
    case 0x5F:
        return instruction_ld(m_e, m_a);
    case 0x60:
        return instruction_ld(m_h, m_b);
    case 0x61:
        return instruction_ld(m_h, m_c);
    case 0x62:
        return instruction_ld(m_h, m_d);
    case 0x63:
        return instruction_ld(m_h, m_e);
    case 0x64:
        return instruction_ld(m_h, m_h);
    case 0x65:
        return instruction_ld(m_h, m_l);
    case 0x66:
        return instruction_ld(m_h, m_hl);
    case 0x67:
        return instruction_ld(m_h, m_a);
    case 0x68:
        return instruction_ld(m_l, m_b);
    case 0x69:
        return instruction_ld(m_l, m_c);
    case 0x6A:
        return instruction_ld(m_l, m_d);
    case 0x6B:
        return instruction_ld(m_l, m_e);
    case 0x6C:
        return instruction_ld(m_l, m_h);
    case 0x6D:
        return instruction_ld(m_l, m_l);
    case 0x6E:
        return instruction_ld(m_l, m_hl);
    case 0x6F:
        return instruction_ld(m_l, m_a);
    case 0x70:
        return instruction_ld_reg_to_addr(m_hl, m_b);
    case 0x71:
        return instruction_ld_reg_to_addr(m_hl, m_c);
    case 0x72:
        return instruction_ld_reg_to_addr(m_hl, m_d);
    case 0x73:
        return instruction_ld_reg_to_addr(m_hl, m_e);
    case 0x74:
        return instruction_ld_reg_to_addr(m_hl, m_h);
    case 0x75:
        return instruction_ld_reg_to_addr(m_hl, m_l);
    case 0x76:
        return instruction_halt();
    case 0x77:
        return instruction_ld_reg_to_addr(m_hl, m_a);
    case 0x78:
        return instruction_ld(m_a, m_b);
    case 0x79:
        return instruction_ld(m_a, m_c);
    case 0x7A:
        return instruction_ld(m_a, m_d);
    case 0x7B:
        return instruction_ld(m_a, m_e);
    case 0x7C:
        return instruction_ld(m_a, m_h);
    case 0x7D:
        return instruction_ld(m_a, m_l);
    case 0x7E:
        return instruction_ld(m_a, m_hl);
    case 0x7F:
        return instruction_ld(m_a, m_a);
    case 0x80:
        return instruction_add(m_a, m_b);
    case 0x81:
        return instruction_add(m_a, m_c);
    case 0x82:
        return instruction_add(m_a, m_d);
    case 0x83:
        return instruction_add(m_a, m_e);
    case 0x84:
        return instruction_add(m_a, m_h);
    case 0x85:
        return instruction_add(m_a, m_l);
    case 0x86:
        return instruction_add(m_a, m_hl);
    case 0x87:
        return instruction_add(m_a, m_a);
    case 0x88:
        return instruction_adc(m_b);
    case 0x89:
        return instruction_adc(m_c);
    case 0x8A:
        return instruction_adc(m_d);
    case 0x8B:
        return instruction_adc(m_e);
    case 0x8C:
        return instruction_adc(m_h);
    case 0x8D:
        return instruction_adc(m_l);
    case 0x8E:
        return instruction_adc(m_hl);
    case 0x8F:
        return instruction_adc(m_a);
    case 0x90:
        return instruction_sub(m_b);
    case 0x91:
        return instruction_sub(m_c);
    case 0x92:
        return instruction_sub(m_d);
    case 0x93:
        return instruction_sub(m_e);
    case 0x94:
        return instruction_sub(m_h);
    case 0x95:
        return instruction_sub(m_l);
    case 0x96:
        return instruction_sub(m_hl);
    case 0x97:
        return instruction_sub(m_a);
    case 0x98:
        return instruction_sbc(m_b);
    case 0x99:
        return instruction_sbc(m_c);
    case 0x9A:
        return instruction_sbc(m_d);
    case 0x9B:
        return instruction_sbc(m_e);
    case 0x9C:
        return instruction_sbc(m_h);
    case 0x9D:
        return instruction_sbc(m_l);
    case 0x9E:
        return instruction_sbc(m_hl);
    case 0x9F:
        return instruction_sbc(m_a);
    case 0xA0:
        return instruction_and(m_b);
    case 0xA1:
        return instruction_and(m_c);
    case 0xA2:
        return instruction_and(m_d);
    case 0xA3:
        return instruction_and(m_e);
    case 0xA4:
        return instruction_and(m_h);
    case 0xA5:
        return instruction_and(m_l);
    case 0xA6:
        return instruction_and(m_hl);
    case 0xA7:
        return instruction_and(m_a);
    case 0xA8:
        return instruction_xor(m_b);
    case 0xA9:
        return instruction_xor(m_c);
    case 0xAA:
        return instruction_xor(m_d);
    case 0xAB:
        return instruction_xor(m_e);
    case 0xAC:
        return instruction_xor(m_h);
    case 0xAD:
        return instruction_xor(m_l);
    case 0xAE:
        return instruction_xor_hl_address();
    case 0xAF:
        return instruction_xor(m_a);
    case 0xB0:
        return instruction_or(m_b);
    case 0xB1:
        return instruction_or(m_c);
    case 0xB2:
        return instruction_or(m_d);
    case 0xB3:
        return instruction_or(m_e);
    case 0xB4:
        return instruction_or(m_h);
    case 0xB5:
        return instruction_or(m_l);
    case 0xB6:
        return instruction_or(m_hl);
    case 0xB7:
        return instruction_or(m_a);
    case 0xB8:
        return instruction_cp(m_b);
    case 0xB9:
        return instruction_cp(m_c);
    case 0xBA:
        return instruction_cp(m_d);
    case 0xBB:
        return instruction_cp(m_e);
    case 0xBC:
        return instruction_cp(m_h);
    case 0xBD:
        return instruction_cp(m_l);
    case 0xBE:
        return instruction_cp(m_hl);
    case 0xBF:
        return instruction_cp(m_a);
    case 0xC0:
        return instruction_ret(Condition::NZ);
    case 0xC1:
        return instruction_pop(m_bc);
    case 0xC2:
        return instruction_jp(Condition::NZ);
    case 0xC3:
        return instruction_jp();
    case 0xC4:
        return instruction_conditional_call(Condition::NZ);
    case 0xC5:
        return instruction_push(m_bc);
    case 0xC6:
        return instruction_add();
    case 0xC7:
        return instruction_rst(RST::RST0);
    case 0xC8:
        return instruction_ret(Condition::Z);
    case 0xC9:
        return instruction_ret();
    case 0xCA:
        return instruction_jp(Condition::Z);
    case 0xCB:
        return instruction_cb();
    case 0xCC:
        return instruction_conditional_call(Condition::Z);
    case 0xCD:
        return instruction_call();
    case 0xCE:
        return instruction_adc();
    case 0xCF:
        return instruction_rst(RST::RST1);
    case 0xD0:
        return instruction_ret(Condition::NC);
    case 0xD1:
        return instruction_pop(m_de);
    case 0xD2:
        return instruction_jp(Condition::NC);
    case 0xD4:
        return instruction_conditional_call(Condition::NC);
    case 0xD5:
        return instruction_push(m_de);
    case 0xD6:
        return instruction_sub();
    case 0xD7:
        return instruction_rst(RST::RST2);
    case 0xD8:
        return instruction_ret(Condition::C);
    case 0xD9:
        return instruction_reti();
    case 0xDA:
        return instruction_jp(Condition::C);
    case 0xDC:
        return instruction_conditional_call(Condition::C);
    case 0xDE:
        return instruction_sbc();
    case 0xDF:
        return instruction_rst(RST::RST3);
    case 0xE0:
        return instruction_ldh_a_to_memory();
    case 0xE1:
        return instruction_pop(m_hl);
    case 0xE2:
        return instruction_ldh_a_to_c();
    case 0xE5:
        return instruction_push(m_hl);
    case 0xE6:
        return instruction_and();
    case 0xE7:
        return instruction_rst(RST::RST4);
    case 0xE8:
        return instruction_add_sp();
    case 0xE9:
        return instruction_jp(m_hl);
    case 0xEA:
        return instruction_ld_reg_to_addr(m_a);
    case 0xEE:
        return instruction_xor();
    case 0xEF:
        return instruction_rst(RST::RST5);
    case 0xF0:
        return instruction_ldh_memory_to_a();
    case 0xF1:
        return instruction_pop(m_af);
    case 0xF2:
        return instruction_ldh_c_to_a();
    case 0xF3:
        return instruction_di();
    case 0xF5:
        return instruction_push(m_af);
    case 0xF6:
        return instruction_or();
    case 0xF7:
        return instruction_rst(RST::RST6);
    case 0xF8:
        return instruction_ld_hl_sp();
    case 0xF9:
        return instruction_ld_sp_hl();
    case 0xFA:
        return instruction_ld_addr_to_reg(m_a);
    case 0xFB:
        return instruction_ei();
    case 0xFE:
        return instruction_cp();
    case 0xFF:
        return instruction_rst(RST::RST7);
    default:
        outln("OPCODE NOT IMPLEMENTED: {:X}", opcode);
        exit(1);
    }
}

unsigned long CPU::instruction_cb()
{
    auto instruction = CPU::fetch_byte();

    switch (instruction) {
    case 0x00:
        return instruction_rlc(m_b);
    case 0x01:
        return instruction_rlc(m_c);
    case 0x02:
        return instruction_rlc(m_d);
    case 0x03:
        return instruction_rlc(m_e);
    case 0x04:
        return instruction_rlc(m_h);
    case 0x05:
        return instruction_rlc(m_l);
    case 0x06:
        return instruction_rlc(m_hl);
    case 0x07:
        return instruction_rlc(m_a);
    case 0x08:
        return instruction_rrc(m_b);
    case 0x09:
        return instruction_rrc(m_c);
    case 0x0A:
        return instruction_rrc(m_d);
    case 0x0B:
        return instruction_rrc(m_e);
    case 0x0C:
        return instruction_rrc(m_h);
    case 0x0D:
        return instruction_rrc(m_l);
    case 0x0E:
        return instruction_rrc(m_hl);
    case 0x0F:
        return instruction_rrc(m_a);
    case 0x10:
        return instruction_rl(m_b);
    case 0x11:
        return instruction_rl(m_c);
    case 0x12:
        return instruction_rl(m_d);
    case 0x13:
        return instruction_rl(m_e);
    case 0x14:
        return instruction_rl(m_h);
    case 0x15:
        return instruction_rl(m_l);
    case 0x16:
        return instruction_rl(m_hl);
    case 0x17:
        return instruction_rl(m_a);
    case 0x18:
        return instruction_rr(m_b);
    case 0x19:
        return instruction_rr(m_c);
    case 0x1A:
        return instruction_rr(m_d);
    case 0x1B:
        return instruction_rr(m_e);
    case 0x1C:
        return instruction_rr(m_h);
    case 0x1D:
        return instruction_rr(m_l);
    case 0x1E:
        return instruction_rr(m_hl);
    case 0x1F:
        return instruction_rr(m_a);
    case 0x20:
        return instruction_sla(m_b);
    case 0x21:
        return instruction_sla(m_c);
    case 0x22:
        return instruction_sla(m_d);
    case 0x23:
        return instruction_sla(m_e);
    case 0x24:
        return instruction_sla(m_h);
    case 0x25:
        return instruction_sla(m_l);
    case 0x26:
        return instruction_sla(m_hl);
    case 0x27:
        return instruction_sla(m_a);
    case 0x28:
        return instruction_sra(m_b);
    case 0x29:
        return instruction_sra(m_c);
    case 0x2A:
        return instruction_sra(m_d);
    case 0x2B:
        return instruction_sra(m_e);
    case 0x2C:
        return instruction_sra(m_h);
    case 0x2D:
        return instruction_sra(m_l);
    case 0x2E:
        return instruction_sra(m_hl);
    case 0x2F:
        return instruction_sra(m_a);
    case 0x30:
        return instruction_swap(m_b);
    case 0x31:
        return instruction_swap(m_c);
    case 0x32:
        return instruction_swap(m_d);
    case 0x33:
        return instruction_swap(m_e);
    case 0x34:
        return instruction_swap(m_h);
    case 0x35:
        return instruction_swap(m_l);
    case 0x36:
        return instruction_swap(m_hl);
    case 0x37:
        return instruction_swap(m_a);
    case 0x38:
        return instruction_srl(m_b);
    case 0x39:
        return instruction_srl(m_c);
    case 0x3A:
        return instruction_srl(m_d);
    case 0x3B:
        return instruction_srl(m_e);
    case 0x3C:
        return instruction_srl(m_h);
    case 0x3D:
        return instruction_srl(m_l);
    case 0x3E:
        return instruction_srl(m_hl);
    case 0x3F:
        return instruction_srl(m_a);
    default:
        outln("EXTERNAL OPCODE NOT IMPLEMENTED: 0xCB {:X}", instruction);
        exit(1);
    }
}

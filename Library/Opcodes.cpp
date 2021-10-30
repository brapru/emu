#include <CPU.h>
#include <Utils/Format.h>

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
    case 0x04:
        instruction_inc(m_b);
        break;
    case 0x05:
        instruction_dec(m_b);
        break;
    case 0x06:
        instruction_ld(m_b);
        break;
    case 0x08:
        instruction_ld_reg_to_addr(m_sp);
        break;
    case 0x09:
        instruction_add_hl(m_bc);
        break;
    case 0x0A:
        instruction_ld(m_a, m_bc);
        break;
    case 0x0B:
        instruction_dec(m_bc);
        break;
    case 0x0C:
        instruction_inc(m_c);
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
    case 0x15:
        instruction_dec(m_d);
        break;
    case 0x16:
        instruction_ld(m_d);
        break;
    case 0x18:
        instruction_jr();
        break;
    case 0x19:
        instruction_add_hl(m_de);
        break;
    case 0x1A:
        instruction_ld(m_a, m_de);
        break;
    case 0x1B:
        instruction_dec(m_de);
        break;
    case 0x1C:
        instruction_inc(m_e);
        break;
    case 0x1D:
        instruction_dec(m_e);
        break;
    case 0x1E:
        instruction_ld(m_e);
        break;
    case 0x1F:
        instruction_rra();
        break;
    case 0x20:
        instruction_jr(opcode);
        break;
    case 0x21:
        instruction_ld(m_hl);
        break;
    case 0x22:
        instruction_ld_reg_to_addr(m_hl, m_a, RegisterOperation::Increment);
        break;
    case 0x23:
        instruction_inc(m_hl);
        break;
    case 0x24:
        instruction_inc(m_h);
        break;
    case 0x25:
        instruction_dec(m_h);
        break;
    case 0x26:
        instruction_ld(m_h);
        break;
    case 0x28:
        instruction_jr(opcode);
        break;
    case 0x29:
        instruction_add_hl(m_hl);
        break;
    case 0x2A:
        instruction_ld(m_hl, RegisterOperation::Increment);
        break;
    case 0x2B:
        instruction_dec(m_hl);
        break;
    case 0x2C:
        instruction_inc(m_l);
        break;
    case 0x2D:
        instruction_dec(m_l);
        break;
    case 0x2E:
        instruction_ld(m_l);
        break;
    case 0x30:
        instruction_jr(opcode);
        break;
    case 0x31:
        instruction_ld(m_sp);
        break;
    case 0x32:
        instruction_ld(m_hl, RegisterOperation::Decrement);
        break;
    case 0x33:
        instruction_inc(m_sp);
        break;
    case 0x34:
        instruction_inc_hl_address();
        break;
    case 0x35:
        instruction_dec_hl_address();
        break;
    case 0x36:
        instruction_ld_hl();
        break;
    case 0x38:
        instruction_jr(opcode);
        break;
    case 0x39:
        instruction_add_hl(m_sp);
        break;
    case 0x3A:
        instruction_ld(m_hl, RegisterOperation::Decrement);
        break;
    case 0x3B:
        instruction_dec(m_sp);
        break;
    case 0x3C:
        instruction_inc(m_a);
        break;
    case 0x3D:
        instruction_dec(m_a);
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
    case 0x80:
        instruction_add(m_a, m_b);
        break;
    case 0x81:
        instruction_add(m_a, m_c);
        break;
    case 0x82:
        instruction_add(m_a, m_d);
        break;
    case 0x83:
        instruction_add(m_a, m_e);
        break;
    case 0x84:
        instruction_add(m_a, m_h);
        break;
    case 0x85:
        instruction_add(m_a, m_l);
        break;
    case 0x86:
        instruction_add(m_a, m_hl);
        break;
    case 0x87:
        instruction_add(m_a, m_a);
        break;
    case 0x88:
        instruction_adc(m_b);
        break;
    case 0x89:
        instruction_adc(m_c);
        break;
    case 0x8A:
        instruction_adc(m_d);
        break;
    case 0x8B:
        instruction_adc(m_e);
        break;
    case 0x8C:
        instruction_adc(m_h);
        break;
    case 0x8D:
        instruction_adc(m_l);
        break;
    case 0x8E:
        instruction_adc(m_hl);
        break;
    case 0x8F:
        instruction_adc(m_a);
        break;
    case 0x90:
        instruction_sub(m_b);
        break;
    case 0x91:
        instruction_sub(m_c);
        break;
    case 0x92:
        instruction_sub(m_d);
        break;
    case 0x93:
        instruction_sub(m_e);
        break;
    case 0x94:
        instruction_sub(m_h);
        break;
    case 0x95:
        instruction_sub(m_l);
        break;
    case 0x96:
        instruction_sub(m_hl);
        break;
    case 0x97:
        instruction_sub(m_a);
        break;
    case 0x98:
        instruction_sbc(m_b);
        break;
    case 0x99:
        instruction_sbc(m_c);
        break;
    case 0x9A:
        instruction_sbc(m_d);
        break;
    case 0x9B:
        instruction_sbc(m_e);
        break;
    case 0x9C:
        instruction_sbc(m_h);
        break;
    case 0x9D:
        instruction_sbc(m_l);
        break;
    case 0x9E:
        instruction_sbc(m_hl);
        break;
    case 0x9F:
        instruction_sbc(m_a);
        break;
    case 0xA0:
        instruction_and(m_b);
        break;
    case 0xA1:
        instruction_and(m_c);
        break;
    case 0xA2:
        instruction_and(m_d);
        break;
    case 0xA3:
        instruction_and(m_e);
        break;
    case 0xA4:
        instruction_and(m_h);
        break;
    case 0xA5:
        instruction_and(m_l);
        break;
    case 0xA6:
        instruction_and(m_hl);
        break;
    case 0xA7:
        instruction_and(m_a);
        break;
    case 0xA8:
        instruction_xor(m_b);
        break;
    case 0xA9:
        instruction_xor(m_c);
        break;
    case 0xAA:
        instruction_xor(m_d);
        break;
    case 0xAB:
        instruction_xor(m_e);
        break;
    case 0xAC:
        instruction_xor(m_h);
        break;
    case 0xAD:
        instruction_xor(m_l);
        break;
    case 0xAE:
        instruction_xor_hl_address();
        break;
    case 0xAF:
        instruction_xor(m_a);
        break;
    case 0xB0:
        instruction_or(m_b);
        break;
    case 0xB1:
        instruction_or(m_c);
        break;
    case 0xB2:
        instruction_or(m_d);
        break;
    case 0xB3:
        instruction_or(m_e);
        break;
    case 0xB4:
        instruction_or(m_h);
        break;
    case 0xB5:
        instruction_or(m_l);
        break;
    case 0xB6:
        instruction_or(m_hl);
        break;
    case 0xB7:
        instruction_or(m_a);
        break;
    case 0xB8:
        instruction_cp(m_b);
        break;
    case 0xB9:
        instruction_cp(m_c);
        break;
    case 0xBA:
        instruction_cp(m_d);
        break;
    case 0xBB:
        instruction_cp(m_e);
        break;
    case 0xBC:
        instruction_cp(m_h);
        break;
    case 0xBD:
        instruction_cp(m_l);
        break;
    case 0xBE:
        instruction_cp(m_hl);
        break;
    case 0xBF:
        instruction_cp(m_a);
        break;
    case 0xC0:
        instruction_ret(opcode);
        break;
    case 0xC1:
        instruction_pop(m_bc);
        break;
    case 0xC2:
        instruction_jp(Condition::NZ);
        break;
    case 0xC3:
        instruction_jp();
        break;
    case 0xC4:
        instruction_conditional_call(opcode);
        break;
    case 0xC5:
        instruction_push(m_bc);
        break;
    case 0xC6:
        instruction_add(m_a);
        break;
    case 0xC8:
        instruction_ret(opcode);
        break;
    case 0xC9:
        instruction_ret();
        break;
    case 0xCA:
        instruction_jp(Condition::Z);
        break;
    case 0xCB:
        instruction_cb();
        break;
    case 0xCD:
        instruction_call();
        break;
    case 0xCE:
        instruction_adc();
        break;
    case 0xD0:
        instruction_ret(opcode);
        break;
    case 0xD1:
        instruction_pop(m_de);
        break;
    case 0xD2:
        instruction_jp(Condition::NC);
        break;
    case 0xD5:
        instruction_push(m_de);
        break;
    case 0xD6:
        instruction_sub();
        break;
    case 0xD8:
        instruction_ret(opcode);
        break;
    case 0xDA:
        instruction_jp(Condition::C);
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
    case 0xE6:
        instruction_and();
        break;
    case 0xE8:
        instruction_add_sp();
        break;
    case 0xE9:
        instruction_jp(m_hl);
        break;
    case 0xEA:
        instruction_ld_reg_to_addr(m_a);
        break;
    case 0xEE:
        instruction_xor();
        break;
    case 0xF0:
        instruction_ldh_memory_to_a();
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
    case 0xF6:
        instruction_or();
        break;
    case 0xF8:
        instruction_ld_hl_sp();
        break;
    case 0xF9:
        instruction_ld_sp_hl();
        break;
    case 0xFA:
        instruction_ld_addr_to_reg(m_a);
        break;
    case 0xFB:
        instruction_ei();
        break;
    case 0xFE:
        instruction_cp();
        break;
    default:
        outln("OPCODE NOT IMPLEMENTED: {:X}", opcode);
        exit(1);
    }
}

void CPU::instruction_cb()
{
    auto instruction = CPU::fetch_byte();

    switch (instruction) {
    case 0x18:
        instruction_rr(m_b);
        break;
    case 0x19:
        instruction_rr(m_c);
        break;
    case 0x1A:
        instruction_rr(m_d);
        break;
    case 0x1B:
        instruction_rr(m_e);
        break;
    case 0x1C:
        instruction_rr(m_h);
        break;
    case 0x1D:
        instruction_rr(m_l);
        break;
    case 0x1E:
        instruction_rr(m_hl);
        break;
    case 0x1F:
        instruction_rr(m_f);
        break;
    case 0x30:
        instruction_swap(m_b);
        break;
    case 0x31:
        instruction_swap(m_c);
        break;
    case 0x32:
        instruction_swap(m_d);
        break;
    case 0x33:
        instruction_swap(m_e);
        break;
    case 0x34:
        instruction_swap(m_h);
        break;
    case 0x35:
        instruction_swap(m_l);
        break;
    case 0x36:
        instruction_swap(m_hl);
        break;
    case 0x37:
        instruction_swap(m_a);
        break;
    case 0x38:
        instruction_srl(m_b);
        break;
    case 0x39:
        instruction_srl(m_c);
        break;
    case 0x3A:
        instruction_srl(m_d);
        break;
    case 0x3B:
        instruction_srl(m_e);
        break;
    case 0x3C:
        instruction_srl(m_h);
        break;
    case 0x3D:
        instruction_srl(m_l);
        break;
    case 0x3E:
        instruction_srl(m_hl);
        break;
    case 0x3F:
        instruction_srl(m_a);
        break;
    default:
        outln("EXTERNAL OPCODE NOT IMPLEMENTED: 0xCB {:X}", instruction);
        exit(1);
    }
}
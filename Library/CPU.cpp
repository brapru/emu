#include <CPU.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

CPU::CPU(MMU& mmu, Serial& serial)
    : m_mmu(mmu)
    , m_serial(serial)
    , m_af(m_a, m_f)
    , m_bc(m_b, m_c)
    , m_de(m_d, m_e)
    , m_hl(m_h, m_l)
{
    m_f.set_zero_flag(true);
    if (m_mmu.valid_cartridge()) {
        m_f.set_flag_carry(true);
        m_f.set_half_carry_flag(true);
    }
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

    if (m_is_halted)
        instruction_nop();

    auto opcode_pc = m_pc.value();
    auto opcode = fetch_byte();

    out("PC: 0x{:04X}, OP: 0x{:02X} INSTR: ",
        opcode_pc,
        opcode);

    outln("SP: 0x{:02x}, AF: 0x{:02X}, BC: 0x{:02X}, DE: 0x{:02X}, HL: 0x{:02X}, Flag: {:08B}",
        m_sp.value(),
        m_af.value(),
        m_bc.value(),
        m_de.value(),
        m_hl.value(),
        m_f.value());

    execute_instruction(opcode);

    m_serial.update();
    m_serial.print();

    if (m_interrupt_master_enable) {
        handle_interrupts();
    }
}

void CPU::handle_interrupts()
{
    uint8_t active_interrupt = has_interrupts();
    if (!active_interrupt)
        return;

    m_interrupt_master_enable = false;
    m_is_halted = false;

    stack_push(m_pc);

    if (checkbit(active_interrupt, 0)) {
        m_pc.set(Interrupts::VBLANK);
        bitclear(active_interrupt, 0);
        return;
    }

    if (checkbit(active_interrupt, 1)) {
        m_pc.set(Interrupts::LCD_STATUS);
        bitclear(active_interrupt, 1);
        return;
    }

    if (checkbit(active_interrupt, 2)) {
        m_pc.set(Interrupts::TIMER);
        bitclear(active_interrupt, 2);
        return;
    }

    if (checkbit(active_interrupt, 3)) {
        m_pc.set(Interrupts::SERIAL);
        bitclear(active_interrupt, 3);
        return;
    }

    if (checkbit(active_interrupt, 4)) {
        m_pc.set(Interrupts::JOYPAD);
        bitclear(active_interrupt, 4);
        return;
    }
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

void CPU::stack_push(WholeRegister& reg)
{
    m_sp.decrement();
    m_mmu.write(m_sp.value(), reg.hi());
    m_sp.decrement();
    m_mmu.write(m_sp.value(), reg.lo());
}

void CPU::stack_push(WordRegister& reg)
{
    m_sp.decrement();
    m_mmu.write(m_sp.value(), reg.hi());
    m_sp.decrement();
    m_mmu.write(m_sp.value(), reg.lo());
}

uint16_t CPU::stack_pop()
{
    auto lo = m_mmu.read(m_sp.value());
    m_sp.increment();
    auto hi = m_mmu.read(m_sp.value());
    m_sp.increment();

    return (hi << 8) | (lo & 0xFF);
}
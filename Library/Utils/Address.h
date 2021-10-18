inline bool address_in_range(uint16_t const address, uint16_t const min, uint16_t const max)
{
    return (address >= min && address <= max);
}
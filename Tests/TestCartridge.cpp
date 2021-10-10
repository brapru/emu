#include <gtest/gtest.h>

#include <Cartridge.h>

TEST(CartridgeTest, TestEmptyCartridge)
{
    ASSERT_FALSE(Cartridge::the().is_loaded());
}

TEST(CartridgeTest, TestLoadCartridgeFail)
{
    Cartridge::the().load_rom_file("Tests/Roms/nonexistent.gb");
    ASSERT_FALSE(Cartridge::the().is_loaded());
}

TEST(CartridgeTest, TestLoadCartridge)
{
    Cartridge::the().load_rom_file("Tests/Roms/cpu_instrs.gb");

    ASSERT_TRUE(Cartridge::the().is_loaded());
    EXPECT_EQ("CPU_INSTRS", Cartridge::the().title());
}
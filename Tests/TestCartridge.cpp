#include <gtest/gtest.h>

#include <Cartridge.h>

class CartridgeTest : public ::testing::Test {
protected:
    Cartridge cart_bad { "Tests/Roms/nonexistent.gb" };
    Cartridge cart_good { "Tests/Roms/cpu_instrs.gb" };
};

TEST_F(CartridgeTest, TestLoadCartridgeFail)
{
    ASSERT_FALSE(cart_bad.is_loaded());
}

TEST_F(CartridgeTest, TestLoadCartridge)
{
    ASSERT_TRUE(cart_good.is_loaded());
    EXPECT_EQ("CPU_INSTRS", cart_good.title());
}
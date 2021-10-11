#include <gtest/gtest.h>

#include <Cartridge.h>

class CartridgeTest : public ::testing::Test {
protected:
    Cartridge cart_good { "Tests/Roms/cpu_instrs.gb" };
};

TEST_F(CartridgeTest, TestLoadCartridge)
{
    ASSERT_TRUE(cart_good.is_loaded());
    EXPECT_EQ("CPU_INSTRS", cart_good.title());
}
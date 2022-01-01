#include <gtest/gtest.h>
#include <memory>

#include <Cartridge.h>
#include <Utils/File.h>

class CartridgeTest : public ::testing::Test {
protected:
    std::vector<uint8_t> rom_data_good = read_rom_into_vector("Tests/Roms/cpu_instrs.gb");
    std::shared_ptr<Cartridge> cart_good = std::make_shared<NoMBC>(rom_data_good);
};

TEST_F(CartridgeTest, TestLoadCartridge)
{
    ASSERT_TRUE(cart_good->is_loaded());
    EXPECT_EQ("CPU_INSTRS", reinterpret_cast<const char*>(cart_good->header().title));
}

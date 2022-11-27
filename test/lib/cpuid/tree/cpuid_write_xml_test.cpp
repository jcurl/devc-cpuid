#include <gtest/gtest.h>

#include "cpuid/tree/cpuid_write_xml.h"

namespace rjcp::cpuid::tree {

TEST(CpuIdXmlOutputWriter, WriteEmpty)
{
    std::stringstream genxml{};

    CpuIdTree tree{};
    WriteCpuIdXml(tree, genxml);

    std::stringstream expected{};
    expected << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl;
    expected << R"(<cpuid type="x86">)" << std::endl;
    expected << "</cpuid>" << std::endl;
    ASSERT_EQ(genxml.str(), expected.str());
}

TEST(CpuIdXmlWriter, SingleProcessorSingleRegister)
{
    std::stringstream genxml{};

    CpuIdTree tree{};
    CpuIdProcessor processor{};
    processor.AddLeaf(CpuIdRegister{0, 0, 0x00000016, 0x756E6547, 0x6C65746E, 0x49656E69});
    tree.SetProcessor(0, processor);
    WriteCpuIdXml(tree, genxml);

    std::stringstream expected{};
    expected << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl;
    expected << R"(<cpuid type="x86">)" << std::endl;
    expected << "  <processor>" << std::endl;
    expected << "    " R"(<register eax="00000000" ecx="00000000">00000016,756E6547,6C65746E,49656E69</register>)" << std::endl;
    expected << "  </processor>" << std::endl;
    expected << "</cpuid>" << std::endl;
    ASSERT_EQ(genxml.str(), expected.str());
}

}

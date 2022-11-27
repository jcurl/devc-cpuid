#include "cpuid/tree/cpuid_write_xml.h"

#include <array>

namespace rjcp::cpuid::tree {

class hex final
{
public:
    hex(std::uint32_t value) : m_value{value} {}
    auto operator()(std::ostream& stream) -> std::ostream&
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index) - We know it cannot go out of bounds
        stream << chex[(m_value >> 24) & 0xFF];
        stream << chex[(m_value >> 16) & 0xFF];
        stream << chex[(m_value >> 8) & 0xFF];
        stream << chex[m_value & 0xFF];
        // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
        return stream;
    }

private:
    std::uint32_t m_value;

    static constexpr std::array<const char *, 256> chex {
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
        "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
        "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
        "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
        "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
        "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
        "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
        "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
        "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
        "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
        "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
    };
};

auto operator<<(std::ostream &out, hex number) -> std::ostream&
{
    return number(out);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void WriteCpuIdXmlRegister(std::ostream& stream, std::uint32_t eax, std::uint32_t ecx, std::uint32_t oeax, std::uint32_t oebx, std::uint32_t oecx, std::uint32_t oedx)
{
    stream << "    <register ";
    stream << "eax=\"" << hex(eax) << "\" ecx=\"" << hex(ecx) << "\">";
    stream << hex(oeax) << "," << hex(oebx) << "," << hex(oecx) << "," << hex(oedx);
    stream << "</register>" << std::endl;
}

void WriteCpuIdXmlProcessor(CpuIdProcessor& processor, std::ostream& stream)
{
    stream << "  <processor>" << std::endl;

    for (auto& cpureg : processor) {
        WriteCpuIdXmlRegister(stream,
            cpureg.second.InEax(), cpureg.second.InEcx(),
            cpureg.second.Eax(), cpureg.second.Ebx(), cpureg.second.Ecx(), cpureg.second.Edx());
    }

    stream << "  </processor>" << std::endl;
}

void WriteCpuIdXml(CpuIdTree& tree, std::ostream& stream)
{
    stream << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl;
    stream << R"(<cpuid type="x86">)" << std::endl;

    for (auto& processor : tree) {
        WriteCpuIdXmlProcessor(processor.second, stream);
    }

    stream << "</cpuid>" << std::endl;
}

}

#include "cpuid/get_cpuid.h"

namespace rjcp::cpuid {

enum class CpuType
{
    VENDOR_INTEL,
    VENDOR_AMD,
    VENDOR_UNKNOWN
};

void GetCpuIdStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor);
void GetCpuIdIntelStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor);
void GetCpuIdIntelExtended(ICpuId& cpuid, tree::CpuIdProcessor& processor);
void GetCpuIdHypervisor(ICpuId& cpuid, tree::CpuIdProcessor& processor);
void GetCpuIdRegion(ICpuId& cpuid, tree::CpuIdProcessor& processor, std::uint32_t region);

auto GetCpuId(ICpuIdFactory& factory) -> std::unique_ptr<tree::CpuIdTree>
{
    auto tree = std::make_unique<tree::CpuIdTree>();

    unsigned int threads = factory.threads();

    for (unsigned int cpunum = 0; cpunum < threads; cpunum++) {
        auto cpuid = factory.create(cpunum);
        if (!cpuid) return tree;

        auto reg = cpuid->GetCpuId(0, 0);
        tree::CpuIdProcessor processor{};
        if (!reg.IsValid()) {
            // Couldn't get the CPU, so show that it is empty.
            tree->SetProcessor(cpunum, std::move(processor));
            continue;
        }
        processor.AddLeaf(reg);

        CpuType type = CpuType::VENDOR_UNKNOWN;
        if (reg.Ebx() == 0x756E6547 && reg.Ecx() == 0x6C65746E && reg.Edx() == 0x49656E69) {
            // GenuineIntel
            type = CpuType::VENDOR_INTEL;
        } else if (reg.Ebx() == 0x68747541 && reg.Ecx() == 0x444d4163 && reg.Edx() == 0x69746E65) {
            // AuthenticAMD
            type = CpuType::VENDOR_AMD;
        }

        switch (type) {
        case CpuType::VENDOR_INTEL:
        case CpuType::VENDOR_AMD:
            GetCpuIdIntelStandard(*cpuid, processor);
            GetCpuIdIntelExtended(*cpuid, processor);
            GetCpuIdHypervisor(*cpuid, processor);
            break;
        default:
            GetCpuIdStandard(*cpuid, processor);
            break;
        }

        tree->SetProcessor(cpunum, std::move(processor));
    }

    return tree;
}

/**
 * @brief Get the CpuId Registers for Intel and AMD.
 *
 * When comparing Intel with AMD, they don't overlap, so while the bits have
 * different meanings in some cases, the same algorithm works for both.
 *
 * @param cpuid The CPUID object to get the CPUID information.
 * @param processor The tree to put the CPUID information after the query.
 */
void GetCpuIdIntelStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    const CpuIdRegister* reg0 = processor.GetLeaf(0, 0);
    if (reg0 == nullptr) return;

    std::uint32_t leafs = reg0->Eax();
    std::uint32_t leaf = 1;
    bool sgx = false;
    while (leaf <= leafs && leaf <= 0xFFFF) {
        switch (leaf) {
        case 4: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid()) {
                processor.AddLeaf(reg);

                std::uint32_t subleaf = 1;
                while (subleaf < 0xFF && reg.IsValid() && (reg.Eax() & 0x0000001F)) {
                    reg = cpuid.GetCpuId(leaf, subleaf);
                    if (reg.IsValid())
                        processor.AddLeaf(reg);
                    subleaf++;
                }
            }
            break;
        }
        case 7: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid()) {
                processor.AddLeaf(reg);
                sgx = reg.Ebx() & 0x00000004;

                std::uint32_t features = reg.Eax();
                for (std::uint32_t i = 1; i <= features; i++) {
                    reg = cpuid.GetCpuId(leaf, i);
                    if (reg.IsValid())
                        processor.AddLeaf(reg);
                }
            }
            break;
        }
        case 11:
        case 31: {
            CpuIdRegister reg;
            std::uint32_t subleaf = 0;
            do {
                reg = cpuid.GetCpuId(leaf, subleaf);
                if (reg.IsValid())
                    processor.AddLeaf(reg);
                subleaf++;
            } while(subleaf < 0xFF && reg.IsValid() && (reg.Ebx() & 0x0000FFFF));
            break;
        }
        case 13: {
            CpuIdRegister reg;
            for (std::uint32_t i = 0; i < 64; i++) {
                reg = cpuid.GetCpuId(leaf, i);
                if (reg.IsValid() &&
                  (i <= 2 || reg.Eax() || reg.Ebx() || reg.Ecx() || reg.Edx()))
                    processor.AddLeaf(reg);
            }
            break;
        }
        case 15: {
            for (std::uint32_t i = 0; i < 2; i++) {
                CpuIdRegister reg = cpuid.GetCpuId(leaf, i);
                if (reg.IsValid())
                    processor.AddLeaf(reg);
            }
            break;
        }
        case 16: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid()) {
                processor.AddLeaf(reg);

                std::uint32_t resid = 1;
                std::uint32_t residbit = reg.Ebx() >> 1;
                while (residbit) {
                    reg = cpuid.GetCpuId(leaf, resid);
                    if (reg.IsValid())
                        processor.AddLeaf(reg);
                    residbit >>= 1;
                    resid++;
                }
            }
            break;
        }
        case 18: {
            CpuIdRegister reg0 = cpuid.GetCpuId(leaf, 0);
            if (reg0.IsValid())
                processor.AddLeaf(reg0);

            CpuIdRegister reg1 = cpuid.GetCpuId(leaf, 1);
            if (reg1.IsValid())
                processor.AddLeaf(reg1);

            if (sgx) {
                CpuIdRegister reg;
                std::uint32_t subleaf = 2;
                do {
                    reg = cpuid.GetCpuId(leaf, subleaf);
                    if (reg.IsValid())
                        processor.AddLeaf(reg);
                    subleaf++;
                    if (subleaf > 0xFF || !reg.IsValid() || (reg.Eax() & 0x0000000F) == 0)
                        subleaf = 0;
                } while (subleaf > 0);
            }
            break;
        }
        case 20:
        case 23:
        case 24:
        case 32: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid()) {
                processor.AddLeaf(reg);

                std::uint32_t subleafs = reg.Eax();
                for (std::uint32_t i = 1; i <= subleafs; i++) {
                    reg = cpuid.GetCpuId(leaf, i);
                    if (reg.IsValid())
                        processor.AddLeaf(reg);
                }
            }
            break;
        }
        default: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid())
                processor.AddLeaf(reg);
            break;
        }
        }
        leaf++;
    }
}

void GetCpuIdStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    GetCpuIdRegion(cpuid, processor, 0x00000000);
}

/**
 * @brief Get the CpuId Extended Registers for Intel and AMD.
 *
 * When comparing Intel with AMD, they don't overlap, so while the bits have
 * different meanings in some cases, the same algorithm works for both.
 *
 * @param cpuid The CPUID object to get the CPUID information.
 * @param processor The tree to put the CPUID information after the query.
 */
void GetCpuIdIntelExtended(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    CpuIdRegister reg0 = cpuid.GetCpuId(0x80000000, 0);
    if (!reg0.IsValid()) return;
    processor.AddLeaf(reg0);

    std::uint32_t leafs = reg0.Eax();
    std::uint32_t leaf = 0x80000001;
    while (leaf <= leafs && leaf <= 0x8000FFFF) {
        switch (leaf) {
        case 0x8000001D: {
            // AMD Specification, Volume 3, CPUID instruction.
            CpuIdRegister reg;
            std::uint32_t subleaf = 0;
            do {
                reg = cpuid.GetCpuId(leaf, subleaf);
                if (reg.IsValid())
                    processor.AddLeaf(reg);
                subleaf++;
            } while(subleaf < 0xFF && reg.IsValid() && (reg.Eax() & 0x0000000F) != 0);
            break;
        }
        case 0x80000026: {
            // AMD Specification, Volume 3, CPUID instruction.
            CpuIdRegister reg;
            std::uint32_t subleaf = 0;
            do {
                reg = cpuid.GetCpuId(leaf, subleaf);
                if (reg.IsValid())
                    processor.AddLeaf(reg);
                subleaf++;
            } while(subleaf < 0xFF && reg.IsValid() && (reg.Ecx() & 0x0000FF00));
            break;
        }
        default: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            if (reg.IsValid())
                processor.AddLeaf(reg);
            break;
        }
        }
        leaf++;
    }
}

void GetCpuIdHypervisor(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    const CpuIdRegister* reg0 = processor.GetLeaf(0, 0);
    if (reg0 == nullptr) return;

    const CpuIdRegister* reg1 = processor.GetLeaf(1, 0);
    if (reg1 == nullptr) return;
    if ((reg1->Ecx() & 0x80000000) != 0) {
        GetCpuIdRegion(cpuid, processor, 0x40000000);
    }
}

void GetCpuIdRegion(ICpuId& cpuid, tree::CpuIdProcessor& processor, std::uint32_t region)
{
    CpuIdRegister reg0 = cpuid.GetCpuId(region, 0);
    if (!reg0.IsValid()) return;

    processor.AddLeaf(reg0);
    std::uint32_t leafs = reg0.Eax();
    std::uint32_t leaf = region + 1;
    while (leaf <= leafs) {
        processor.AddLeaf(cpuid.GetCpuId(leaf, 0));
        leaf++;
    }
}

}

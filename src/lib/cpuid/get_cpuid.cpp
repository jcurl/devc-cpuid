#include "cpuid/get_cpuid.h"

#include <thread>

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

    unsigned int threads = std::thread::hardware_concurrency();

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
        } else if (reg.Ebx() == 0x68747541 && reg.Ecx() == 0x444d4163 && reg.Edx() == 0x444d4163) {
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

void GetCpuIdIntelStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    const CpuIdRegister* reg0 = processor.GetLeaf(0, 0);
    if (reg0 == nullptr) return;

    std::uint32_t leafs = reg0->Eax();
    std::uint32_t leaf = 1;
    bool sgx = false;
    while (leaf <= leafs) {
        switch (leaf) {
        case 4: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            processor.AddLeaf(reg);

            unsigned int subleaf = 1;
            while (reg.Eax() & 0x0000001F) {
                reg = cpuid.GetCpuId(leaf, subleaf);
                processor.AddLeaf(reg);
                subleaf++;
            };
            break;
        }
        case 7: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            processor.AddLeaf(reg);
            sgx = reg.Ebx() & 0x00000004;

            unsigned int features = reg.Eax();
            for (unsigned int i = 1; i <= features; i++) {
                processor.AddLeaf(cpuid.GetCpuId(7, i));
            }
            break;
        }
        case 11:
        case 31: {
            CpuIdRegister reg;
            unsigned int subleaf = 0;
            do {
                reg = cpuid.GetCpuId(leaf, subleaf);
                processor.AddLeaf(reg);
                subleaf++;
            } while(reg.Ebx() & 0x0000FFFF);
            break;
        }
        case 13: {
            CpuIdRegister reg;
            unsigned int subleaf = 0;
            do {
                reg = cpuid.GetCpuId(leaf, subleaf);
                processor.AddLeaf(reg);
                subleaf++;
            } while(subleaf <= 2 || (reg.Eax() || reg.Ebx() || reg.Ecx() || reg.Edx()));
            break;
        }
        case 15: {
            for (unsigned int i = 0; i < 2; i++) {
                processor.AddLeaf(cpuid.GetCpuId(leaf, i));
            }
            break;
        }
        case 16: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            processor.AddLeaf(reg);

            unsigned int resid = 1;
            unsigned int residbit = reg.Ebx() >> 1;
            while (residbit) {
                processor.AddLeaf(cpuid.GetCpuId(leaf, resid));
                residbit >>= 1;
                resid++;
            }
            break;
        }
        case 18: {
            processor.AddLeaf(cpuid.GetCpuId(leaf, 0));
            processor.AddLeaf(cpuid.GetCpuId(leaf, 1));

            if (sgx) {
                CpuIdRegister reg;
                unsigned int subleaf = 2;
                do {
                    reg = cpuid.GetCpuId(leaf, subleaf);
                    processor.AddLeaf(reg);
                    subleaf++;
                    if ((reg.Eax() & 0x0000000F) == 0) subleaf = 0;
                } while (subleaf > 0);
            }
            break;
        }
        case 20: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            processor.AddLeaf(reg);

            unsigned int subleafs = reg.Eax();
            for (unsigned int i = 1; i < subleafs; i++) {
                processor.AddLeaf(cpuid.GetCpuId(leaf, i));
            }
            break;
        }
        case 23:
        case 24:
        case 32: {
            CpuIdRegister reg = cpuid.GetCpuId(leaf, 0);
            processor.AddLeaf(reg);

            unsigned int subleafs = reg.Eax();
            for (unsigned int i = 1; i <= subleafs; i++) {
                processor.AddLeaf(cpuid.GetCpuId(leaf, i));
            }
            break;
        }
        default:
            processor.AddLeaf(cpuid.GetCpuId(leaf, 0));
            break;
        }
        leaf++;
    }
}

void GetCpuIdStandard(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    GetCpuIdRegion(cpuid, processor, 0x00000000);
}

void GetCpuIdIntelExtended(ICpuId& cpuid, tree::CpuIdProcessor& processor)
{
    GetCpuIdRegion(cpuid, processor, 0x80000000);
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
    processor.AddLeaf(reg0);
    std::uint32_t leafs = reg0.Eax();
    std::uint32_t leaf = region + 1;
    while (leaf <= leafs) {
        processor.AddLeaf(cpuid.GetCpuId(leaf, 0));
        leaf++;
    }
}

}

#include "cpuid/get_cpuid.h"
#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_native_config.h"
#include "cpuid/tree/cpuid_write_xml.h"

#include <iostream>

auto main() -> int
{
    auto factory = rjcp::cpuid::CreateCpuIdFactory(rjcp::cpuid::CpuIdNativeConfig{});
    auto cpu = rjcp::cpuid::GetCpuId(*factory);
    rjcp::cpuid::tree::WriteCpuIdXml(*cpu, std::cout);
    return 0;
}

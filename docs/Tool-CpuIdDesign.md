# CPUID Tool for Dumping CpuId Information <!-- omit in toc -->

The tool is designed to dump the CPUID information to the console in XML format,
that is compatible with [CpuIdWin](https://github.com/jcurl/RJCP.DLL.CpuId/). It
has a secondary purpose to test the device API `/dev/cpu/N/cpuid` resource
manager, implemented by this repository, for QNX 7.1.0.

- [1. High Level Requirements](#1-high-level-requirements)
- [2. Design](#2-design)

## 1. High Level Requirements

The tool is intended to be very simple, read the data given by the CPUID
instruction and perform minimal interpretation. By *minimal*, this implies
enough to know how to read the CPUID instructions, but not interpret the
contents any further.

It should have the ability, via the command line, to choose how to read the
CPUID instructions. This will map to the factories given by the template
`CpuIdFactory<T>`.

## 2. Design

The main entry point is the `main` function.

It does the following:

* Gets a `ICpuIdFactory` from a configuration, either:
  * `CpuIdNativeConfig` (via the CPUID insruction) or
  * `CpuIdDeviceConfig` (via the device `/dev/cpu/N/cpuid`).
  * to the factory function `rjcp::cpuid::CreateCpuIdFactory`
* Gets the CPUID dump, from the free function `rjcp::cpuid::GetCpuId`
* Writes the output to a stream, in this case `std::cout`, with
  `rjcp::cpuid::tree::WriteCpuIdXml`

The output of this file can then be loaded using other tools, such as
[RJCP.CpuId](https://github.com/jcurl/RJCP.DLL.CpuId/).

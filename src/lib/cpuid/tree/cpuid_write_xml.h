#ifndef RJCP_LIB_CPUID_TREE_CPUID_WRITE_XML_H
#define RJCP_LIB_CPUID_TREE_CPUID_WRITE_XML_H

#include "cpuid/tree/cpuid_tree.h"

#include <iostream>

namespace rjcp::cpuid::tree {

/**
 * @brief Writes the CPUID tree to the stream provided.
 *
 * @param tree The CPUID data to write.
 * @param stream The stream to write the data to.
 * @return true The data was properly written to the stream.
 * @return false The data was not properly written to the stream. It might be in
 * an inconsistent state.
 */
void WriteCpuIdXml(CpuIdTree& tree, std::ostream& stream);

}

#endif

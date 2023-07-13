#include <cstring>
#include "binaryninjacore.h"
#include "binaryninjaapi.h"

using namespace BinaryNinja;

static BNArchitecture *arch = NULL;

extern "C" bool arch_supported_default_impl(const char *arch_name) {
    const char *binja_arch;
    if (!strcmp(arch_name, "arm")) {
        binja_arch = "armv7";
    } else if (!strcmp(arch_name, "x86_64")) {
        binja_arch = arch_name;
    } else {
        return false;
    }
    BNSetBundledPluginDirectory(BINJA_PLUGIN_DIR);
    BNInitPlugins(true);
    arch = BNGetArchitectureByName(binja_arch);
    return arch;
}

extern "C" bool is_indirect_branch_default_impl(uint8_t *insn_data, size_t insn_size) {
    size_t ttCount;
    BNInstructionTextToken* ttResult = NULL;
    BNGetInstructionText(arch, (const uint8_t*) insn_data, 0 /*addr*/, &insn_size, &ttResult, &ttCount);
    std::string call_op = "call";

    auto opcode = ttResult[0];
    // auto padding = ttResult[1];
    auto first_operand = ttResult[2];
    if ((call_op.compare(opcode.text) == 0) && (first_operand.type != BNInstructionTextTokenType::CodeRelativeAddressToken)){
        return true;
    }

    //BNInstructionInfo info;
    //BNGetInstructionInfo(arch, insn_data, 0, insn_size, &info);
    //for (int i = 0; i < info.branchCount; i++) {
    //    BNBranchType br = info.branchType[i];
    //    if ((br == BNBranchType::IndirectBranch) || (br == BNBranchType::UnresolvedBranch)) {
    //        return true;
    //    }
    //}
    return false;
}

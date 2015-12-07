//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015 Technical University of Kaiserslautern.

#pragma once

#include "Fragment.h"
#include "BasicBlock.h"
#include <vector>

namespace disasm {

enum class MaxBlockType{
    kData,
    kMaybe,
    kCode
};

/**
 * MaximalBlock
 */
class MaximalBlock {
public:
    /**
     */
    virtual ~MaximalBlock() = default;
    MaximalBlock(const MaximalBlock &src) = default;
    MaximalBlock &operator=(const MaximalBlock &src) = default;
    MaximalBlock(MaximalBlock &&src) = default;

    friend class MaximalBlockBuilder;

    /**
     * MB is valid when all of its BBs are valid. A BB is valid when it
     * has a branch as last instruction.
     */
    bool valid() const;
    addr_t startAddr() const;
    void setType(const MaxBlockType type);

    const BasicBlock& getBasicBlockById(const unsigned bb_id) const;
    const std::vector<BasicBlock>& getBasicBlocks() const;
    // getting size and memsize of getFragments are provided by the fragment itself.
    // providing the same for BBs, however, requires MB intervention!
    size_t getBasicBlockMemSize(const unsigned int bb_id) const;

    unsigned getBasicBlocksCount() const;
    unsigned getFragmentsCount() const;
    unsigned getInstructionCount() const;

    const std::vector<MCInstSmall*> getInstructions(BasicBlock& bblock);

    //XXX: access should be to an iterator instead of a collection?
    const std::vector<Fragment>& getFragments() const;
    const std::vector<MCInstSmall> &getInstructions() const {
        return m_insts;
    }

    const unsigned & id() const ;

private:
    explicit MaximalBlock(unsigned int id);

private:
    unsigned int m_id;
    MaxBlockType m_type;
    BranchInstType m_br_type;
    // contains a valid value only in the case of a direct branch
    addr_t m_br_target;
    std::vector<Fragment> m_frags;
    std::vector<MCInstSmall> m_insts;
    std::vector<BasicBlock> m_bblocks;
};
}

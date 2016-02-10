//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2016 University of Kaiserslautern.

#pragma once
#include "disasm/common.h"
#include "disasm/MaximalBlock.h"
#include <functional>

namespace disasm {
/**
 * BlockCFGNode
 */
class BlockCFGNode {
public:
    /**
     * Construct a BlockCFGNode that is initially not valid.  Calling
     * methods other than operator= on this results in
     * undefined behavior.
     */
    BlockCFGNode();
    BlockCFGNode(MaximalBlock *current_block);
    virtual ~BlockCFGNode() = default;
    BlockCFGNode(const BlockCFGNode &src) = default;
    BlockCFGNode &operator=(const BlockCFGNode &src) = default;
    BlockCFGNode(BlockCFGNode &&src) = default;

    const MaximalBlock * getMaximalBlock() const;
    const BlockCFGNode *getOverlapNode() const;
    unsigned int id() const noexcept;

    void addPredecessor(BlockCFGNode *predecessor, addr_t target_addr);

    void setDirectSuccessor(BlockCFGNode *successor);
    const BlockCFGNode *getDirectSuccessor() const;
    void setRemoteSuccessor(BlockCFGNode *successor);
    const BlockCFGNode *getRemoteSuccessor() const;

    const std::vector<std::pair<BlockCFGNode *, addr_t>> &
        getPredecessors() const;
    bool hasOverlapWithOtherNode() const noexcept;
    const BasicBlock * getValidBasicBlock() const noexcept;

    /*
     * return the sequence of instructions in valid basic block starting from
     * the candidate start address. Throws exception in case valid basic block not set.
     */
    std::vector<const MCInst *> getCandidateInstructions() const;
    std::vector<const MCInst *> getCandidateInstructionsSatisfying
        (std::function<bool(const MCInst *inst)> predicate) const;
    addr_t getCandidateStartAddr() const noexcept;
    void setCandidateStartAddr(addr_t candidate_start) noexcept;
    void setType(const MaximalBlockType type);
    MaximalBlockType getType() const;
    bool isData() const;
    bool isCode() const;
    bool isValidBasicBlockSet() const noexcept;
    bool givenCandidateStartAddressIsValid() const noexcept;
    /*
     * Candidate start adjust to address of the first instruction in the valid BB
     * that is bigger than it.
     * precondition: valid basic block must be set
     */
    void adjustCandidateStartAddress() noexcept;
    friend class SectionDisassemblyAnalyzer;
private:
    void setMaximalBlock(MaximalBlock *maximal_block) noexcept;
    BlockCFGNode *getOverlapNodePtr() const;
private:
    MaximalBlockType m_type;
    addr_t m_candidate_start_addr;
    BasicBlock *m_valid_basic_block_ptr;
    BlockCFGNode *m_overlap_node;
    /// valid only in case of conditional branch
    BlockCFGNode *m_direct_successor;
    BlockCFGNode *m_remote_successor;
    MaximalBlock *m_max_block;
    std::vector<std::pair<BlockCFGNode *, addr_t>> m_predecessors;
};
}

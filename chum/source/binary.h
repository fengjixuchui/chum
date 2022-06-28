#pragma once

#include "block.h"
#include "symbol.h"

#include <vector>
#include <Windows.h>

#include <Zydis/Zydis.h>

namespace chum {

// This is a database that contains the code and data that makes up an
// x86-64 binary.
class binary {
public:
  // Create an empty binary.
  binary();

  // Print the contents of this binary, for debugging purposes.
  void print();

  // Create a new basic block that contains no instructions.
  basic_block* create_basic_block();

  // Create a new data block with uninitialized data.
  data_block* create_data_block(std::uint32_t size, std::uint32_t alignment = 1);

  // Create a new symbol.
  symbol* create_symbol(symbol_type type, char const* name = nullptr);

private:
  ZydisDecoder decoder_;
  ZydisFormatter formatter_;

  // Every symbol that makes up this binary. These are accessed with symbol IDs.
  std::vector<symbol*> symbols_;

  // Every piece of data that makes up this binary.
  std::vector<data_block*> data_blocks_;

  // Every piece of code that makes up this binary.
  std::vector<basic_block*> basic_blocks_;
};

} // namespace chum

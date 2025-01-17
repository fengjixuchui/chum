#include "chum.h"

#include <algorithm>
#include <random>

// Insert a NOP before every instruction.
void insert_nops(chum::binary& bin) {
  for (auto const bb : bin.basic_blocks()) {
    for (std::size_t i = bb->instructions.size(); i > 0; --i)
      bb->insert(bin.instr("\x90"), i - 1);
  }
}

// Add a call at the start of every basic block to an instrumentation function.
void instrument(chum::binary& bin) {
  // Create a basic block.
  auto const block = bin.create_basic_block();
  block->push(bin.instr("\x90")); // NOP
  block->push(bin.instr("\xC3")); // RET

  for (auto const bb : bin.basic_blocks()) {
    if (bb == block)
      continue;

    // All memory references are treated as symbol IDs.
    // CALL block
    bb->insert(bin.instr("\xE8", block));
  }
}

// Shuffle the order of every basic block in the binary.
void shuffle_blocks(chum::binary& bin) {
  auto rd = std::random_device{};
  auto rng = std::default_random_engine{ rd() };
  std::shuffle(std::begin(bin.basic_blocks()), std::end(bin.basic_blocks()), rng);
}

int main() {
  auto bin = chum::disassemble("hello-world-x64.dll");
  if (!bin)
    return 0;

  insert_nops(*bin);
  instrument(*bin);
  shuffle_blocks(*bin);

  bin->print();
  bin->create("output.dll");
}


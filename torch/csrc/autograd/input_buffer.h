#pragma once

// The InputBuffer class accumulates a list of Variables for use by a
// function. It implements logic to avoid modifying the passed
// values in-place (adding an input twice will accumulate the result).
// This behaviour needed and used only in backward graphs.

#include <Python.h>
#include <vector>
#include <utility>
#include <memory>
#include <ATen/ATen.h>

#include "torch/csrc/autograd/variable.h"

namespace torch { namespace autograd {

struct InputBuffer {
  explicit InputBuffer(size_t size)
    : buffer(size) {}
  InputBuffer(const InputBuffer& other) = delete;
  InputBuffer(InputBuffer&& other) = default;

  // Accumulates the variable at a specified index.
  void add(size_t idx, Variable var);

  int device() const;

  Variable operator[](std::size_t pos) { return buffer[pos]; }

  // Returns the inputs as a list of variables. Destroys given InputBuffer.
  static std::vector<Variable> variables(InputBuffer&& buffer);

private:
  std::vector<Variable> buffer;
};

}}  // namespace torch::autograd

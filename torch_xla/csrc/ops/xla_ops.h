#pragma once

#include <mutex>
#include <string>

#include "ir.h"

namespace torch_xla {
namespace ir {
namespace ops {

class OpKindWrapper {
 public:
  OpKindWrapper(const char* name) : name_(name) {}

  const OpKind& operator*() const { return get(); }

  operator OpKind() const { return get(); }

 private:
  const OpKind& get() const {
    std::call_once(once_, [this]() { op_kind_ = OpKind::Get(name_); });
    return op_kind_;
  }

  const char* name_;
  mutable OpKind op_kind_;
  mutable std::once_flag once_;
};

extern const OpKindWrapper xla_device_data;

}  // namespace ops
}  // namespace ir
}  // namespace torch_xla

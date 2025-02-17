#include "torch_xla/csrc/ops/gather.h"

#include "tensorflow/compiler/xla/client/lib/slicing.h"
#include "torch_xla/csrc/data_ops.h"
#include "torch_xla/csrc/helpers.h"
#include "torch_xla/csrc/lowering_context.h"
#include "torch_xla/csrc/ops/infer_output_shape.h"

namespace torch_xla {

Gather::Gather(const torch::lazy::Value& input, int64_t dim,
               const torch::lazy::Value& index, bool sparse_grad)
    : XlaNode(torch::lazy::OpKind(at::aten::gather), {input, index},
              xla::ShapeUtil::MakeShape(GetXlaShape(input).element_type(),
                                        GetXlaShape(index).dimensions()),
              /*num_outputs=*/1, torch::lazy::MHash(dim)),
      dim_(dim),
      sparse_grad_(sparse_grad) {}

torch::lazy::NodePtr Gather::Clone(torch::lazy::OpList operands) const {
  return torch::lazy::MakeNode<Gather>(operands.at(0), dim_, operands.at(1),
                                       sparse_grad_);
}

XlaOpVector Gather::Lower(LoweringContext* loctx) const {
  xla::XlaOp input = loctx->GetOutputOp(operand(0));
  xla::XlaOp index = loctx->GetOutputOp(operand(1));
  return ReturnOp(xla::TorchGather(input, index, dim_, sparse_grad_), loctx);
}

std::string Gather::ToString() const {
  std::stringstream ss;
  ss << XlaNode::ToString() << ", dim=" << dim_
     << ", sparse_grad=" << sparse_grad_;
  return ss.str();
}

}  // namespace torch_xla

#ifndef CLIC_INCLUDE_TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class MaximumYProjectionKernel : public Operation
{
  public:
    explicit MaximumYProjectionKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto GetOutput () -> Image;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLEMAXIMUMYPROJECTIONKERNEL_HPP

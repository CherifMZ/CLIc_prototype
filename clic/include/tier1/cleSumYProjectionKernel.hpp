

#ifndef CLIC_INCLUDE_TIER1_CLESUMYPROJECTIONKERNEL_HPP
#define CLIC_INCLUDE_TIER1_CLESUMYPROJECTIONKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class SumYProjectionKernel : public Operation
{
  public:
    explicit SumYProjectionKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER1_CLESUMYPROJECTIONKERNEL_HPP

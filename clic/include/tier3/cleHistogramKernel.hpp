
#ifndef CLIC_INCLUDE_TIER3_CLEHISTOGRAMKERNEL_HPP
#define CLIC_INCLUDE_TIER3_CLEHISTOGRAMKERNEL_HPP

#include "cleOperation.hpp"

namespace cle
{

class HistogramKernel : public Operation
{
  public:
    explicit HistogramKernel (const ProcessorPointer &device);
    auto SetInput (const Image &object) -> void;
    auto SetOutput (const Image &object) -> void;
    auto SetMinimumIntensity (const float &value) -> void;
    auto SetMaximumIntensity (const float &value) -> void;
    auto SetSteps (const int &step_x = 1, const int &step_y = 1, const int &step_z = 1) -> void;
    auto SetNumBins (const unsigned int &bin = 256) -> void;
    auto Execute () -> void override;

  private:
    float min_intensity_ = std::numeric_limits<float>::infinity ();
    float max_intensity_ = std::numeric_limits<float>::infinity ();
};

} // namespace cle

#endif // CLIC_INCLUDE_TIER3_CLEHISTOGRAMKERNEL_HPP

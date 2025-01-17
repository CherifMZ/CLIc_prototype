

#include <iostream>
#include <vector>

#include "cleMemory.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"


template <class type>
auto
run_test(const std::shared_ptr<cle::Processor> & gpu, const std::array<size_t, 3> & shape) -> bool
{
  const auto    object_type = cle::IMAGE1D;
  cle::DataType data_type = cle::TypeToDataType<type>();

  const type        data = 10;
  std::vector<type> array(shape[0] * shape[1] * shape[2]);
  std::vector<type> output(shape[0] * shape[1] * shape[2]);
  std::fill(array.begin(), array.end(), data);

  auto gpu_output = cle::Memory::AllocateMemory(gpu, shape, data_type, object_type);
  auto gpu_input = cle::Memory::AllocateMemory(gpu_output);

  cle::Memory::WriteObject(gpu_input, array.data(), array.size());
  gpu_input.CopyDataTo(gpu_output);

  cle::Memory::ReadObject<type>(gpu_output, output.data(), output.size());

  return std::equal(output.begin(), output.end(), array.begin());
}

auto
main(int argc, char ** argv) -> int
{
  auto gpu = std::make_shared<cle::Processor>();
  gpu->SelectDevice();
  gpu->WaitForKernelToFinish();

  if (gpu->ImageSupport())
  {
    if (!run_test<float>(gpu, { 10, 1, 1 }))
    {
      return EXIT_FAILURE;
    }
    // if (!run_test<int32_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<int16_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<int8_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint32_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint16_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint8_t>(gpu, { 10, 1, 1 })) { return EXIT_FAILURE; }
    if (!run_test<float>(gpu, { 10, 5, 1 }))
    {
      return EXIT_FAILURE;
    }
    // if (!run_test<int32_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<int16_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<int8_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint32_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint16_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    // if (!run_test<uint8_t>(gpu, { 10, 5, 1 })) { return EXIT_FAILURE; }
    if (!run_test<float>(gpu, { 10, 5, 3 }))
    {
      return EXIT_FAILURE;
    }
    // if (!run_test<int32_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
    // if (!run_test<int16_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
    // if (!run_test<int8_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
    // if (!run_test<uint32_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
    // if (!run_test<uint16_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
    // if (!run_test<uint8_t>(gpu, { 10, 5, 3 })) { return EXIT_FAILURE; }
  }
  return EXIT_SUCCESS;
}

#include <random>

#include "clesperanto.hpp"

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(0));
  std::fill(valid.begin(), valid.end(), static_cast<type>(0));
  const int center = (shape[0] / 2) + (shape[1] / 2) * shape[0] + (shape[2] / 2) * shape[0] * shape[1];
  input[center] = static_cast<type>(1);
  valid[center] = static_cast<type>(1);
  if (shape[0] > 1)
  {
    valid[center + 1] = static_cast<type>(1);
    valid[center - 1] = static_cast<type>(1);
  }
  if (shape[1] > 1)
  {
    valid[center + shape[0]] = static_cast<type>(1);
    valid[center - shape[0]] = static_cast<type>(1);
    valid[center + shape[0] + 1] = static_cast<type>(1);
    valid[center + shape[0] - 1] = static_cast<type>(1);
    valid[center - shape[0] + 1] = static_cast<type>(1);
    valid[center - shape[0] - 1] = static_cast<type>(1);
  }
  if (shape[2] > 1)
  {
    valid[center + (shape[1] * shape[0])] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) + 1] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) - 1] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) + shape[0]] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) - shape[0]] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(1);
    valid[center + (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(1);

    valid[center - (shape[1] * shape[0])] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) + 1] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) - 1] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) + shape[0]] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) - shape[0]] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) + shape[0] + 1] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) + shape[0] - 1] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) - shape[0] + 1] = static_cast<type>(1);
    valid[center - (shape[1] * shape[0]) - shape[0] - 1] = static_cast<type>(1);
  }

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.DilateBox(gpu_input, gpu_output);
  auto output = cle.Pull<type>(gpu_output);

  if (std::equal(output.begin(), output.end(), valid.begin()) == false)
  {
    std::cout << "DilateBox test failed!" << std::endl;
    std::cout << "gpu_input: " << gpu_input << std::endl;
    std::cout << "gpu_output: " << gpu_output << std::endl;
    // print valid and output
    std::cout << "Valid: " << std::endl;
    for (size_t i = 0; i < valid.size(); i++)
    {
      std::cout << valid[i] << " ";
      if ((i + 1) % shape[0] == 0)
      {
        std::cout << std::endl;
      }
    }
    std::cout << "Output: " << std::endl;
    for (size_t i = 0; i < output.size(); i++)
    {
      std::cout << output[i] << " ";
      if ((i + 1) % shape[0] == 0)
      {
        std::cout << std::endl;
      }
    }
    return false;
  }
  return true;
}

auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 10, 7, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<float>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int32_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint32_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int16_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint16_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<int8_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<uint8_t>({ 10, 7, 5 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  // if (!run_test<float>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 10, 1, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 10, 7, 1 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<float>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int32_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint32_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int16_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint16_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<int8_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  // if (!run_test<uint8_t>({ 10, 7, 5 }, cle::IMAGE))
  // {
  // return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}
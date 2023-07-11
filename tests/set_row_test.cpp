
#include "cle.hpp"

#include <assert.h>
#include <random>

template <class type>
auto
run_test(const std::array<size_t, 3> & shape, const cle::mType & mem_type) -> bool
{
  size_t            w = shape[0];
  size_t            h = shape[1];
  size_t            d = shape[2];
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  for (auto it = input.begin(), it_valid = valid.begin(); (it != input.end()) && (it_valid != valid.end());
       ++it, ++it_valid)
  {
    *it = static_cast<type>((int)rand() % 10);
    *it_valid = *it;

    // if iterator is at the second element of the row, set the value to 100
    size_t row_index = std::floor((it - input.begin()) / w);
    row_index = row_index % (h);
    if (row_index == 1)
    {
      *it_valid = static_cast<type>(100);
    }
  }

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("TX", "all");
  auto gpu_input = cle::Array::create(shape[0], shape[1], shape[2], cle::toType<type>(), mem_type, device);
  gpu_input->write(input.data());

  cle::tier1::set_row_func(device, gpu_input, 1, 100);

  std::vector<type> output(gpu_input->nbElements());
  gpu_input->read(output.data());

  return std::equal(output.begin(), output.end(), valid.begin()) ? 0 : 1;
}

auto
main(int argc, char ** argv) -> int
{
  cle::BackendManager::getInstance().setBackend("opencl");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<float>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);

  cle::BackendManager::getInstance().setBackend("cuda");
  std::cout << cle::BackendManager::getInstance().getBackend() << " backend selected" << std::endl;
  assert(run_test<float>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::BUFFER) == 0);
  // assert(run_test<int64_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int32_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int16_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);
  // assert(run_test<int8_t>({ 10, 5, 3 }, cle::mType::IMAGE) == 0);

  return EXIT_SUCCESS;
}
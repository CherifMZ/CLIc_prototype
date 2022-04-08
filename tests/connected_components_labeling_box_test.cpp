
#include <random>

#include "clesperanto.hpp"
#include "utils.hpp"




template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    if (depth == 2)
    {
        arr_1 = {0, 0, 0, 0, 0,
                 0, 0, 0, 1, 0,
                 0, 0, 0, 0, 0,
                 1, 1, 0, 1, 0,
                 0, 1, 0, 1, 1,
                 0, 1, 0, 0, 0};
        valid = {0, 0, 0, 0, 0,
                 0, 0, 0, 1, 0,
                 0, 0, 0, 0, 0,
                 2, 2, 0, 1, 0,
                 0, 2, 0, 1, 1,
                 0, 2, 0, 0, 0};
    }
    else if (height == 3)
    {
        arr_1 = {0, 1, 0, 0, 1,
                 0, 0, 1, 0, 1,
                 0, 0, 0, 0, 1};
        valid = {0, 1, 0, 0, 2,
                 0, 0, 1, 0, 2,
                 0, 0, 0, 0, 2};
    }
    else
    {
        arr_1 = {0, 1, 1, 0, 1};
        valid = {0, 1, 1, 0, 2};
    }
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    auto ocl_output = cle.Create<type>(shape);
    cle.ConnectedComponentsLabelingBox(oclArray_A, ocl_output);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape, "image");
    auto ocl_output = cle.Create<type>(shape, "image");
    cle.ConnectedComponentsLabelingBox(oclArray_A, ocl_output);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image = run_kernel_with_image<type>(arr_1, shape);
    if (IsDifferent(output_image, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using image ... FAILED! " << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (test<float>(5, 3, 2))
    {
        std::cerr << "ConnectedComponentsLabelingBox kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(5, 3, 1))
    {
        std::cerr << "ConnectedComponentsLabelingBox kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(5, 1, 1))
    {        
        std::cerr << "ConnectedComponentsLabelingBox kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "ConnectedComponentsLabelingBox kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
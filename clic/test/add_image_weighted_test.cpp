
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (3), height (3), depth (3);
    unsigned int dims[3] = {width, height, depth};
    std::vector<float> input_data1 (width*height*depth);
    std::fill(input_data1.begin(), input_data1.end(), 10);
    std::vector<float> input_data2 (width*height*depth);
    std::fill(input_data2.begin(), input_data2.end(), 10);
    std::vector<float> valid_data (width*height*depth);
    std::fill(valid_data.begin(), valid_data.end(), 15);

    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    // Initialise device memory and push from host to device
    cle::Buffer Buffer_A = cle.Push<float>(input_data1, dims);
    cle::Buffer Buffer_B = cle.Push<float>(input_data2, dims);
    cle::Buffer Buffer_C = cle.Create<float>(dims);

    // Call kernel
    cle.AddImagesWeighted(Buffer_A, Buffer_B, Buffer_C, 1, 0.5);

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_C);  

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < width*height*depth; i++)
    {
        if(i%width==0)
        {
            std::cout << std::endl;
        }
        std::cout << output_data[i] << " ";
        difference += std::abs(valid_data[i] - output_data[i]);
    }
    std::cout << std::endl;
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failed, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}

#include <random>
#include <iostream>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    // Initialise random input and valid output.
    unsigned int width (3), height (3), depth (3);
    std::vector<float> input_data ({
        10,10, 5, 
        10, 1, 6, 
         5, 8,10,

         1, 1,10, 
         1,10, 6, 
        10, 8, 4,

         1, 1, 1,
         1, 1,10,
         5,10,10, 
    });
    std::vector<float> valid_data (width*height*1);
    std::fill(valid_data.begin(), valid_data.end(), 10);


    // Initialise GPU information.
    cle::GPU gpu;
    cle::CLE cle(gpu);

    unsigned int dim[3] = {width, height, depth};
    cle::Buffer Buffer_A = cle.Push<float>(input_data, dim);

    unsigned int dim2[3] = {width, height, 1};
    cle::Buffer Buffer_B = cle.Create<float>(dim2);

    // Call kernel
    cle.MaximumZProjection(Buffer_A, Buffer_B);   

    // pull device memory to host
    std::vector<float> ouput_data = cle.Pull<float>(Buffer_B);

    // Verify output
    float difference = 0;
    for (size_t i = 0; i < ouput_data.size(); i++)
    {
        std::cout << valid_data[i] <<"="<< ouput_data[i] << "\n";
        difference += std::abs(valid_data[i] - ouput_data[i]);
    }
    if (difference > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Test failled, cumulated absolute difference " << difference << " > CPU epsilon (" << std::numeric_limits<float>::epsilon() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    
    // That's all folks!
    std::cout << "Test succeded!"<< std::endl;
    return EXIT_SUCCESS;
}

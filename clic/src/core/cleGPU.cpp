#include <iostream>
#include <stdexcept>

#include "cleGPU.hpp"

namespace cle
{

GPU::GPU() 
{
    // todo: make constructor safe
    std::vector<cl::Platform> m_PlatformList = this->ListPlatforms();
    this->m_Platform = m_PlatformList.front(); //! not safe if platform list is empty
    std::vector<cl::Device> m_DeviceList = ListDevices(this->m_Platform, "all");
    this->m_Device = m_DeviceList.front();     //! not safe if device list is empty
    this->AllocateDevice();
}

GPU::GPU(const char* t_device_name, const char* t_device_type) 
{
    this->SelectDevice(t_device_name, t_device_type);
}

GPU::~GPU() 
{
    if (!this->m_ProgramList.empty())
    {
        this->m_ProgramList.clear();
    }
}

const std::vector<cl::Platform> GPU::ListPlatforms() const
{
    std::vector<cl::Platform> m_PlatformList;
    try
    {
        cl::Platform::get(&m_PlatformList);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when fetching Platforms list." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
    return m_PlatformList;
}

const std::vector<cl::Device> GPU::ListDevices(const cl::Platform& t_platform, const char* t_device_type) const
{
    std::vector<cl::Device> m_DeviceList;
    try
    {
        if (strncmp("gpu", t_device_type, strlen(t_device_type)) == 0)
            t_platform.getDevices(CL_DEVICE_TYPE_GPU, &m_DeviceList);
        else if (strncmp("cpu", t_device_type, strlen(t_device_type)) == 0)
            t_platform.getDevices(CL_DEVICE_TYPE_CPU, &m_DeviceList);
        else
            t_platform.getDevices(CL_DEVICE_TYPE_ALL, &m_DeviceList);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when fetching Device list." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
    return m_DeviceList;
}

void GPU::AllocateDevice()
{
    try 
    {
        this->m_Context = cl::Context(this->m_Device);
    }
    catch(const cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when instantiating Context." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
    try
    {
        this->m_CommandQueue = cl::CommandQueue(this->m_Context, this->m_Device);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when instantiating Command Queue." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
}

void GPU::SelectDevice(const char* t_device_name, const char* t_device_type)
{
    bool found (false), deviceCheck(false);
    // Loop on all platforms
    std::vector<cl::Platform> m_PlatformList = this->ListPlatforms();
    auto platform_ite = m_PlatformList.begin();
    while(!found && platform_ite != m_PlatformList.end() )
    {
        // Loop on all devices
        std::vector<cl::Device> m_DeviceList = ListDevices(*platform_ite, t_device_type);
        auto device_ite = m_DeviceList.begin();
        while(!found && device_ite != m_DeviceList.end() )
        {
            // Only check available devices
            if(device_ite->getInfo<CL_DEVICE_AVAILABLE>())
            {
                deviceCheck = device_ite->getInfo<CL_DEVICE_NAME>().find(t_device_name) != std::string::npos;
                if(deviceCheck)
                {
                    // we found a {platform|device} with corresponding keyword
                    found = true;
                    this->m_Device = *device_ite;
                    this->m_Platform = *platform_ite;
                }
            }
            device_ite++;
        }
        platform_ite++;
    }
    if (!found)
    {
        // We take the first {platform|device}.
        //! not safe because we do not know if platform list is empty or not
        // todo: make safe
        this->m_Platform = m_PlatformList.front();
        this->m_Device = ListDevices(this->m_Platform, "all").front();
    }
    this->AllocateDevice();
}

cl::Device GPU::Device() const
{
    return this->m_Device;
}

cl::Context GPU::Context() const
{
    return this->m_Context;
}

cl::CommandQueue GPU::CommandQueue() const
{
    return this->m_CommandQueue;
}

cl::Platform GPU::Platform() const
{
    return this->m_Platform;
}

const cl::Program GPU::GetProgram(const size_t t_hash)
{
    //! not safe if FindProgram() is not used before
    // todo: check it != end()
    auto it = m_ProgramList.find(t_hash);
    return it->second;
}

const bool GPU::FindProgram(const size_t t_hash) const
{
    auto it = m_ProgramList.find(t_hash);
    return it != m_ProgramList.end();
}

void GPU::AddProgram(const cl::Program& t_program, const size_t t_hash)
{
    // todo: should return true or false
    m_ProgramList.insert({t_hash, t_program});
}

const std::string GPU::Name() const
{
    //! not safe if device is null
    // todo: check device allocation before fetching name
    return this->m_Device.getInfo<CL_DEVICE_NAME>();
}

const std::string GPU::Info() const
{
    //! not safe if platform and device are null
    // todo: check platform and device allocation before fetching info
    std::string out = "";
    out += "[" + this->m_Platform.getInfo<CL_PLATFORM_NAME>() + " - " + this->Name() + "]\n";
    out += "\tDevicedeviceType: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_TYPE>()) + "\n";
    out += "\tMaxComputeUnits: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>()) + "\n";
    out += "\tMaxClockFrequency: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>()) + "\n";
    out += "\tVersion: " + this->m_Device.getInfo<CL_DEVICE_VERSION>() + "\n";
    out += "\tExtensions: " + this->m_Device.getInfo<CL_DEVICE_EXTENSIONS>() + "\n";
    out += "\tGlobalMemorySizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>()) + "\n";
    out += "\tLocalMemorySizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()) + "\n";
    out += "\tMaxMemoryAllocationSizeInBytes: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()) + "\n";
    out += "\tMaxWorkGroupSize: " + std::to_string(this->m_Device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()) + "\n";
    return out;
}

const float GPU::Score() const
{
    // todo: make safe
    float score = 0;
    if(this->m_Device.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU)
        score += 4e12;
    else
        score += 2e12;
    score += this->m_Device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();  //! not safe if no device
    return score;
}

const cl::Buffer GPU::CreateBuffer(size_t t_bitsize) const
{
    cl_int error = CL_SUCCESS;
    cl::Buffer object (this->Context(), CL_MEM_READ_WRITE, t_bitsize, nullptr, &error);
    if(error != CL_SUCCESS)
    {
        throw std::runtime_error("Error in creating Buffer with return message \'" + std::string(GetErrorString(error)) + "\' (" + std::to_string(error) + ")\n");
    }
    return object;
}

void GPU::WriteBuffer(const cl::Buffer& t_buffer, void * t_data) const
{
    size_t bitsize = t_buffer.getInfo<CL_MEM_SIZE>();
    try
    {
        this->CommandQueue().enqueueWriteBuffer(t_buffer, CL_TRUE, 0, bitsize, t_data);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when enqueuing Write instruction to Buffer." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
}

void GPU::ReadBuffer(const cl::Buffer& t_buffer, void * t_data) const
{
    size_t bitsize = t_buffer.getInfo<CL_MEM_SIZE>();
    try
    {
        this->CommandQueue().enqueueReadBuffer(t_buffer, CL_TRUE, 0, bitsize, t_data);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when enqueuing Read instruction to Buffer." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
}

const cl::Image* GPU::CreateImage(const std::array<size_t,3>& t_shape, const cl::ImageFormat& t_format) const
{
    cl_int error = CL_SUCCESS;
    cl::Image* object_ptr = nullptr;
    if (t_shape[2] > 1)
    {
        object_ptr = new cl::Image3D (this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0], t_shape[1], t_shape[2], 0, 0, nullptr, &error);
        if(error != CL_SUCCESS)
        {
            throw std::runtime_error("Error in creating Image3D with return message \'" + std::string(GetErrorString(error)) + "\' (" + std::to_string(error) + ")\n");
        }
    }
    else if (t_shape[1] > 1)
    {
        object_ptr = new cl::Image2D (this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0], t_shape[1], 0, nullptr, &error);
        if(error != CL_SUCCESS)
        {
            throw std::runtime_error("Error in creating Image2D with return message \'" + std::string(GetErrorString(error)) + "\' (" + std::to_string(error) + ")\n");
        }
    }
    else
    {
        object_ptr = new cl::Image1D (this->Context(), CL_MEM_READ_WRITE, t_format, t_shape[0], nullptr, &error);
        if(error != CL_SUCCESS)
        {
            throw std::runtime_error("Error in creating Image1D with return message \'" + std::string(GetErrorString(error)) + "\' (" + std::to_string(error) + ")\n");
        }
    }
    return object_ptr;
}

void GPU::WriteImage(const cl::Image* t_image, void* t_data) const
{
    size_t row_pitch = t_image->getImageInfo<CL_IMAGE_ROW_PITCH>();
    size_t slice_pitch = t_image->getImageInfo<CL_IMAGE_SLICE_PITCH>();
    size_t width = t_image->getImageInfo<CL_IMAGE_WIDTH>();
    size_t height = t_image->getImageInfo<CL_IMAGE_HEIGHT>();
    size_t depth = t_image->getImageInfo<CL_IMAGE_DEPTH>(); 

    if(height == 0) height += 1;
    if(depth == 0) depth += 1;
    std::array<size_t,3> origin {0,0,0};
    std::array<size_t,3> region {width, height, depth};

    try
    {   
        this->CommandQueue().enqueueWriteImage(*t_image, CL_TRUE, origin, region, row_pitch, slice_pitch, t_data);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when enqueuing Write instruction to Image." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
}

void GPU::ReadImage(const cl::Image* t_image, void* t_data) const
{
    size_t row_pitch = t_image->getImageInfo<CL_IMAGE_ROW_PITCH>();
    size_t slice_pitch = t_image->getImageInfo<CL_IMAGE_SLICE_PITCH>();
    size_t width = t_image->getImageInfo<CL_IMAGE_WIDTH>();
    size_t height = t_image->getImageInfo<CL_IMAGE_HEIGHT>();
    size_t depth = t_image->getImageInfo<CL_IMAGE_DEPTH>();

    if(height == 0) height = 1;
    if(depth == 0) depth = 1;
    std::array<size_t,3> origin {0,0,0};
    std::array<size_t,3> region {width, height, depth};

    try
    {
        this->CommandQueue().enqueueReadImage(*t_image, CL_TRUE, origin, region, row_pitch, slice_pitch, t_data);
    }
    catch(cl::Error& e)
    {
        std::cerr << "Exception caught in GPU class. Error when enqueuing Read instruction to Image." << std::endl;
        std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }
}

void GPU::SetWaitForKernelToFinish(bool t_flag)
{
    this->m_WaitForFinish = t_flag;
}

void GPU::Finish() const
{
    // todo: this should be in kernel class
    if(m_WaitForFinish)
    {
        try
        { 
            this->m_CommandQueue.finish();
        }
        catch(const cl::Error& e)
        {
            std::cerr << "Exception caught in GPU class. Error when blocking Command Queue until list completion." << std::endl;
            std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
        }       
    }
}

void GPU::Flush() const
{
    // todo: usefullness? to delete or not?
    try
    { 
        this->m_CommandQueue.flush();
    }
    catch(const cl::Error& e)
    {
            std::cerr << "Exception caught in GPU class. Error when sending Command Queue to device." << std::endl;
            std::cerr << "\tError in \"" << e.what() << "\" with return message \'" << GetErrorString(e.err()) << "\' (" << e.err() << ")" << std::endl;
    }   
}

// todo: Copy/Cast operations between buffers and images
// void CopyBufferToBuffer(cle::Buffer& t_src, cle::Buffer& t_dst) const;
// void CopyBufferToImage(cle::Buffer& t_src, cle::Image& t_dst) const;
// void CopyImageToBuffer(cle::Image& t_src, cle::Buffer& t_dst) const;
// void CopyImageToImage(cle::Image& t_src, cle::Image& t_dst) const;

} // namespace cle

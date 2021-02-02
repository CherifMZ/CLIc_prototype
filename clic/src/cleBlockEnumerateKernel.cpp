

#include "cleBlockEnumerateKernel.h"

namespace cle
{

void BlockEnumerateKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void BlockEnumerateKernel::SetInputSums(Buffer& x)
{
    this->AddObject(x, "src_sums");
}

void BlockEnumerateKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void BlockEnumerateKernel::SetBlocksize(int x)
{
    this->AddObject(x, "blocksize");
}

void BlockEnumerateKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

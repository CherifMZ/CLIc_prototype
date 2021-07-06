

#include "cleSmallerConstantKernel.h"

namespace cle
{

void SmallerConstantKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src1");
}

void SmallerConstantKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SmallerConstantKernel::SetConstant(float x)
{
    this->AddObject(x, "scalar");
}

void SmallerConstantKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

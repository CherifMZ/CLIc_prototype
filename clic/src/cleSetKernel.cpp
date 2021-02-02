

#include "cleSetKernel.h"

namespace cle
{

void SetKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SetKernel::SetValue(float x)
{
    this->AddObject(x, "value");
}

void SetKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle


#include "cleCopyKernel.h"

namespace cle
{

void CopyKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void CopyKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void CopyKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle

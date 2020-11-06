#include"SdfRenderPass.h"



SdfRenderPass::SharedPtr SdfRenderPass::Create(uint width, uint height, uint2 mvDim)
{
    SdfRenderPass::SharedPtr sdfRenderPass = SdfRenderPass::SharedPtr(new SdfRenderPass());
    sdfRenderPass->width = width;
    sdfRenderPass->height = height;
    sdfRenderPass->mvDim = mvDim;
    sdfRenderPass->m_fullScreenPass = FullScreenPass::create("Samples/GridFluidSim/SdfRenderPass.ps.slang");
    sdfRenderPass->createPipeline();
    return sdfRenderPass;
}

void SdfRenderPass::Execute(RenderContext* renderContext, Fbo::SharedPtr targetFbo,const Array2f& mPhi)
{
    /*compute_Vars["PerFrameData"]["mvDim"] = mvDim;
    targetT = Texture::create2D(mvDim.x, mvDim.y, ResourceFormat::R32Float, 1U, 1U, nullptr, ResourceBindFlags::UnorderedAccess | ResourceBindFlags::ShaderResource);
    compute_Vars["targetT"] = targetT;
    init_Vars["PerFrameData"]["mvDim"] = mvDim;
    init_Vars["targetT"] = targetT;
    m_fullScreenPass->getVars()->setTexture("targetT", targetT);*/
   // timer.begin("sdfRenderPass");
    compute_Vars["PerFrameData"]["mvDim"] = mvDim;
    targetT = Texture::create2D(mvDim.x, mvDim.y, ResourceFormat::R32Float, 1U, 1U, nullptr, ResourceBindFlags::UnorderedAccess | ResourceBindFlags::ShaderResource);
    compute_Vars["targetT"] = targetT;
    m_fullScreenPass->getVars()->setTexture("targetT", targetT);


    uint sdfNum = mPhi.size();
    std::vector<float> data;
    data.reserve(sdfNum);
    for(int i=0;i<mvDim.x;i++)
    for (int j = 0; j < mvDim.y; j++)
    {
        data.push_back(mPhi(i,j));
    }
    

    sdfData = Buffer::createStructured(sizeof(float), mvDim.x * mvDim.y, Resource::BindFlags::ShaderResource | Resource::BindFlags::UnorderedAccess, Buffer::CpuAccess::None,data.data());
    compute_Vars->setBuffer("sdfData", sdfData);
    renderContext->dispatch(compute_State.get(), compute_Vars.get(), uint3((mvDim.x+15)/16, (mvDim.y + 15) / 16, 1));
    renderContext->flush(true);

   
    m_fullScreenPass->execute(renderContext, targetFbo);
    renderContext->flush(true);
   // timer.end();
}

void SdfRenderPass::OnGuiRender(Gui::Window& w)
{

}

void SdfRenderPass::createPipeline()
{
    compute_Program = ComputeProgram::createFromFile("Samples/GridFluidSim/SdfRenderPass.cs.slang", "main", Program::DefineList(), Shader::CompilerFlags::TreatWarningsAsErrors);
    compute_Vars = ComputeVars::create(compute_Program->getReflector());
    compute_State = ComputeState::create();
    compute_State->setProgram(compute_Program);


    Sampler::Desc LinearSamplerDesc;
    LinearSamplerDesc.setFilterMode(Sampler::Filter::Linear, Sampler::Filter::Linear, Sampler::Filter::Linear);
    LinearSampler = Sampler::create(LinearSamplerDesc);

    targetT = Texture::create2D(mvDim.x, mvDim.y, ResourceFormat::R32Float, 1U, 1U, nullptr, ResourceBindFlags::UnorderedAccess | ResourceBindFlags::ShaderResource);
    compute_Vars["PerFrameData"]["mvDim"] = mvDim;
    compute_Vars["targetT"] = targetT;
    

    m_fullScreenPass->getVars()["PerFrameData"]["gResolution"] = uint2(width, height);
    m_fullScreenPass->getVars()->setTexture("targetT", targetT);
    m_fullScreenPass->getVars()->setSampler("_LinearSampler", LinearSampler);
}

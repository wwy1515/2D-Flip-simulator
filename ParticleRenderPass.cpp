#include "ParticleRenderPass.h"

ParticleRenderPass::SharedPtr ParticleRenderPass::Create(uint width,uint height,uint2 mvDim, const Array2f& mPhi)
{
    ParticleRenderPass::SharedPtr particleRenderPass = ParticleRenderPass::SharedPtr(new ParticleRenderPass());
    particleRenderPass->width = width;
    particleRenderPass->height = height;
    particleRenderPass->mvDim = mvDim;
    particleRenderPass->m_fullScreenPass = FullScreenPass::create("Samples/GridFluidSim/ParticleRenderPass.ps.slang");

    uint sdfNum = mPhi.size();
    particleRenderPass->data.reserve(sdfNum);
    for (int i = 0; i < mvDim.x+1; i++)
        for (int j = 0; j < mvDim.y+1; j++)
        {
            particleRenderPass->data.push_back(mPhi(i, j));
        }

    particleRenderPass->createPipeline();
    return particleRenderPass;
}

void ParticleRenderPass::Execute(RenderContext* renderContext,Fbo::SharedPtr targetFbo, const std::vector<Vec2f>& mParticles)
{
    uint particleNum = mParticles.size();
    compute_Vars["PerFrameData"]["particleCount"] = particleNum;
    std::vector<float2> data;
    data.reserve(particleNum);

    for (int i = 0; i < particleNum; i++)
    {
        data.push_back(float2(mParticles[i][0]*mvDim.x, mParticles[i][1] * mvDim.y));
    }
    particleData = Buffer::createStructured(sizeof(float2), data.size(), Resource::BindFlags::ShaderResource | Resource::BindFlags::UnorderedAccess, Buffer::CpuAccess::None, data.data());
    compute_Vars->setBuffer("particles", particleData);


    renderContext->dispatch(init_State.get(), init_Vars.get(), uint3((width+15)/16, (height+15)/16, 1));
    renderContext->dispatch(compute_State.get(), compute_Vars.get(), uint3((particleNum+511)/512,1,1));
    renderContext->dispatch(compute_State2.get(), compute_Vars2.get(), uint3((mvDim.x+1 + 15) / 16, (mvDim.y+1 + 15) / 16, 1));
    m_fullScreenPass->execute(renderContext, targetFbo);
    renderContext->flush(true);
}

void ParticleRenderPass::OnGuiRender(Gui::Window& w)
{
    
}

void ParticleRenderPass::createPipeline()
{
    compute_Program = ComputeProgram::createFromFile("Samples/GridFluidSim/ParticleRenderPass.cs.slang", "main", Program::DefineList(), Shader::CompilerFlags::TreatWarningsAsErrors);
    init_Program = ComputeProgram::createFromFile("Samples/GridFluidSim/ParticleRenderPass.cs.slang", "main2", Program::DefineList(), Shader::CompilerFlags::TreatWarningsAsErrors);
    compute_Vars = ComputeVars::create(compute_Program->getReflector());
    init_Vars = ComputeVars::create(init_Program->getReflector());
    compute_State = ComputeState::create();
    init_State = ComputeState::create();
    compute_State->setProgram(compute_Program);
    init_State->setProgram(init_Program);


    Sampler::Desc pointSamplerDesc;
    pointSamplerDesc.setFilterMode(Sampler::Filter::Point, Sampler::Filter::Point, Sampler::Filter::Point);
    PointSampler = Sampler::create(pointSamplerDesc);


    compute_Vars["PerFrameData"]["gResolution"] = uint2(width, height);
    compute_Vars["PerFrameData"]["mvDim"] = mvDim;

    targetT = Texture::create2D(width, height, ResourceFormat::RGBA32Float, 1U, 1U, nullptr, ResourceBindFlags::UnorderedAccess | ResourceBindFlags::ShaderResource);
    compute_Vars["targetT"] = targetT;

    init_Vars["PerFrameData"]["gResolution"] = uint2(width, height);
    init_Vars["targetT"] = targetT;

    m_fullScreenPass->getVars()->setTexture("targetT", targetT);
    m_fullScreenPass->getVars()->setSampler("_pointSampler", PointSampler);



    //sdf pass
    compute_Program2 = ComputeProgram::createFromFile("Samples/GridFluidSim/SdfRenderPass.cs.slang", "main", Program::DefineList(), Shader::CompilerFlags::TreatWarningsAsErrors);
    compute_Vars2 = ComputeVars::create(compute_Program2->getReflector());
    compute_State2 = ComputeState::create();
    compute_State2->setProgram(compute_Program2);


    Sampler::Desc LinearSamplerDesc;
    LinearSamplerDesc.setFilterMode(Sampler::Filter::Linear, Sampler::Filter::Linear, Sampler::Filter::Linear);
    LinearSampler = Sampler::create(LinearSamplerDesc);

    targetT2 = Texture::create2D(mvDim.x+1, mvDim.y+1, ResourceFormat::R32Float, 1U, 1U, nullptr, ResourceBindFlags::UnorderedAccess | ResourceBindFlags::ShaderResource);
    compute_Vars2["PerFrameData"]["mvDim"] = uint2(mvDim.x+1,mvDim.y+1);
    compute_Vars2["targetT"] = targetT2;


    m_fullScreenPass->getVars()["PerFrameData"]["gResolution"] = uint2(width, height);
    m_fullScreenPass->getVars()->setTexture("targetT2", targetT2);
    m_fullScreenPass->getVars()->setSampler("_LinearSampler", LinearSampler);

    sdfData = Buffer::createStructured(sizeof(float), (mvDim.x+1) * (mvDim.y+1), Resource::BindFlags::ShaderResource | Resource::BindFlags::UnorderedAccess, Buffer::CpuAccess::None, data.data());
    compute_Vars2->setBuffer("sdfData", sdfData);
   
    
    
}

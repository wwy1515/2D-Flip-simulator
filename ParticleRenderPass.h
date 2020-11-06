#pragma once
#include "Falcor.h"
#include "core/fluid/fluidsim.h"

using namespace Falcor;

class ParticleRenderPass : public std::enable_shared_from_this<ParticleRenderPass>
{
public:
    using SharedPtr = std::shared_ptr<ParticleRenderPass>;
    static ParticleRenderPass::SharedPtr Create(uint width, uint height,uint2 mvDim, const Array2f& mPhi);
    void Execute(RenderContext* renderContext,Fbo::SharedPtr targetFbo,const std::vector<Vec2f> &mParticles);
    void OnGuiRender(Gui::Window& w);
    void createPipeline();
    
    uint width;
    uint height;
    uint2 mvDim;

private:
    Sampler::SharedPtr PointSampler;
    Sampler::SharedPtr LinearSampler;

    FullScreenPass::SharedPtr m_fullScreenPass;
    ComputeProgram::SharedPtr compute_Program;
    ComputeProgram::SharedPtr compute_Program2;
    ComputeProgram::SharedPtr init_Program;
    ComputeVars::SharedPtr    compute_Vars;
    ComputeVars::SharedPtr    init_Vars;
    ComputeVars::SharedPtr    compute_Vars2;
    ComputeState::SharedPtr   compute_State;
    ComputeState::SharedPtr   compute_State2;
    ComputeState::SharedPtr   init_State;
    Texture::SharedPtr targetT;
    Texture::SharedPtr targetT2;

    std::vector<float> data;
    Buffer::SharedPtr particleData;
    Buffer::SharedPtr sdfData;
};

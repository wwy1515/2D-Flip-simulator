#pragma once
#include "Falcor.h"
#include "core/fluid/fluidsim.h"
#include "core/util/Timer.h"

using namespace Falcor;

//template<uint Dimension>
class SdfRenderPass : public std::enable_shared_from_this<SdfRenderPass>
{
public:
    using SharedPtr = std::shared_ptr<SdfRenderPass>;
    static SdfRenderPass::SharedPtr Create(uint width, uint height, uint2 mvDim);
    void Execute(RenderContext* renderContext, Fbo::SharedPtr targetFbo, const Array2f& mPhi);
    void OnGuiRender(Gui::Window& w);
    void createPipeline();

    uint width;
    uint height;
    uint2 mvDim;

private:
    Sim::Timer timer;
    Sampler::SharedPtr LinearSampler;
    Sampler::SharedPtr PointSampler;
    FullScreenPass::SharedPtr m_fullScreenPass;
    ComputeProgram::SharedPtr compute_Program;
    ComputeVars::SharedPtr    compute_Vars;
    ComputeState::SharedPtr   compute_State;
    Texture::SharedPtr targetT;

    Buffer::SharedPtr sdfData;

    Fbo::SharedPtr                  m_Fbo;
    GraphicsProgram::SharedPtr      m_Program = nullptr;
    GraphicsVars::SharedPtr         m_ProgramVars = nullptr;
    GraphicsState::SharedPtr        m_GraphicsState = nullptr;

    RasterizerState::SharedPtr      m_RasterizerState = nullptr;
    DepthStencilState::SharedPtr    m_DepthStencilState = nullptr;

};

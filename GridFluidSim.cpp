
#include "GridFluidSim.h"

uint32_t mSampleGuiWidth = 250;
uint32_t mSampleGuiHeight = 200;
uint32_t mSampleGuiPositionX = 20;
uint32_t mSampleGuiPositionY = 40;
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 800
static uint frameIndex = 0;
static uint expectedFrameIndex = 0;
static uint outputMode = 0;
static bool continuousAnimation = false;


Vec2f c0(0.5f, 0.5f), c1(0.5f, 0.3f), c2(0.3f, 0.35f), c3(0.5f, 0.7f);
float rad0 = 0.49f, rad1 = 0.1f, rad2 = 0.1f, rad3 = 0.1f;

float circle_phi(const Vec2f& position, const Vec2f& centre, float radius) {
    return (dist(position, centre) - radius);
}

float boundary_phi(const Vec2f& position) {
    float phi0 = -circle_phi(position, c0, rad0);
    float phi1 = circle_phi(position, c1, rad1);
    //float phi2 = circle_phi(position, c2, rad2);
    //float phi3 = circle_phi(position, c3, rad3);

    //return phi0;
    return min(phi0, phi1);
    //min(min(phi0,phi1),min(phi2,phi3));
}




const Gui::RadioButtonGroup OutputSelectionButtons =
{
    { (uint32_t)0, "Particle", true },
    { (uint32_t)1, "Sdf", true }
};

void GridFluidSim::onGuiRender(Gui* pGui)
{
    Gui::Window w(pGui, "Falcor", { 250, 200 });
    gpFramework->renderGlobalUI(pGui);
    w.text("Hello from GridFluidSim");
    if (w.button("Single Step"))
    {
        expectedFrameIndex++;
    }
    if (w.button("Step 60 Frames"))
    {
        expectedFrameIndex+=60;
    }
    if (w.button("ContinuousAnimation"))
    {
        continuousAnimation = !continuousAnimation;
    }
    w.text("FrameIndex  "+std::to_string(frameIndex));
    w.radioButtons(OutputSelectionButtons, outputMode);
    float frameTime = gpFramework->getFrameRate().getLastFrameTime();
    w.text("FrameTime: ", false);
    w.text(std::to_string(frameTime), true);
    w.text(" S", true);

}

void GridFluidSim::onLoad(RenderContext* pRenderContext)
{
    

    //Set up the simulation
    sim.pUseFlip = true;
    sim.initialize(grid_width, grid_resolution, grid_resolution);

    //set up a circle boundary
    sim.set_boundary(boundary_phi);

    sim.seed_particles(boundary_phi, grid_resolution*2.0);

    
    sim.compute_phi();

    pParticleRenderPass = ParticleRenderPass::Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, uint2(grid_resolution, grid_resolution),sim.getSolidPhi());
    pSdfRenderPass = SdfRenderPass::Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, uint2(sim.getPhi().ni, sim.getPhi().nj));
    
}

void GridFluidSim::onFrameRender(RenderContext* pRenderContext, const Fbo::SharedPtr& pTargetFbo)
{
    const float4 clearColor(0.38f, 0.52f, 0.10f, 1);
    pRenderContext->clearFbo(pTargetFbo.get(), clearColor, 1.0f, 0, FboAttachmentType::All);
    if (continuousAnimation)
        expectedFrameIndex++;

    
    if (frameIndex < expectedFrameIndex)
    {
        std::cout << "frame" << frameIndex << std::endl;
        //timer.begin("simulation");
        sim.advance(timestep);
        //timer.end();
        frameIndex++;
        
   }
   // gFluid.Advance(gfTimeStep);
    if(outputMode==0)
    pParticleRenderPass->Execute(pRenderContext, pTargetFbo, sim.particles);
    if(outputMode==1)
    pSdfRenderPass->Execute(pRenderContext, pTargetFbo, sim.getPhi());



}

void GridFluidSim::onShutdown()
{
}

bool GridFluidSim::onKeyEvent(const KeyboardEvent& keyEvent)
{
    return false;
}

bool GridFluidSim::onMouseEvent(const MouseEvent& mouseEvent)
{
    return false;
}

void GridFluidSim::onHotReload(HotReloadFlags reloaded)
{
}

void GridFluidSim::onResizeSwapChain(uint32_t width, uint32_t height)
{
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

    Eigen::initParallel();

    //open a console for winmain application, redirect the stdout to the console created.
    AllocConsole();
    HANDLE stdHandle;
    int hConsole;
    FILE* fp;
    stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
    fp = _fdopen(hConsole, "w");
    freopen_s(&fp, "CONOUT$", "w", stdout);



    GridFluidSim::UniquePtr pRenderer = std::make_unique<GridFluidSim>();
    SampleConfig config;


    config.windowDesc.title = "Falcor Project Template";
    config.windowDesc.resizableWindow = true;
    config.windowDesc.width = DEFAULT_WIDTH;
    config.windowDesc.height = DEFAULT_HEIGHT;
    Sample::run(config, pRenderer);
    return 0;
}

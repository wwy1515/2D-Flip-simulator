


cbuffer PerFrameData
{
	uint particleCount;
	uint2 gResolution;
	uint2 mvDim;
};

StructuredBuffer<float2> particles;
RWTexture2D<float4> targetT;


[numthreads(512, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{   
	if(DTid.x>=particleCount) return;
	float2 xy=particles[DTid.x];
    

    float scaleY=gResolution.y/mvDim.y;
    float positionY=gResolution.y-xy.y*scaleY;
    uint TextureY=ceil(positionY);

    float scaleX=gResolution.x/mvDim.x;
    float positionX=xy.x*scaleX;
    uint TextureX=floor(positionX);
    targetT[uint2(TextureX,TextureY)]=float4(0.0,0.0,1.0,1.0);

}

[numthreads(16, 16, 1)]
void main2( uint3 DTid : SV_DispatchThreadID )
{   
    if(any(DTid.xy>=gResolution)) return;
    targetT[DTid.xy]=float4(1.0,1.0,1.0,1.0);

}

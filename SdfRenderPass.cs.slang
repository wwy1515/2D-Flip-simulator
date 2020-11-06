


cbuffer PerFrameData
{   
	uint2 mvDim;
};



StructuredBuffer<float> sdfData;
RWTexture2D<float> targetT;
[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{   
	if(any(DTid.xy>=mvDim)) return;
	uint linearIndex=DTid.y+DTid.x*mvDim.x;
    targetT[uint2(DTid.x,mvDim.y-1-DTid.y)]=sdfData[linearIndex];
}



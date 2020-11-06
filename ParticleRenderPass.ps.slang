
cbuffer PerFrameData
{   
    uint2 gResolution;
};

Texture2D<float4> targetT;
SamplerState _pointSampler;
Texture2D<float> targetT2;
SamplerState _LinearSampler;

float4 main(in float2 UV : TEXCOORD) : SV_TARGET
{

	float value = targetT2.Sample( _LinearSampler, UV);
	if(value<0.0f)
	return float4(1.0f,0.0f,0.0f,1.0f);
    float4 color = targetT.Sample( _pointSampler, UV);
    return color;
}





cbuffer PerFrameData
{   
    uint2 gResolution;
    float narrowBandWidth;
};

Texture2D<float> targetT;
SamplerState _LinearSampler;



float4  main( in float2 UV : TEXCOORD) :SV_Target0
{   
	float value = targetT.Sample( _LinearSampler, UV);
	if(value<0.0f)
	return float4(0.0f,0.0f,1.0f,1.0f);
	else
	return float4(1.0f,0.0f,0.0f,1.0f);

}
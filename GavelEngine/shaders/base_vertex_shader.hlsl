cbuffer first_buffer : register(b0)
{
	float4x4 projection;
}
cbuffer second_buffer : register(b1)
{
	float4x4 view;
	float4x4 model;
	float4x4 model_inverse;
}
float4 main(float4 pos : POSITION) : SV_POSITION
{
	float4x4 WVP = mul(projection,mul(view,model));
	return mul(WVP,pos);
}

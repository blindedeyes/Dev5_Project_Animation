SamplerState filters : register(s0);
Texture2D thisTexture : register(t0);

struct INPUT
{
    float4 pos : SV_POSITION;
    float4 colorOut : COLOR;
    float3 normal : NORMAL;
    float4 worldPos : WORLDPOS;

};

//Just doing directional, doc didn't ask for specific type, and I am being kinda lazy

struct Light
{
    float4 pos; // : POSITION;
    float4 dir; // : DIRECTION;
    float4 color; // : COLOR;
    float4 radius; // : RADIUS;
};

// Per-pixel color data passed through the pixel shader.
//struct PixelShaderInput
//{
//    float4 pos : SV_POSITION;
//    float3 uv : UV;
//    float3 normal : NORMAL;
//    float4 worldPos : WORLDPOS;
//    float4 Tangent : TANGENT;
//    float4 bTangent : BTANGENT;
//    float4 lPos : LightPos;
//};

cbuffer Lights : register(cb0)
{
    Light LightValues[2];
};
float4 main(INPUT input) : SV_TARGET
{
    
    float4 lightColor = float4(0, 0, 0, 0);
    input.normal.xyz = normalize(input.normal.xyz);
	//Lighting
    float ratio = 0.0;
    float4 dir;

    //float3 screenpos = input.pos.xyz / input.pos.w;
    //on screen check

    ratio = dot(-1 * normalize(LightValues[0].dir.xyz), input.normal.xyz);
    ratio = saturate(ratio);
    lightColor += saturate((LightValues[0].color * ratio));
              

	//X is the ratio of ambient light
    lightColor += ((LightValues[1].color * LightValues[1].radius.x));



    return (thisTexture.Sample(filters, input.colorOut.xy) * lightColor);
}
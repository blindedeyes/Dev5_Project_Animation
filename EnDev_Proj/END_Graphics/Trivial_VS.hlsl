struct INPUT_VERTEX
{
	float4 pos : POSITION;
    float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 pos : SV_POSITION;
	float4 colorOut : COLOR;
};

cbuffer cWorldData
{
    float4x4 view;
    float4x4 proj;
    float4x4 world[4];
};
OUTPUT_VERTEX main(INPUT_VERTEX v,
    unsigned int instID : SV_InstanceID)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
    output.colorOut = v.color;
    //output.pos = mul(mul(mul(v.pos , world[instID]), view), proj);
    output.pos = mul(float4(v.pos.xyz,1), world[instID]);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    //output.pos.w = 1;


	return output;
}
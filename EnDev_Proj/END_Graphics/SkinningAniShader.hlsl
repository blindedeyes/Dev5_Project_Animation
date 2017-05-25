struct INPUT_VERTEX
{

	unsigned int bneCnt : BONECNT;
	unsigned int4 boneID : BONES;
	float4 boneWeight : BONEWEIGHT;
	
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 pos : SV_POSITION;
	float4 colorOut : COLOR;
};

cbuffer cWorldData : register(cb0)
{
	float4x4 view;
	float4x4 proj;
	float4x4 world[4];
};
struct bone {
	matrix keyframe[3];
};
cbuffer cBoneData : register(cb1)
{
	float ratio;
	bone bones[35];
};
OUTPUT_VERTEX main(INPUT_VERTEX v,
	unsigned int instID : SV_InstanceID)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	output.colorOut = v.color;
	if (v.bneCnt > 0) {
		float4 temp = 0;
		for (unsigned int i = 0; i < 4; ++i) {
			if (v.bneCnt > i) {
				temp = mul(float4(v.pos.xyz, 1), bones[v.boneID[i]].keyframe[0]);
				//float4 temp2 = mul(temp, bones[v.boneID[i]].keyframe[1]);
				//temp = mul(temp, bones[v.boneID[i]].keyframe[2]);
				temp = mul(temp, lerp(bones[v.boneID[i]].keyframe[1], bones[v.boneID[i]].keyframe[2], ratio));
				//temp = lerp(temp, temp2, ratio);
				output.pos += temp * v.boneWeight[i];
			}
		}
		
	}
	else {
		//output.pos = mul(mul(mul(v.pos , world[instID]), view), proj);
		output.pos = float4(v.pos.xyz, 1);
		//output.pos.w = 1;
	}
	output.pos += mul(output.pos, world[instID]);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);



	return output;
}
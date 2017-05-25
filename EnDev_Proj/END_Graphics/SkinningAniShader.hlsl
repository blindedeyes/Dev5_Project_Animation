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
struct ani {
	float4x4 keyframe[50];
};
struct bone {
	ani anim[15];
};
cbuffer cBoneData : register(cb1)
{
	unsigned int aniID;
	unsigned int KeyFrame;
	float ratio;
	bone bones[50];
};
OUTPUT_VERTEX main(INPUT_VERTEX v,
	unsigned int instID : SV_InstanceID)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	output.colorOut = v.color;
	if (v.bneCnt > 0) {
		float4 temp = 0;
		//for (unsigned int i = 0; i < 4; ++i) {
		//	if (v.bneCnt > i) {
				//bones[v.boneID[i]]
		//		lerp
		//	}
		//}
		temp = mul(float4(v.pos.xyz,1), bones[v.boneID[0]].anim[aniID].keyframe[0]);
		temp = mul(temp, bones[v.boneID[0]].anim[aniID].keyframe[KeyFrame]);
		output.pos = mul(temp, world[instID]);
	}
	else {
		//output.pos = mul(mul(mul(v.pos , world[instID]), view), proj);
		output.pos = mul(float4(v.pos.xyz, 1), world[instID]);
		//output.pos.w = 1;
	}
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);



	return output;
}
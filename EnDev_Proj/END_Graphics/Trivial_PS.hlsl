
struct INPUT
{
    float4 pos : SV_POSITION;
    float4 colorOut : COLOR;
};


float4 main( INPUT input ) : SV_TARGET
{
	return input.colorOut;
}
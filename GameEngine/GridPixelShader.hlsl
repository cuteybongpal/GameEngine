#include "GridCommon.hlsli"

float4 main(VS_OUTPUT pixel) : SV_TARGET
{
    float2 pos = pixel.worldPos.xy;
    float x = abs(pos.x - round(pos.x));
    float y = abs(pos.y - round(pos.y));
    
    if (x < (0.01f / cameraZoom))
        return float4(1, 1, 1, .5f);
    if (y < (0.01f / cameraZoom))
        return float4(1, 1, 1, .5f);
    
    return float4(0.35f, 0.35f, 0.35f, 1);
};

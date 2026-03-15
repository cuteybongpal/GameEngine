#include "GridCommon.hlsli"

VS_OUTPUT main(VS_INPUT vertex)
{
    VS_OUTPUT o;
    o.pos = float4(vertex.pos, 1);
    o.pos = mul(o.pos, proj);
    o.uv = vertex.uv;
    float x = (size.x / 2) / (unitsize * cameraZoom);
    float y = (size.y / 2) / (unitsize * cameraZoom);
    o.worldPos = float4(x * vertex.pos.x, y * vertex.pos.y, 0, 1);
    o.worldPos = mul(cameraPos, o.worldPos);
    
	return o;
}
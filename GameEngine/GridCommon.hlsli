cbuffer Transform : register(b0)
{
    //동차좌표가 뭔데? 이해가 하나도 안되노
    //걍 단위 행렬 ㄱㄱ
    //그리드만 그릴거라 거창한건 필요 없어보임
    //진짜 너무 어렵다
    float4x4 proj;
    float4x4 cameraPos;
    float cameraZoom;
    float2 size;
    float unitsize;
}


struct VS_INPUT
{
    // : POSITION 이건 Semantic이라고하는데, 이 데이터가 파이프라인에서 어떤 역할인지를 GPU에게 알려주는 거, POSITION은 원래 정점 위치를 알려주는 얘
    float3 pos : POSITION;
    //TEXCOORD0 이거는 UV좌표에 붙는 거
    float2 uv : TEXCOORD0;
    
};

struct VS_OUTPUT
{
    // : SV_POSITION은 클립 공간의 좌표를 의미함
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 worldPos : TEXCOORD1;
};
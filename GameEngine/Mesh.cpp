#include "Mesh.h"
#include "GraphicsDevice.h"
#include <iostream>

MeshBuffer meshToMeshBuffer(Mesh& mesh)
{
    MeshBuffer mBuffer {};
    //정점 버퍼 생성
    {
        D3D11_BUFFER_DESC bd{};

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = mesh.vertexes.size() * sizeof(Vertex);
        //이거를 뭘로 사용할지 정점 버퍼로 사용할것이기 때문에, BERTEX_BUFFER로
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = mesh.vertexes.data();
        HRESULT hr = Graphic::GetDevice()->CreateBuffer(&bd, &init, mBuffer.vertexBuffer.GetAddressOf());

        if (FAILED(hr))
        {
            HRESULT reason = Graphic::GetDevice()->GetDeviceRemovedReason();
            std::cout << "DeviceRemovedReason=0x" << std::hex << reason << std::dec << "\n";
            std::cout << "CreateBuffer failed. hr=0x" << std::hex << hr << std::dec << "\n";
            std::cout << "mesh to mesh Buffer failed! generate vertex buffer failed" << std::endl;
            return {}; // 실패 시 빈 MeshBuffers 반환
        }
    }

    {
        D3D11_BUFFER_DESC bd{};

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.ByteWidth = mesh.indexes.size() * sizeof(UINT);
        //이거를 뭘로 사용할지 인덱스 버퍼로 사용할것이기 때문에, INDEX_BUFFER로
        
        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = mesh.indexes.data();
        HRESULT hr = Graphic::GetDevice()->CreateBuffer(&bd, &init, mBuffer.indexBuffer.GetAddressOf());

        if (FAILED(hr))
        {
            std::cout << "mesh to mesh Buffer failed! generate index buffer failed" << std::endl;
            return {}; // 실패 시 빈 MeshBuffers 반환
        }
    }
    mBuffer.indexCount = mesh.indexes.size();
    mBuffer.offset = 0;
    mBuffer.stride = sizeof(Vertex);
    return mBuffer;
}

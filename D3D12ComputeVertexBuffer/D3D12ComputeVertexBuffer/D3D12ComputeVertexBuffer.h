//*********************************************************
//
// Copyright (c) Shader.jp. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "DXSample.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12ComputeVertexBuffer : public DXSample
{
public:
    D3D12ComputeVertexBuffer(UINT width, UINT height, std::wstring name);

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

private:
    static const UINT FrameCount = 2;

    // Descriptor Heap Index
    enum class SampleHeapIndex : int
    {
        PerDrawCbv = 0,
        ComputeCbv,
        SrcVertexSRV,
        CacheVertexUAV,
        SampleHeapIndexMax,
    };

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    struct SceneConstantBuffer
    {
        XMFLOAT4 offset;
        float padding[60]; // Padding so the constant buffer is 256-byte aligned.
    };
    static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    struct ComputeConstantBuffer
    {
        XMFLOAT4 offset;
        XMFLOAT4 color;
        float padding[56]; // Padding so the constant buffer is 256-byte aligned.
    };
    static_assert((sizeof(ComputeConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandAllocator> m_computeCommandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;

    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12RootSignature> m_computeRootSignature;

    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_resouceHeap;

    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12PipelineState> m_computePipelineState;

    // コマンドリスト.
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<ID3D12GraphicsCommandList> m_computeCommandList;

    UINT m_rtvDescriptorSize;
    UINT m_descripterIncrementSize;

    // App resources.
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ComPtr<ID3D12Resource> m_cacheVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_cacheVertexBufferView;

    // 描画用コンスタントバッファ.
    ComPtr<ID3D12Resource> m_PerDrawCBV;
    SceneConstantBuffer m_PerDrawData;
    UINT8* m_pCbvDataBegin;

    // Compute Shader用コンスタントバッファ 
    ComPtr<ID3D12Resource> m_ComputeCbv;
    ComputeConstantBuffer m_ComputeCbvData;
    UINT8* m_ComputeCbvDataBegin;

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;

    void LoadPipeline();
    void LoadAssets();
    void CreateRootSignatures();
    void CreatePipelineStates();
    void CreateConstantBuffers();
    void CreateVertexBuffer();
    void PopulateComputeCommandList();
    void PopulateCommandList();
    void WaitForPreviousFrame();


};


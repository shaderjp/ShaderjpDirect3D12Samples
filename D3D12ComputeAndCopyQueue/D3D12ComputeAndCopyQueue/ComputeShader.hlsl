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


struct VertexBuffer
{
	float3 position;
	float4 color;
};

cbuffer SceneConstantBuffer : register(b0)
{
	float4 offset;
	float4 color;
	float4 padding[14];
};

StructuredBuffer<VertexBuffer> g_srcVertexBuffer : register(t0);
RWStructuredBuffer<VertexBuffer> g_destVertexBuffer: register(u0);

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float3 destPosition = g_srcVertexBuffer[DTid.x].position;

	g_destVertexBuffer[DTid.x].position = destPosition + offset;
	g_destVertexBuffer[DTid.x].color = g_srcVertexBuffer[DTid.x].color * color;
}
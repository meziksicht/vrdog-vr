// Modifications made by K. Kameník - Meziksicht, 2025.
// Copyright (c) 2017-2025 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if defined(XR_USE_GRAPHICS_API_D3D11) || defined(XR_USE_GRAPHICS_API_D3D12)

#include <DirectXMath.h>

struct ModelConstantBuffer {
    DirectX::XMFLOAT4X4 Model;
};
struct ViewProjectionConstantBuffer {
    DirectX::XMFLOAT4X4 ViewProjection;
};

// Separate entrypoints for the vertex and pixel shader functions.
constexpr char ShaderHlsl[] = R"_(
    struct PSVertex {
        float4 Pos : SV_POSITION;
        float2 uv : TEXCOORD;
    };
    struct Vertex {
        float3 Pos : POSITION;
        float2 uv : TEX;
    };
    cbuffer ModelConstantBuffer : register(b0) {
        float4x4 Model;
    };
    cbuffer ViewProjectionConstantBuffer : register(b1) {
        float4x4 ViewProjection;
    };

    Texture2D    mytexture : register(t0);
    SamplerState mysampler : register(s0);

    PSVertex MainVS(Vertex input) {
       PSVertex output;
       output.Pos = mul(mul(float4(input.Pos, 1), Model), ViewProjection);
       output.uv = input.uv;
       return output;
    }

    float4 MainPS(PSVertex input) : SV_TARGET {
        return mytexture.Sample(mysampler, input.uv);
    }
    )_";

DirectX::XMMATRIX XM_CALLCONV LoadXrPose(const XrPosef& pose);
DirectX::XMMATRIX XM_CALLCONV LoadXrMatrix(const XrMatrix4x4f& matrix);

Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const char* hlsl, const char* entrypoint, const char* shaderTarget);
Microsoft::WRL::ComPtr<IDXGIAdapter1> GetAdapter(LUID adapterId);

#endif

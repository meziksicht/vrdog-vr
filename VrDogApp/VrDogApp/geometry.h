// Modifications made by K. Kameník - Meziksicht, 2025.
// Copyright (c) 2017-2025 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace Geometry {

struct Vertex {
    XrVector3f Position;
    XrVector2f UV;
};

//constexpr XrVector3f Red{1, 0, 0};
//constexpr XrVector3f DarkRed{0.25f, 0, 0};
//constexpr XrVector3f Green{0, 1, 0};
//constexpr XrVector3f DarkGreen{0, 0.25f, 0};
//constexpr XrVector3f Blue{0, 0, 1};
//constexpr XrVector3f DarkBlue{0, 0, 0.25f};


constexpr XrVector2f UV_1{ 0,1 };
constexpr XrVector2f UV_2{ 1,1 };
constexpr XrVector2f UV_3{ 1,0 };
constexpr XrVector2f UV_4{ 0,0 };


// Vertices for a 1x1x1 meter cube. (Left/Right, Top/Bottom, Front/Back)
constexpr XrVector3f LBB{-0.5f, -0.5f, -0.5f};
constexpr XrVector3f LBF{-0.5f, -0.5f, 0.5f};
constexpr XrVector3f LTB{-0.5f, 0.5f, -0.5f};
constexpr XrVector3f LTF{-0.5f, 0.5f, 0.5f};
constexpr XrVector3f RBB{0.5f, -0.5f, -0.5f};
constexpr XrVector3f RBF{0.5f, -0.5f, 0.5f};
constexpr XrVector3f RTB{0.5f, 0.5f, -0.5f};
constexpr XrVector3f RTF{0.5f, 0.5f, 0.5f};

#define CUBE_SIDE(V1, V2, V3, V4, V5, V6, UV1, UV2, UV3, UV4) {V1, UV1}, {V2, UV4}, {V3, UV3}, {V4, UV1}, {V5, UV3}, {V6, UV2},

constexpr Vertex c_cubeVertices[] = {
    CUBE_SIDE(LTB, LBF, LBB, LTB, LTF, LBF, UV_1, UV_2, UV_3, UV_4)    // -X
    CUBE_SIDE(RTB, RBB, RBF, RTB, RBF, RTF, UV_1, UV_2, UV_3, UV_4)        // +X
    CUBE_SIDE(LBB, LBF, RBF, LBB, RBF, RBB, UV_1, UV_2, UV_3, UV_4)  // -Y
    CUBE_SIDE(LTB, RTB, RTF, LTB, RTF, LTF, UV_1, UV_2, UV_3, UV_4)      // +Y
    CUBE_SIDE(LBB, RBB, RTB, LBB, RTB, LTB, UV_1, UV_2, UV_3, UV_4)   // -Z
    CUBE_SIDE(LBF, LTF, RTF, LBF, RTF, RBF, UV_1, UV_2, UV_3, UV_4)       // +Z
};

// Winding order is clockwise. Each side uses a different color.
constexpr unsigned short c_cubeIndices[] = {
    0,  1,  2,  3,  4,  5,   // -X
    6,  7,  8,  9,  10, 11,  // +X
    12, 13, 14, 15, 16, 17,  // -Y
    18, 19, 20, 21, 22, 23,  // +Y
    24, 25, 26, 27, 28, 29,  // -Z
    30, 31, 32, 33, 34, 35,  // +Z
};

}  // namespace Geometry

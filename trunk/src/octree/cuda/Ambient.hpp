/*
 *  Copyright (c) 2009-2011, NVIDIA Corporation
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of NVIDIA Corporation nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "base/Math.hpp"
#include "base/DLLImports.hpp"
#include "Render.hpp" // for a number of tracing constants

namespace FW
{

//------------------------------------------------------------------------

#define AMBK_BLOCK_WIDTH        32
#define AMBK_BLOCK_HEIGHT       2

//------------------------------------------------------------------------

struct AmbientRequest
{
    Vec3i           pos;                // node position
    S32             level;              // node level (child level)
};

struct AmbientResult
{
    Vec3f           ao;                 // ao result
};

struct AmbientInput
{
    S32             numRequests;        // number of nodes to process
    S32             raysPerNode;        // number of AO rays to cast, maximum is 256
    F32             rayLength;          // ray length (scene size is 1.0)
    CUdeviceptr     requestPtr;         // requests be here
    CUdeviceptr     resultPtr;          // results go here
    CUdeviceptr     rootNode;           // hierarchy root node
    CUdeviceptr     activeWarps;        // for tracking active warps
    OctreeMatrices  octreeMatrices;
};

//------------------------------------------------------------------------
}

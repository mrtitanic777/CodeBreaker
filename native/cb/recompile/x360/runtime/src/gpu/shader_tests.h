#pragma once

namespace gpu
{
    // CB_SHADER_TEST=1: translator verification instead of running the game (shader_tests.cpp). Returns the number of
    // failures.
    int RunShaderTests();
}

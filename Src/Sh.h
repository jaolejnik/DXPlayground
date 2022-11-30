#pragma once

#include "../pch.h"
#include "Cubemap.h"

typedef DirectX::XMVECTOR (*polarFn)(const float theta, const float phi);

constexpr int SHbands = 4;
constexpr int SHcoeffCount = SHbands * SHbands;
constexpr float SHweight = 4.0 * DirectX::XM_PI;

struct SHSample
{
    DirectX::XMFLOAT3 vector;
    float theta;
    float phi;
    std::vector<float> coefficients;
};

class SphericalHarmonics
{
private:
    std::vector<SHSample *> m_samples;

    DirectX::XMVECTOR SampleCubemap(
        Cubemap *cubemap,
        DirectX::XMFLOAT3 vec);
    float K(int l, int m);
    float P(int l, int m, float x);

public:
    SphericalHarmonics(int sqrtSampleCount, int bandCount);
    ~SphericalHarmonics();

    void GenerateSamples(int sqrtSampleCount, int bandCount);
    float EvaluateSH(int l, int m, float theta, float phi);
    void ProjectToSH(polarFn fn);
    void ProjectCubemapToSH(Cubemap *cubemap);
};

// Test lighting function from Green's paper
DirectX::XMVECTOR LightPolar(float theta, float phi);

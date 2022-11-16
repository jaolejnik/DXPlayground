#pragma once

#include "../pch.h"

struct SHSample
{
    DirectX::XMFLOAT3 spherical;
    DirectX::XMFLOAT3 vector;
    double *coeff;
};

class SphericalHarmonics
{
private:
    SHSample *m_samples;

public:
    SphericalHarmonics() {}
    ~SphericalHarmonics() {}

    void GenerateSamples(int n, int bands);
    double SH(int l, int m, double theta, double phi);
};

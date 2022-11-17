#pragma once

#include "../pch.h"

struct SHSample
{
    DirectX::XMFLOAT3 vector;
    double theta;
    double phi;
    std::vector<double> coeff;
};

class SphericalHarmonics
{
private:
    std::vector<SHSample *> m_samples;

public:
    SphericalHarmonics() {}
    ~SphericalHarmonics();

    void InitializeSamplesArray(int sqrt_n_samples, int n_bands);
    void GenerateSamples(int sqrt_n_samples, int bands);
    double EvaluateSH(int l, int m, double theta, double phi);
};

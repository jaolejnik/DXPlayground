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
    double K(int l, int m);
    double P(int l, int m, double x);

public:
    std::vector<SHSample *> m_samples;

    SphericalHarmonics(int sqrtSampleCount, int bandCount);
    ~SphericalHarmonics();

    void GenerateSamples(int sqrtSampleCount, int bandCount);
    double EvaluateSH(int l, int m, double theta, double phi);
};

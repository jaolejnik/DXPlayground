#include <cmath>
#include <random>

#include "MathUtils.h"
#include "Sh.h"

SphericalHarmonics::SphericalHarmonics(int sqrtSampleCount, int bandCount)
{
    GenerateSamples(sqrtSampleCount, bandCount);
}

void SphericalHarmonics::GenerateSamples(int sqrtSampleCount, int bandCount)
{
    int i = 0;
    double oneOverN = 1.0 / sqrtSampleCount;

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<double> random(0, 1);

    for (int a = 0; a < sqrtSampleCount; a++)
    {
        for (int b = 0; b < sqrtSampleCount; b++)
        {
            SHSample *sample = new SHSample;

            double x = (a + random(re)) * oneOverN;
            double y = (b + random(re)) * oneOverN;
            double test = std::sqrt(1.0 - x);
            double theta = 2.0 * std::acos(std::sqrt(1.0 - x));
            double phi = 2.0 * DirectX::XM_PI * y;

            sample->theta = theta;
            sample->phi = phi;
            sample->vector = {
                static_cast<float>(std::sin(theta) * std::cos(phi)),
                static_cast<float>(std::sin(theta) * sin(phi)),
                static_cast<float>(std::cos(theta)),
            };

            for (int l = 0; l < bandCount; l++)
            {
                for (int m = -l; m <= l; m++)
                {
                    int index = l * (l + 1) + m;
                    sample->coeff.push_back(EvaluateSH(l, m, theta, phi));
                }
            }

            m_samples.push_back(sample);
        }
    }
}

// return a point sample of a Spherical Harmonic basis function
// l is the band, range [0..N]
// m in the range [-l..l]
// theta in the range [0..Pi]
// phi in the range [0..2*Pi]
double SphericalHarmonics::EvaluateSH(int l, int m, double theta, double phi)
{
    if (m == 0)
        return K(l, 0) * P(l, m, std::cos(theta));
    else if (m > 0)
        return sqrt2 * K(l, m) * std::cos(m * phi) * P(l, m, cos(theta));
    else
        return sqrt2 * K(l, -m) * std::sin(-m * phi) * P(l, -m, std::cos(theta));
}

SphericalHarmonics::~SphericalHarmonics()
{
    for (SHSample *&sample : m_samples)
        delete sample;
}

// --- UTILITY METHODS REQUIRED TO PRECOMPUT SH ---

// renormalisation constatn for SH function
double SphericalHarmonics::K(int l, int m)
{
    double tmp = ((2.0 * l + 1.0) * factorial[l - m]) /
                 (4.0 * DirectX::XM_PI * factorial[l + m]);
    return sqrt(tmp);
}

// evaluate an Associated Legendre Polunomial at x
double SphericalHarmonics::P(int l, int m, double x)
{
    double pmm = 1.0;
    if (m > 0)
    {
        double somx2 = sqrt((1.0 - x) * (1.0 + x));
        double fact = 1.0;
        for (int i = 1; i <= m; i++)
        {
            pmm *= (-fact) * somx2;
            fact += 2.0;
        }
    }

    if (l == m)
        return pmm;

    double pmmp1 = x * (2.0 * m + 1.0) * pmm;
    if (l == m + 1)
        return pmmp1;

    double pll = 0.0;
    for (int ll = m + 2; ll <= l; ll++)
    {
        pll = ((2.0 * ll - 1.0) * x * pmmp1 - (ll + m - 1.0) * pmm) / (ll - m);
        pmm = pmmp1;
        pmmp1 = pll;
    }

    return pll;
}

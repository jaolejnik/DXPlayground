#include <cmath>
#include <random>

#include "MathUtils.h"
#include "Sh.h"

using namespace DirectX;

SphericalHarmonics::SphericalHarmonics(int sqrtSampleCount, int bandCount)
{
    GenerateSamples(sqrtSampleCount, bandCount);
}

void SphericalHarmonics::GenerateSamples(int sqrtSampleCount, int bandCount)
{
    int i = 0;
    float oneOverN = 1.0f / sqrtSampleCount;

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<float> random(0, 1);

    for (int a = 0; a < sqrtSampleCount; a++)
    {
        for (int b = 0; b < sqrtSampleCount; b++)
        {
            SHSample *sample = new SHSample;

            float x = (a + random(re)) * oneOverN;
            float y = (b + random(re)) * oneOverN;
            float test = std::sqrt(1.0f - x);
            float theta = 2.0f * std::acos(std::sqrt(1.0 - x));
            float phi = 2.0f * DirectX::XM_PI * y;

            sample->theta = theta;
            sample->phi = phi;
            sample->vector = {
                std::sin(theta) * std::cos(phi),
                std::sin(theta) * std::sin(phi),
                std::cos(theta),
            };

            for (int l = 0; l < bandCount; l++)
            {
                for (int m = -l; m <= l; m++)
                {
                    int index = l * (l + 1) + m;
                    sample->coefficients.push_back(EvaluateSH(l, m, theta, phi));
                }
            }

            m_samples.push_back(sample);
        }
    }

    m_encodedResults.resize(m_samples[0]->coefficients.size());
}

// return a point sample of a Spherical Harmonic basis function
// l is the band, range [0..N]
// m in the range [-l..l]
// theta in the range [0..Pi]
// phi in the range [0..2*Pi]
float SphericalHarmonics::EvaluateSH(int l, int m, float theta, float phi)
{
    if (m == 0)
        return K(l, 0) * P(l, m, std::cos(theta));
    else if (m > 0)
        return sqrt2 * K(l, m) * std::cos(m * phi) * P(l, m, cos(theta));
    else
        return sqrt2 * K(l, -m) * std::sin(-m * phi) * P(l, -m, std::cos(theta));
}

void SphericalHarmonics::ProjectToSH(polarFn fn)
{
    for (SHSample *&sample : m_samples)
    {
        int i = 0;
        for (const float &coeff : sample->coefficients)
        {
            m_encodedResults[i] += fn(sample->theta, sample->phi) * coeff;
            i++;
        }
    }

    float factor = SHweight / static_cast<float>(m_samples.size());
    for (auto &result : m_encodedResults)
        result *= factor;
}

DirectX::XMVECTOR SphericalHarmonics::SampleCubemap(
    std::vector<Texture *> &cubemap,
    DirectX::XMFLOAT3 vec)
{
    Faces face;
    float u;
    float v;
    float maxAxis;

    bool isXPositive = vec.x > 0;
    bool isYPositive = vec.y > 0;
    bool isZPositive = vec.z > 0;

    float absX = std::abs(vec.x);
    float absY = std::abs(vec.y);
    float absZ = std::abs(vec.z);

    if (absX >= absY &&
        absX && absZ)
    {
        maxAxis = absX;
        v = vec.y;

        if (isXPositive)
        {
            u = -vec.z;
            face = Faces::PosX;
        }
        else
        {
            u = vec.z;
            face = Faces::NegX;
        }
    }

    if (absY >= absX &&
        absY >= absZ)
    {
        maxAxis = absY;
        u = vec.x;

        if (isYPositive)
        {
            v = -vec.z;
            face = Faces::PosY;
        }
        else
        {
            v = vec.z;
            face = Faces::NegY;
        }
    }

    if (absZ >= absX &&
        absZ >= absY)
    {
        maxAxis = absZ;
        v = vec.y;

        if (isZPositive)
        {
            u = vec.x;
            face = Faces::PosZ;
        }
        else
        {
            u = -vec.x;
            face = Faces::NegZ;
        }
    }

    u = 0.5f * (u / maxAxis + 1.0f);
    v = 0.5f * (v / maxAxis + 1.0f);

    return cubemap[static_cast<int>(face)]->SampleTexture(u, v);
}

void SphericalHarmonics::ProjectCubemapToSH(std::vector<Texture *> &cubemap)
{
    for (SHSample *&sample : m_samples)
    {
        int i = 0;
        for (const float &coeff : sample->coefficients)
        {
            auto test = SampleCubemap(cubemap, sample->vector);
            m_encodedResults[i] += SampleCubemap(cubemap, sample->vector) * coeff;
            i++;
        }
    }

    float factor = SHweight / static_cast<float>(m_samples.size());
    for (auto &result : m_encodedResults)
        result *= factor;
}

SphericalHarmonics::~SphericalHarmonics()
{
    for (SHSample *&sample : m_samples)
        delete sample;
}

// --- UTILITY METHODS REQUIRED TO PRECOMPUTE SH ---

// renormalisation constatn for SH function
float SphericalHarmonics::K(int l, int m)
{
    float tmp = ((2.0f * l + 1.0f) * factorial[l - m]) /
                (4.0f * DirectX::XM_PI * factorial[l + m]);
    return sqrt(tmp);
}

// evaluate an Associated Legendre Polunomial at x
float SphericalHarmonics::P(int l, int m, float x)
{
    float pmm = 1.0f;
    if (m > 0)
    {
        float somx2 = sqrt((1.0f - x) * (1.0f + x));
        float fact = 1.0f;
        for (int i = 1; i <= m; i++)
        {
            pmm *= (-fact) * somx2;
            fact += 2.0f;
        }
    }

    if (l == m)
        return pmm;

    float pmmp1 = x * (2.0f * m + 1.0f) * pmm;
    if (l == m + 1)
        return pmmp1;

    float pll = 0.0f;
    for (int ll = m + 2; ll <= l; ll++)
    {
        pll = ((2.0f * ll - 1.0f) * x * pmmp1 - (ll + m - 1.0f) * pmm) / (ll - m);
        pmm = pmmp1;
        pmmp1 = pll;
    }

    return pll;
}

// Test lighting function from Green's paper
DirectX::XMVECTOR LightPolar(float theta, float phi)
{
    float val = max(0.0f, 5.0f * std::cos(theta) - 4.0f) +
                max(0.0f, -4.0f * std::sin(theta - DirectX::XM_PI) * std::cos(phi - 2.5f) - 3.0f);

    return {val, val, val, val};
}

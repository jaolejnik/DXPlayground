#include <cmath>
#include <random>

#include "Sh.h"

void SphericalHarmonics::GenerateSamples(int sqrt_n_samples, int n_bands)
{
    int i = 0;
    double oneOverN = 1.0 / sqrt_n_samples;

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<double> random(0, 1);

    for (int a = 0; a < sqrt_n_samples; a++)
    {
        for (int b = 0; b < sqrt_n_samples; b++)
        {
            SHSample *sample = new SHSample;

            double x = (a + random(re) * oneOverN);
            double y = (b + random(re) * oneOverN);
            double theta = 2.0 * acos(sqrt(1.0 - x));
            double phi = 2.0 * DirectX::XM_PI * y;

            sample->theta = theta;
            sample->phi = phi;
            sample->vector = {
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta),
            };

            for (int l = 0; l < n_bands; l++)
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

double SphericalHarmonics::EvaluateSH(int l, int m, double theta, double phi)
{
    return 1.0;
}

SphericalHarmonics::~SphericalHarmonics()
{
    for (SHSample *&sample : m_samples)
        delete sample;
}

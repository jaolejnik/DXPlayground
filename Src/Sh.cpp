#include <cmath>
#include <random>

#include "Sh.h"

void SphericalHarmonics::GenerateSamples(int n, int bands)
{
    int i = 0;
    double oneOverN = 1.0 / n;

    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<double> random(0, 1);

    for (int a = 0; a < n; a++)
    {
        for (int b = 0; b < n; b++)
        {
            double x = (a + random(re) * oneOverN);
            double y = (b + random(re) * oneOverN);
            double theta = 2.0 * acos(sqrt(1.0 - x));
            double phi = 2.0 * DirectX::XM_PI * y;

            m_samples[i].spherical = {
                theta,
                phi,
                1.0,
            };
            m_samples[i].vector = {
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta),
            };

            // generate coeffs
        }
    }
}

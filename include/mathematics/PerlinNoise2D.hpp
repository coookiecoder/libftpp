#pragma once

#include <cmath>
#include <vector>

class PerlinNoise2D {
    private:
        std::vector<int> perm;
    public:
        PerlinNoise2D();
        PerlinNoise2D(const PerlinNoise2D &) = default;
        PerlinNoise2D(PerlinNoise2D &&) = default;

        PerlinNoise2D &operator=(const PerlinNoise2D &) = default;
        PerlinNoise2D &operator=(PerlinNoise2D &&) = default;

        ~PerlinNoise2D() = default;

        float sample(float x, float y) const;
};

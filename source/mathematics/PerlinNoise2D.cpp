#include <algorithm>
#include <numeric>
#include <random>
#include <mathematics/PerlinNoise2D.hpp>

static
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static
float dot(int grad, float x, float y) {
    return grad * x + grad * y;
}

static
int grad(int hash) {
    return (hash & 15) - 8;
}

static
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

PerlinNoise2D::PerlinNoise2D() {
    perm.resize(512);
    std::iota(perm.begin(), perm.begin() + 256, 0);  // Fill with 0 to 255

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(perm.begin(), perm.begin() + 256, g);

    // Duplicate the permutation table to avoid wrapping around
    std::copy(perm.begin(), perm.begin() + 256, perm.begin() + 256);
}


float PerlinNoise2D::sample(float x, float y) const {
    // Find unit grid cell coordinates
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    // Relative position within the grid cell
    float xf = x - std::floor(x);
    float yf = y - std::floor(y);

    // Smooth the coordinates using the fade function
    float u = fade(xf);
    float v = fade(yf);

    // Hash coordinates of the 4 corners of the unit square
    int a = perm[X] + Y;
    int b = perm[X + 1] + Y;

    // Dot product for the four corners
    float aa = dot(grad(perm[a]), xf, yf);
    float ba = dot(grad(perm[b]), xf - 1, yf);
    float ab = dot(grad(perm[a + 1]), xf, yf - 1);
    float bb = dot(grad(perm[b + 1]), xf - 1, yf - 1);

    // Interpolate between the four corners
    float x1 = lerp(aa, ba, u);
    float x2 = lerp(ab, bb, u);
    return lerp(x1, x2, v);
}
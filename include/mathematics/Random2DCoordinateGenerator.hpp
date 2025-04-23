#pragma once

class Random2DCoordinateGenerator {
    private:
        long long seedBuffer = 0;
        long long hash(long long x, long long y) const;

    public:
        explicit Random2DCoordinateGenerator(long long seed = 0);
        Random2DCoordinateGenerator(const Random2DCoordinateGenerator& other) = delete;
        Random2DCoordinateGenerator(const Random2DCoordinateGenerator&& other) = delete;

        Random2DCoordinateGenerator& operator=(const Random2DCoordinateGenerator& other) = delete;
        Random2DCoordinateGenerator& operator=(const Random2DCoordinateGenerator&& other) = delete;

        ~Random2DCoordinateGenerator() = default;

        long long seed() const;
        long long operator()(const long long& x, const long long& y) const;
};

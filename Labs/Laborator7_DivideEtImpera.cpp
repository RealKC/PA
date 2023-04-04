#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>

// Rezultate dupa rularea programului:
// All tests returned correct results
// Classic multiply:  -> duration = 5µs (0.005ms)
// Better multiply:  -> duration = 2µs (0.002ms)

using BigInteger = long long;

bool any_failures = false;

#define TEST(expr, expected)                                                                                                          \
    if ((expr) != (expected)) {                                                                                                       \
        any_failures = true;                                                                                                          \
        auto __value = (expr);                                                                                                        \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Test failed, got: " << __value << "(" #expr ") but expected: " #expected "\n"; \
    }

template<typename Callback>
void benchmark(char const* description, Callback callback)
{
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    callback();
    auto end = high_resolution_clock::now();

    auto duration = end - start;

    auto us = duration_cast<microseconds>(duration);

    float const ms_fractional = static_cast<float>(us.count()) / 1000;
    std::cout << description << " -> duration = " << us.count() << "µs (" << ms_fractional << "ms)" << std::endl;
}

BigInteger multiply_classic(BigInteger x, BigInteger y)
{
    if (x == 0 || y == 0) {
        return 0;
    }

    if (x < 10 and y < 10) {
        return x * y;
    }

    int x_digits = std::ceil(std::log10(x));
    int y_digits = std::ceil(std::log10(y));

    auto max_digits = std::max(x_digits, y_digits);
    BigInteger power_of_ten = std::pow(10, max_digits / 2);

    auto xs = x / power_of_ten;
    auto xd = x % power_of_ten;

    auto ys = y / power_of_ten;
    auto yd = y % power_of_ten;

    auto xsys = multiply_classic(xs, ys);
    auto xsyd = multiply_classic(xs, yd);
    auto xdys = multiply_classic(xd, ys);
    auto xdyd = multiply_classic(xd, yd);

    return xsys * std::pow(10, 2 * (max_digits / 2)) + (xsyd + xdys) * power_of_ten + xdyd;
}

BigInteger multiply_better(BigInteger x, BigInteger y)
{
    if (x == 0 || y == 0) {
        return 0;
    }

    if (x < 10 and y < 10) {
        return x * y;
    }

    int x_digits = std::ceil(std::log10(x));
    int y_digits = std::ceil(std::log10(y));

    auto max_digits = std::max(x_digits, y_digits);
    BigInteger power_of_ten = std::pow(10, max_digits / 2);

    auto xs = x / power_of_ten;
    auto xd = x % power_of_ten;

    auto ys = y / power_of_ten;
    auto yd = y % power_of_ten;

    auto xsys = multiply_better(xs, ys);
    auto xdyd = multiply_better(xd, yd);
    auto middle = multiply_better(xs - xd, yd - ys) + xsys + xdyd;

    return xsys * std::pow(10, 2 * (max_digits / 2)) + middle * power_of_ten + xdyd;
}

int main()
{
    TEST(multiply_classic(61'438'521, 94'736'407), 5'820'464'730'934'047);
    TEST(multiply_classic(1, 2), 2);
    TEST(multiply_classic(10, 20), 200);
    TEST(multiply_classic(123'456'78, 123'456'78), 152415765279684);
    TEST(multiply_classic(123'456'789, 123), 15185185047);
    TEST(multiply_classic(1234, 456), 562704);
    TEST(multiply_classic(123456, 789), 97406784);
    TEST(multiply_classic(123, 23), 2829);

    TEST(multiply_better(61'438'521, 94'736'407), 5'820'464'730'934'047);
    TEST(multiply_better(1, 2), 2);
    TEST(multiply_better(10, 20), 200);
    TEST(multiply_better(123'456'78, 123'456'78), 152415765279684);
    TEST(multiply_better(123'456'789, 123), 15185185047);
    TEST(multiply_better(1234, 456), 562704);
    TEST(multiply_better(123456, 789), 97406784);

    if (any_failures) {
        std::cerr << "A test failed!!!" << std::endl;
    } else {
        std::cerr << "All tests returned correct results" << std::endl;
    }

    benchmark("Classic multiply: ", [] { multiply_classic(123'456'789, 123); });
    benchmark("Better multiply: ", [] { multiply_better(123'456'789, 123); });

    return any_failures;
}

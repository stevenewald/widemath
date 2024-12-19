#include "widemath/uint128.hpp"

#include <benchmark/benchmark.h>

#include <limits>

static void BM_WM_ADDITION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    wm::uint128 step = static_cast<__uint128_t>(std::numeric_limits<wm::uint128>::max())
                       / static_cast<__uint128_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (wm::uint128 num = 0; num < std::numeric_limits<wm::uint128>::max() - step;
             num += step) {
            benchmark::DoNotOptimize(num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

static void BM_BUILTIN_ADDITION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    __uint128_t step =
        std::numeric_limits<__uint128_t>::max() / static_cast<__uint128_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (__uint128_t num = 0; num < std::numeric_limits<__uint128_t>::max() - step;
             num += step) {
            benchmark::DoNotOptimize(num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

static void BM_WM_SUBTRACTION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    wm::uint128 step = static_cast<__uint128_t>(std::numeric_limits<wm::uint128>::max())
                       / static_cast<__uint128_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (wm::uint128 num = std::numeric_limits<wm::uint128>::max(); num > step;
             num -= step) {
            benchmark::DoNotOptimize(num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

static void BM_BUILTIN_SUBTRACTION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    __uint128_t step =
        std::numeric_limits<__uint128_t>::max() / static_cast<__uint128_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (__uint128_t num = std::numeric_limits<__uint128_t>::max(); num > step;
             num -= step) {
            benchmark::DoNotOptimize(num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

static void BM_WM_MULTIPLICATION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    uint64_t step =
        std::numeric_limits<uint64_t>::max() / static_cast<uint64_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (wm::uint128 num = 0; num < std::numeric_limits<uint64_t>::max();
             num += step) {
            benchmark::DoNotOptimize(num * num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

static void BM_BUILTIN_MULTIPLICATION(benchmark::State& state)
{
    int64_t num_steps = state.range(0);
    uint64_t step =
        std::numeric_limits<uint64_t>::max() / static_cast<uint64_t>(num_steps);

    for ([[maybe_unused]] auto _ : state) {
        for (__uint128_t num = 0; num < std::numeric_limits<uint64_t>::max();
             num += step) {
            benchmark::DoNotOptimize(num * num);
        }
    }
    state.SetItemsProcessed(num_steps * state.iterations());
}

BENCHMARK(BM_WM_ADDITION)->Arg(128 << 5);
BENCHMARK(BM_BUILTIN_ADDITION)->Arg(128 << 5);
BENCHMARK(BM_WM_SUBTRACTION)->Arg(128 << 5);
BENCHMARK(BM_BUILTIN_SUBTRACTION)->Arg(128 << 5);
BENCHMARK(BM_WM_MULTIPLICATION)->Arg(128 << 5);
BENCHMARK(BM_BUILTIN_MULTIPLICATION)->Arg(128 << 5);

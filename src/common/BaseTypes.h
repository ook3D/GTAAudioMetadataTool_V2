#pragma once

#include <cstdint>
#include <vector>
#include <cmath>
#include "IoUtils.h"
#include <fifo_map.hpp>
#include <nlohmann/json.hpp>

namespace AMT
{
    template <class K, class V, class dummy_compare, class A>
    using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
    using ordered_json = nlohmann::basic_json<my_workaround_fifo_map>;
} // namespace AMT

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "zonemaps.cpp"
#include "zonemaps.h"

using namespace std;

std::vector<int> generatePointQueries(std::vector<int> data, int n) {
  std::vector<int> queries(data.begin(), data.end());

  // add a few elements out of range
  int non_existing_counter = (data.size() * 0.1);
  std::uniform_int_distribution<int> dist{n, (int)(1.8 * n)};
  // Initialize the random_device
  std::random_device rd;
  // Seed the engine
  std::mt19937_64 generator(rd());
  std::set<int> non_existing;
  while (non_existing.size() != non_existing_counter) {
    non_existing.insert(dist(generator));
  }

  queries.insert(queries.end(), non_existing.begin(), non_existing.end());

  // shuffle indexes
  std::random_shuffle(queries.begin(), queries.end());

  return queries;
}

// Simple struct for range
struct Range
{
  int start;
  int end;
};


Range generateRangeQueries(std::vector<int> data, double startRatio, double endRatio){
  // Assume keys are sorted
  int minKey = data[0];
  int maxKey = data[data.size() - 1];

  // Calculate start and end key value
  int startKey = int(minKey + (maxKey - minKey) * startRatio);
  int endKey = int(minKey + (maxKey - minKey) * endRatio);

  // Assign them to a Range
  Range res;
  res.start = startKey;
  res.end = endKey;

  return res;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage: ./main <input_file> <test_case>" << std::endl;
    return 0;
  }

  std::string input_file = argv[1];
  std::string test_case = argv[2];

  if (test_case != "test_pq" && test_case != "test_rq_1" &&
      test_case != "test_rq_2" && test_case != "test_rq_3" &&
      test_case != "test_rq_4") {
    std::cout << "<test_case> argument has to be one of the following: "
                 "test_pq, test_rq_1, test_rq_2, test_rq_3 or test_rq_4"
              << std::endl;
    return 0;
  }

  // read data
  std::ifstream ifs;
  std::vector<int> data;

  ifs.open(input_file, std::ios::binary);
  ifs.seekg(0, std::ios::end);
  size_t filesize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  data.resize(filesize / sizeof(int));
  ifs.read((char *)data.data(), filesize);

  // 1. ----------------------------- initialize zonemap and build
  // ----------------------------- build zonemap
  zonemap<int> zones(data, (unsigned int)data.size() / 100);

  if (test_case == "test_pq") {
    // 2. ----------------------------- point queries
    // -----------------------------
    std::vector<int> queries = generatePointQueries(data, data.size());

    auto start = std::chrono::high_resolution_clock::now();
    // query from zonemaps here

    for (int i = 0; i < queries.size(); i++) {
      zones.query(queries[i]);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    unsigned long long point_query_time = duration.count();
    std::cout << "Time taken to perform point queries from zonemap = "
              << point_query_time << " microseconds" << endl;
  } else if (test_case == "test_rq_1") {
    // 3. ----------------------------- range queries
    // -----------------------------
    auto start = std::chrono::high_resolution_clock::now();
    // range query from zonemaps here

    Range rng = generateRangeQueries(data, 0.1, 0.2);
    zones.query(rng.start, rng.end);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    unsigned long long range_query_time = duration.count();
    std::cout << "Time taken to perform range query 1 from zonemap = "
              << range_query_time << " microseconds" << endl;
  } else if (test_case == "test_rq_2") {
    // 3. ----------------------------- range queries
    // -----------------------------
    auto start = std::chrono::high_resolution_clock::now();
    // range query from zonemaps here
    Range rng = generateRangeQueries(data, 0.3, 0.4);
    zones.query(rng.start, rng.end);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    unsigned long long range_query_time = duration.count();
    std::cout << "Time taken to perform range query 2 from zonemap = "
              << range_query_time << " microseconds" << endl;
  } else if (test_case == "test_rq_3") {
    // 3. ----------------------------- range queries
    // -----------------------------
    auto start = std::chrono::high_resolution_clock::now();
    // range query from zonemaps here
    Range rng = generateRangeQueries(data, 0.5, 0.6);
    zones.query(rng.start, rng.end);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    unsigned long long range_query_time = duration.count();
    std::cout << "Time taken to perform range query 3 from zonemap = "
              << range_query_time << " microseconds" << endl;
  } else if (test_case == "test_rq_4") {
    // 3. ----------------------------- range queries
    // -----------------------------
    auto start = std::chrono::high_resolution_clock::now();
    // range query from zonemaps here
    Range rng = generateRangeQueries(data, 0.7, 0.8);
    zones.query(rng.start, rng.end);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    unsigned long long range_query_time = duration.count();
    std::cout << "Time taken to perform range query 4 from zonemap = "
              << range_query_time << " microseconds" << endl;
  }

  return 0;
}
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
struct Range {
  int start;
  int end;
};

Range generateRangeQueries(std::vector<int> data, double startRatio,
                           double endRatio) {
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

void test_pq(std::vector<int> &data, zonemap<int> &zones) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<int> queries = generatePointQueries(data, data.size());

  // Start
  for (int i = 0; i < queries.size(); i++) {
    zones.query(queries[i]);
  }
  // End

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  unsigned long long elapse = duration.count();
  double avgTime = (double)elapse / queries.size();

  printf("-------------------------\n");
  printf("Point query %d times\n", queries.size());
  printf("Overall time %llu ms\n", elapse);
  printf("Average time: %lf ms\n", avgTime);
}

void test_rq(std::vector<int> &data, zonemap<int> &zones, double startPer,
             double endPer, int times = 100) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<int> queries = generatePointQueries(data, data.size());

  // Start!
  Range rng = generateRangeQueries(data, startPer, endPer);
  for (int i = 0; i < times; i++) {
    zones.query(rng.start, rng.end);
  }
  // End

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  unsigned long long elapse = duration.count();
  double avgTime = (double)elapse / times;

  printf("-------------------------\n");
  printf("Range query %f to %f:\n", startPer, endPer);
  printf("Overall time %llu ms for %d times\n", elapse, times);
  printf("Average time: %lf ms\n", avgTime);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: ./main <input_file> (<test_case>), default all"
              << std::endl;
    return 0;
  }

  std::string input_file = argv[1];
  std::string test_case;
  if (argc == 2) {
    test_case = "all";
  } else {
    test_case = argv[2];
  }

  if (test_case != "all" && test_case != "test_pq" &&
      test_case != "test_rq_1" && test_case != "test_rq_2" &&
      test_case != "test_rq_3" && test_case != "test_rq_4") {
    printf("<test_case> argument has to be one of the following: \n");
    printf("all, test_pq, test_rq_1, test_rq_2, test_rq_3, test_rq_4\n");
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

  // Print data size
  printf("Data size: %d\n", data.size());

  // Initialize zonemap
  zonemap<int> zones(data, (unsigned int)data.size() / 100);

  // Tests
  if (test_case == "test_pq") {
    test_pq(data, zones);
  } else if (test_case == "test_rq_1") {
    test_rq(data, zones, 0.1, 0.2, 100);
  } else if (test_case == "test_rq_2") {
    test_rq(data, zones, 0.3, 0.4, 100);
  } else if (test_case == "test_rq_3") {
    test_rq(data, zones, 0.5, 0.6, 100);
  } else if (test_case == "test_rq_4") {
    test_rq(data, zones, 0.7, 0.8, 100);
  } else if (test_case == "all") {
    test_pq(data, zones);
    test_rq(data, zones, 0.1, 0.2, 100);
    test_rq(data, zones, 0.3, 0.4, 100);
    test_rq(data, zones, 0.5, 0.6, 100);
    test_rq(data, zones, 0.7, 0.8, 100);
  }
  return 0;
}
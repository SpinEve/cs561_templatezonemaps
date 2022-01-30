#include "zonemaps.h"

template <typename T>
zonemap<T>::zonemap(std::vector<T> _elements,
                    unsigned int _num_elements_per_zone) {
  // constructor
  elements = _elements;
  num_elements_per_zone = _num_elements_per_zone;
  num_zones = 0;
}

template <typename T>
void zonemap<T>::build() {
  zone<T> tmpZone = zone<T>();
  tmpZone.elements = std::vector<T>();
  tmpZone.size = 0;

  for (int i = 0; i < elements.size(); i++) {
    tmpZone.elements.push_back(elements[i]);  // element[i:i+size]?
    tmpZone.size++;
    if (elements[i] > tmpZone.max) tmpZone.max = elements[i];
    if (elements[i] < tmpZone.min) tmpZone.min = elements[i];
    if (tmpZone.size >= num_elements_per_zone) {
      zones.push_back(tmpZone);
      num_zones++;
      tmpZone = zone<T>();
      tmpZone.elements = std::vector<T>();
      tmpZone.size = 0;
    }
  }
  if (tmpZone.size > 0) {
    zones.push_back(tmpZone);
    num_zones++;
  }
}

template <typename T>
void zonemap<T>::sort_elements() {}

template <typename T>
bool zonemap<T>::query(T key) {
  // For each zone
  for (int i = 0; i < num_zones; i++) {
    // Check key is in zone range
    if (key >= zones[i].min && key <= zones[i].max) {
      for (int j = 0; j < zones[i].size; j++) {
        // Find it
        if (key == zones[i][j]) return true;
      }
    }
  }
  // Not found
  return false;
}

template <typename T>
std::vector<T> zonemap<T>::query(T low, T high) {
  // Assume the query is [low, high), left close right open
  std::vector<T> res = std::vector<T>();
  
  // Empty range, return
  if (high <= low) return res;

  // For every zone
  for (int i = 0; i < num_zones; i++) {
    // Skip zone not within range
    if (high <= zones[i].min) continue;
    if (low > zones[i].max) continue;

    // Add elements in range
    for (int j = 0; j < zones[i].size; j++) {
      if (zones[i][j] >= low && zones[i][j] < high) {
        res.push_back(zones[i][j]);
      }
    }
  }
  return res;
}

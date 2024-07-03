#pragma once
#include <random>
using namespace std;

// Inline injects this code into wherever it's called instead of referencing it, becuase these functions are outside of a class
// so if 2 files include Random.h they would be defined multiple times otherwise

inline int random() { return rand(); }
inline int random(unsigned int max) { return rand() % max; } // Max is exclusive
inline int random(int min, int max) { return min + random(max - min); }

inline float randomf() { return rand() / (float) RAND_MAX; } // At least one of the inputs needs to be a float so the output is a float
inline float randomf(float max) { return max * randomf(); }
inline float randomf(float min, float max) { return min + randomf(max - min); }
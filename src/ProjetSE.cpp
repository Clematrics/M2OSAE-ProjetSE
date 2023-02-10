//============================================================================
// Name        : ProjetSE.cpp
// Author      : Clément Devatine & Hadrien Caumont
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
using namespace std;

#define assertm(expected, msg) assert(((void)msg, expected));

#define TEST

#include "const.h"
#include "data.h"
#include "SortEngine.h"
#include "test.hpp"

/// Return whether the program loop should continue or not
/// For now, does 'uiSimCount' iterations for testing purposes
/// TODO: to change for the final shipping!!
//bool runFlag() {
//	static unsigned int iterCount = uiSimCount;
//	if (iterCount == 0) return false;
//	iterCount--;
//	return true;
//}

float Vref[uiSensorSize] = { 0 };

static auto sortEngine = SortEngine();

void body(size_t iteration) {
	// Data acquisition
	// The data is arranged in the following way : all the first pixels of each sensor are placed on the first line
	// Thus, the data from the sensor is interlaced.
	// After acquisition, the data must be rearranged
	// (or the data could be arranged directly during the transfer from the sensors, but more costly on their end probably)
	unsigned short V[uiSensorSize][uiSensorCount] = { 0 };
	#ifdef TEST
	// Get the data from the samples and rearrange it
	for (size_t sensor = 0; sensor < uiSensorCount; sensor++) {
		for (size_t pixel = 0; pixel < uiSensorSize; pixel++) {
			V[pixel][sensor] = p_usSrcData[iteration][sensor][pixel];
		}
	}
	#endif

	// Median value computed element-by-element
	float Vm[uiSensorSize] = { 0 };
	for (size_t k = 0; k < uiSensorSize; k++) {
		// Multiple options to test
		// First is using stdlib algorithms
//		std::sort(V[k], V[k] + uiSensorCount);
		sortEngine.sort(V[k], uiSensorCount);

		assertm(uiSensorCount != 0, "The number of sensors must be non zero, otherwise, the mean cannot be computed");
		Vm[k] = V[k][uiSensorCount / 2 - 1];
//		if (uiSensorCount % 2 == 0) {
//			Vm[k] = 0.5 * (V[k][uiSensorCount / 2] + V[k][uiSensorCount / 2 - 1]);
//		} else {
//			Vm[k] = V[k][uiSensorCount / 2];
//		}
	}

	// Mean value computed element-by-element, with rejection
	float Vmu[uiSensorSize] = { 0 };
	for (size_t k = 0; k < uiSensorSize; k++) {
		float sum = 0.f;
		size_t count = 0;
		for (size_t sensor = 0; sensor < uiSensorCount; sensor++) {
			auto value = V[k][sensor];
			if (Vm[k] - usGrantedError < value && value < Vm[k] + usGrantedError) {
				count++;
				sum += value;
			}
		}
		Vmu[k] = sum / count;
//		unsigned short* begin = V[k];
//		unsigned short* end = V[k] + uiSensorCount;
//		// Find bounds
//		// Find the first element v such that v > Vm[k] - usGrantedError, or the end iterator if not found
//		auto leftBound = std::upper_bound(begin, end, Vm[k] - usGrantedError);
//		assertm(leftBound != end, "The left bound iterator should not be at the end. Is usGrantedError equal to zero?");
//		// Getting the end iterator is not possible in this case, except if usGrantedError == 0 or if the data is altered or unsorted
//
//		// Find the first element v such that v >= Vm[k] + usGrantedError, or the end iterator if not found
//		// Getting the end iterator IS possible, and is not a problem if returned
//		auto rightBound = std::lower_bound(begin, end, Vm[k] + usGrantedError);
//
//		// Sum between bounds
//		Vmu[k] = std::accumulate(leftBound, rightBound, 0.0);
//		auto elements = std::distance(leftBound, rightBound);
//		assertm(elements != 0, "The left and right bound are equal, which should not happen. The sorted array V[k] was probably modified");
//		Vmu[k] /= elements; // Normalisation
	}

	if (iteration == 0) {
		// For the first iteration, compute Vref
		std::copy_n(Vmu, uiSensorSize, Vref);
	} else {
		float Vs[uiSensorSize] = { 0 };
		std::transform(Vmu, Vmu + sizeof(Vmu), Vref, Vs, [](auto vmu, auto ref){ return max(vmu - ref, 0.f); });
		size_t hotspots = 0;
		for (size_t i = 0; i < uiSensorSize; i++) {
			if (Vs[i] > usHotspotDetectionThreshold) {
				hotspots++;
			}
		}
//		auto hotspots = std::count_if(Vs, Vs + sizeof(Vs), [](auto vs){ return vs > usHotspotDetectionThreshold; });

		#ifdef TEST
		std::cout << "Sample " << iteration << " - Hotspots: " << hotspots << "\n";
		#endif

		// Vs should be stored to be transmitted to Earth? later
	}
}

int main() {
	// TODO: to test with or without
	// sortEngine.setSortType(SortEngine::QUICK);

	test("start", 0);

	// First iteration is special: computes the reference vector
	for (size_t iteration = 0; iteration < uiSimCount; iteration++) {
		body(iteration);
	}

	test("end", 2);

	return 0;
}

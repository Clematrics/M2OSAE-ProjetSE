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

#ifdef TEST
#define assertm(expected, msg) assert(((void)msg, expected));
#else
#define assertm(expected, msg)
#endif

#define SAMPLEDATA
#define PROFILE

#include "const.h"
#include "data.h"
#include "SortEngine.h"
#include "test.hpp"

float Vref[uiSensorSize] = { 0 };

static auto sortEngine = SortEngine();

void body_batch(size_t iteration) {
	// Data acquisition
	// The data is arranged in the following way : all the first pixels of each sensor are placed on the first line
	// Thus, the data from the sensor is interlaced.
	// After acquisition, the data must be rearranged
	// (or the data could be arranged directly during the transfer from the sensors, but more costly on their end probably)
	unsigned short V[uiSensorSize][uiSensorCount] = { 0 };
	#ifdef SAMPLEDATA
	// Get the data from the samples and rearrange it
	for (size_t sensor = 0; sensor < uiSensorCount; sensor++) {
		for (size_t pixel = 0; pixel < uiSensorSize; pixel++) {
			V[pixel][sensor] = p_usSrcData[iteration][sensor][pixel];
		}
	}
	#endif
	#ifdef PROFILE
	test("pixel rearrangment", 1);
	#endif

	// Median value computed element-by-element
	float Vm[uiSensorSize] = { 0 };
	for (size_t k = 0; k < uiSensorSize; k++) {
		// std::sort(V[k], V[k] + uiSensorCount);
		sortEngine.sort(V[k], uiSensorCount);

		assertm(uiSensorCount != 0, "The number of sensors must be non zero, otherwise, the median cannot be computed");
		if (uiSensorCount % 2 == 0) {
			Vm[k] = (V[k][uiSensorCount / 2] + V[k][uiSensorCount / 2 - 1]) / 2;
		} else {
			Vm[k] = V[k][uiSensorCount / 2];
		}
	}
	#ifdef PROFILE
	test("median computation", 1);
	#endif

	// Mean value computed element-by-element, with rejection
	float Vmu[uiSensorSize] = { 0 };
	for (size_t k = 0; k < uiSensorSize; k++) {
		// naive method
		float sum = 0;
		size_t count = 0;
		for (size_t sensor = 0; sensor < uiSensorCount; sensor++) {
			auto value = V[k][sensor];
			if (value - Vm[k] < usGrantedError && Vm[k] - value < usGrantedError) {
				count++;
				sum += value;
			}
		}
		// The division by zero should not happen, except in a very specific case:
		// in case the number of sensors is even, the computed median is
		// 	 M = (V[k][uiSensorCount / 2] + V[k][uiSensorCount / 2 - 1]) / 2
		// However, if V[k][uiSensorCount / 2] - M and M - V[k][uiSensorCount / 2 - 1] are
		// greater than usGrantedError, then no value checks the condition
		//   value - Vm[k] < usGrantedError && Vm[k] - value < usGrantedError
		// thus the count variable is never incremented
		Vmu[k] = sum / count;

		// non naive method
		/* unsigned short* begin = V[k];
		unsigned short* end = V[k] + uiSensorCount;
		// Find bounds
		// Find the first element v such that v > Vm[k] - usGrantedError, or the end iterator if not found
		auto leftBound = std::upper_bound(begin, end, Vm[k] - usGrantedError);
		assertm(leftBound != end, "The left bound iterator should not be at the end. Is usGrantedError equal to zero?");
		// Getting the end iterator is not possible in this case, except if usGrantedError == 0 or if the data is altered or unsorted

		// Find the first element v such that v >= Vm[k] + usGrantedError, or the end iterator if not found
		// Getting the end iterator IS possible, and is not a problem if returned
		auto rightBound = std::lower_bound(begin, end, Vm[k] + usGrantedError);

		// Sum between bounds
		Vmu[k] = std::accumulate(leftBound, rightBound, 0.0);
		auto elements = std::distance(leftBound, rightBound);
		assertm(elements != 0, "The left and right bound are equal, which should not happen. The sorted array V[k] was probably modified");
		Vmu[k] /= elements; // Normalisation */
	}
	#ifdef PROFILE
	test("mean computation", 1);
	#endif

	if (iteration == 0) {
		// For the first iteration, compute Vref
		std::copy_n(Vmu, uiSensorSize, Vref);
	} else {
		float Vs[uiSensorSize] = { 0 };
		std::transform(Vmu, Vmu + uiSensorSize, Vref, Vs, [](auto vmu, auto ref){ return max(vmu - ref, 0.f); });
		size_t hotspots = 0;
		for (size_t i = 0; i < uiSensorSize; i++) {
			if (Vs[i] > usHotspotDetectionThreshold) {
				hotspots++;
			}
		}
		// auto hotspots = std::count_if(Vs, Vs + uiSensorSize, [](auto vs){ return vs > usHotspotDetectionThreshold; });

		#ifdef TEST
		if (hotspots != p_usHotspots[iteration])
			std::cout << "Sample " << iteration << " - Hotspots expected: " << p_usHotspots[iteration] << "; measured: " << hotspots << "\n";
		#endif
	}
	#ifdef PROFILE
	test("Vref / hotspots computation", 1);
	#endif
}


void body(size_t iteration) {
	size_t hotspots = 0;
	float Vs[uiSensorSize] = { 0.f };

	for (size_t pixel = 0; pixel < uiSensorSize; pixel++) {
		// Data acquisition
		// The data is arranged in the following way : all the first pixels of each sensor are placed on the first line
		// Thus, the data from the sensor is interlaced.
		// After acquisition, the data must be rearranged
		// (or the data could be arranged directly during the transfer from the sensors, but more costly on their end probably)
		unsigned short V[uiSensorCount] = { 0 };
		#ifdef SAMPLEDATA
		// Get the data from the samples and rearrange it
		for (size_t sensor = 0; sensor < uiSensorCount; sensor++) {
			V[sensor] = p_usSrcData[iteration][sensor][pixel];
		}
		#endif

		// Median value computed element-by-element
		float Vm = 0.f;
		sortEngine.sort(V, uiSensorCount);

		assertm(uiSensorCount != 0, "The number of sensors must be non zero, otherwise, the median cannot be computed");
		if (uiSensorCount % 2 == 0) {
			Vm = (V[uiSensorCount / 2] + V[uiSensorCount / 2 - 1]) / 2;
		} else {
			Vm = V[uiSensorCount / 2];
		}
		
		// Mean value computed element-by-element, with rejection
		float Vmu = 0.f;
		// non naive method
		unsigned short* begin = V;
		unsigned short* end = V + uiSensorCount;
		// Find bounds
		// Find the first element v such that v > Vm[k] - usGrantedError, or the end iterator if not found
		auto leftBound = std::upper_bound(begin, end, Vm - usGrantedError);
		assertm(leftBound != end, "The left bound iterator should not be at the end. Is usGrantedError equal to zero?");
		// Getting the end iterator is not possible in this case, except if usGrantedError == 0 or if the data is altered or unsorted

		// Find the first element v such that v >= Vm[k] + usGrantedError, or the end iterator if not found
		// Getting the end iterator IS possible, and is not a problem if returned
		auto rightBound = std::lower_bound(begin, end, Vm + usGrantedError);

		// Sum between bounds
		Vmu = std::accumulate(leftBound, rightBound, 0.0);
		auto elements = std::distance(leftBound, rightBound);
		assertm(elements != 0, "The left and right bound are equal, which should not happen. The sorted array V[k] was probably modified");
		Vmu /= elements; // Normalisation

		if (iteration == 0) {
			// For the first iteration, compute Vref
			Vref[pixel] = Vmu;
		} else {
			Vs[pixel] = max(Vmu - Vref[pixel], 0.f);
			if (Vs[pixel] > usHotspotDetectionThreshold) {
				hotspots++;
			}
		}
	}

	#ifdef TEST
	if (hotspots != p_usHotspots[iteration])
		std::cout << "Sample " << iteration << " - Hotspots expected: " << p_usHotspots[iteration] << "; measured: " << hotspots << "\n";
	#endif
}

#include <time.h>
#include <stdlib.h>

int main() {
	sortEngine.setSortType(SortEngine::HEAP);

	srand(13412324);
	unsigned short arr[uiSensorCount] = {0};
	for (size_t i = 0, v = uiSensorCount; i < uiSensorCount; i++, v--)
		arr[i] = rand() % 10000;

	#ifdef PROFILE
	test("start", 0);
	#endif

	sortEngine.sort(arr, uiSensorCount);
	test("iteration", 1);

	// First iteration is special: computes the reference vector
	for (size_t iteration = 0; iteration < uiSimCount; iteration++) {
		// body_batch(iteration);
		// body(iteration);
		// #ifdef PROFILE
		// test("iteration", 1);
		// #endif
	}

	#ifdef TEST
	std::cout << std::flush;
	#endif
	#ifdef PROFILE
	test("end", 2);
	#endif

	return 0;
}

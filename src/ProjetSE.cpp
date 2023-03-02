//============================================================================
// Name        : ProjetSE.cpp
// Author      : Clément Devatine & Hadrien Caumont
// Version     :
// Copyright   : 
// Description : Embedded systems project
//============================================================================

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
using namespace std;

// #define SAMPLEDATA
// #define PROFILE

#ifdef TEST
#define assertm(expected, msg) assert(((void)msg, expected));
#else
#define assertm(expected, msg)
#endif

#include "const.h"
#include "data.h"
#include "SortEngine.h"
#include "test.hpp"

// Reference vector
float Vref[uiSensorSize] = { 0 };

static auto sortEngine = SortEngine();

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

int main() {
	sortEngine.setSortType(SortEngine::QUICK);

	#ifdef PROFILE
	test("start", 0);
	#endif

	// First iteration is special: computes the reference vector
	for (size_t iteration = 0; iteration < uiSimCount; iteration++) {
		#ifdef PROFILE
		__asm__ volatile("" : "+g" (iteration) : :);
		#endif
		body(iteration);
		#ifdef PROFILE
		test("iteration", 1);
		#endif
	}

	#ifdef TEST
	std::cout << std::flush;
	#endif
	#ifdef PROFILE
	test("end", 2);
	#endif

	return 0;
}

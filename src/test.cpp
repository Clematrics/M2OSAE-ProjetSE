/*
 * test.cpp
 *
 *  Created on: Feb 10, 2023
 *      Author: clement
 */


int test(const char *testName, int testId) {
	int static testCounter = 0;
	return testCounter++;
}

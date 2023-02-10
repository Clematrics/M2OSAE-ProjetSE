/*************************************************************************

 Copyright (c) 2009 by CNRS/LESIA

 This software is copyrighted by and is the sole property of CNRS/LESIA.
 All rights, title, ownership, or other interests in the software remain
 the property of CNRS/LESIA.
 This software may only be used in accordance with the corresponding
 license agreement. Any unauthorized use, duplication, transmission,
 distribution, or disclosure of this software is expressly forbidden.

 This Copyright notice may not be removed or modified without prior
 written consent of CNRS/LESIA.


 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 SortEngine Component

 SortEngine.cpp

 $Rev: 13 $

 ************************************************************************/

#include "SortEngine.h"
#include "const.h"
#include "stdio.h"
#include "test.h"

/**
 *
 * Macro used to swap two value.
 * N.B.: the "temp" variable should be defined and accessible from where the macro is called
 */
#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;


SortEngine::SortEngine() {
	// By default, the sorting algorithm is Heapsort
	m_sortType=HEAP;
}

SortEngine::~SortEngine() {

}


void SortEngine::sort(unsigned short * toSort, unsigned int len){
//Define TESTSORT to enable sorting efficiency measurment
#ifdef TESTSORT
	test("Sorting start",0);
#endif
	switch(m_sortType){
	case HEAP: 	heapSort(toSort,len);
	break;
	case QUICK: quickSort(toSort,len);
	break;
	}
#ifdef TESTSORT
	test("Sorting finished",2);
#endif
	return;
}

void SortEngine::heapSort(unsigned short * toSort, unsigned int len){
	/**
	 * Sort an array of unsigned short using the Heapsort algorithm
	 *
	 * Adapt from "Numerical Recipe in C"
	 * See www.nr.com for original sources
	 *
	 */

	unsigned long parent,child,heapSize,l=0;
	unsigned short savedValue;

	if (len < 2) return;
	l=(len >> l);
	heapSize=len;

	for (;;) {
		if (l > 0) {
			savedValue=toSort[--l];
		}
		else {
			if (--heapSize == 0){
				break;
			}
			savedValue=toSort[heapSize];
			toSort[heapSize]=toSort[0];
		}
		parent=l;
		child=(l<<1)+1;
		while (child < heapSize) {
			if (child+1 < heapSize && toSort[child] <toSort[child+1])
				child++;
			if (savedValue < toSort[child]) {
				toSort[parent]=toSort[child];
				parent=child;
				child=(child<<1)+1;

			}
			else{
				break;
			}
		}
		toSort[parent]=savedValue;
	}
}
void SortEngine::quickSort(unsigned short * toSort, unsigned int len){
	/**
	 * Sort an array of unsigned short using the Quicksort algorithm
	 *
	 * Adapt from "Numerical Recipe in C"
	 * See www.nr.com for original sources
	 *
	 */

	/**
	 * This const define the threshold in array size between quicksort and classical insertion sort
	 * Has the quicksort consist in recursive subarray partitionning and sorting,
	 * as soon as sub array size came to this limit the insertion sorting algorithm is used
	 */
	const unsigned int uiInsertionSortLimit=7;

	unsigned int i,ir=len,j,pivotIndex,l=1;
	int stackIndex=0;
	unsigned short stack[uiSensorSize];
	unsigned short a,temp;

	for (;;) {
		if (ir-l < uiInsertionSortLimit) { //small array, using insertion sort
			for (j=l+1;j<=ir;j++) {
				a=toSort[j-1];
				for (i=j-1;i>=l;i--) {
					if (toSort[i - 1] <= a) break;
					toSort[i]=toSort[i - 1];

				}
				toSort[i]=a;
			}
			if (stackIndex == 0) break;
			ir=stack[stackIndex--];
			l=stack[stackIndex--];
		}
		else {
			pivotIndex=(l+ir) >> 1;
			SWAP(toSort[pivotIndex-1],toSort[l])
			if (toSort[l-1] > toSort[ir-1]) {
				SWAP(toSort[l-1],toSort[ir-1])
			}
			if (toSort[l] > toSort[ir-1]) {
				SWAP(toSort[l],toSort[ir-1])
			}
			if (toSort[l-1] > toSort[l]) {
				SWAP(toSort[l-1],toSort[l])
			}
			i=l+1;
			j=ir;
			a=toSort[l];
			for (;;) {
				do i++; while (toSort[i-1] < a);
				do j--; while (toSort[j-1] > a);
				if (j < i) break;
				SWAP(toSort[i-1],toSort[j-1]);
			}
			toSort[l]=toSort[j-1];
			toSort[j-1]=a;
			stackIndex += 2;
			if (stackIndex > uiSensorSize){
				// The stack array size has been set in order to avoid this failure
				printf("NSTACK too small in sort.");
				return ;
			}
			if (ir-i+1 >= j-l) {
				stack[stackIndex]=ir;
				stack[stackIndex-1]=i;
				ir=j-1;
			}
			else {
				stack[stackIndex]=j-1;
				stack[stackIndex-1]=l;
				l=i;
			}
		}
	}
}

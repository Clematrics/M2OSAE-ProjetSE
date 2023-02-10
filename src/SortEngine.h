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

	SortEngine.h

	$Rev: 13 $

************************************************************************/

/************************************************************************

	COMPONENT DEFINITION							RELEASE

	SortEngine.h									1.0.0.0

	AUTHOR

	Loïc Gueguen, LESIA.

	DESCRIPTION

	This file provide the interface of a class design for sorting processes

	RELEASE HISTORY

	DATE				NAME							DESCRIPTION

    18 janv. 2010		Loïc Gueguen				Initial Version 1.0.0.0

************************************************************************/

#ifndef SORT_H_
#define SORT_H_

class SortEngine {
public:
	/**
	 * Kind of sort algorithm available (HEAP: Heapsort, QUICK: Quicksort)
	 *
	 */
	enum SortType {HEAP,QUICK};

	/**
	 *
	 * @return
	 */
	SortEngine();
	/**
	 *
	 * @return
	 */
	virtual ~SortEngine();
	/**
	 * Sort an array of datas in ascending order
	 *
	 * @param toSort Array to sort
	 * @param len Size of the array to sort
	 */
	void sort(unsigned short * toSort, unsigned int len);
    /**
     * Modify the kind of sorting algorithm used
     *
     * @param type
     */
	void setSortType(SortType type){m_sortType = type;}
private:
	void heapSort(unsigned short * toSort, unsigned int len);
	void quickSort(unsigned short * toSort, unsigned int len);
	SortType m_sortType;
};

#endif /* SORT_H_ */

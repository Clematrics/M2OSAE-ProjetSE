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

	const.h

	$Rev: 13 $

************************************************************************/

/************************************************************************

	COMPONENT DEFINITION							RELEASE

	const.h											1.0.0.0

	AUTHOR

	Loïc Gueguen, LESIA.

	DESCRIPTION

	This file defines the constants of the application

	RELEASE HISTORY

	DATE				NAME							DESCRIPTION

    18 janv. 2010		Loïc Gueguen				Initial Version 1.0.0.0

************************************************************************/

#ifndef CONST_H_
#define CONST_H_

#include "SortEngine.h"

/**
 * Sensor size (in pixel)
 */
const unsigned int uiSensorSize  					= 100;
/**
 * Number of sensor
 */
const unsigned int uiSensorCount 					= 50;
/**
 * Number of sample generated
 */
const unsigned int uiSimCount    					= 50;
/**
 * Granted error (i.e. see "err" value in point 3 of scientific description)
 */
const unsigned short usGrantedError					= 100 ;
/**
 * Threshold for Hotspot detection see "s" value in point 5.2 of scientific description)
 */
const unsigned short usHotspotDetectionThreshold	= 200;

#endif /* CONST_H_ */

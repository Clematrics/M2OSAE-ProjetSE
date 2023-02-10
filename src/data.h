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

	data.h

	$Rev: 13 $

************************************************************************/

/************************************************************************

	COMPONENT DEFINITION							RELEASE

	data.h											1.0.0.0

	AUTHOR

	Loïc Gueguen, LESIA.

	DESCRIPTION

	This file defines static test data

	RELEASE HISTORY

	DATE				NAME							DESCRIPTION

     18 janv. 2010		Loïc Gueguen				Initial Version 1.0.0.0

************************************************************************/

#ifndef DATA_H_
#define DATA_H_

#include "const.h"

/**
 * Array providing sample of simulated datas
 */
extern unsigned short p_usSrcData[uiSimCount][uiSensorCount][uiSensorSize];

#endif /* DATA_H_ */

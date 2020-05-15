/*
 *   ---============================---
 * ---===     LoTd NT3 Library     ===---
 *   ---============================---
 *
 *   This library contains the files that
 * are needed to use the NT3 library by 
 * including this one file for simplicity.
 * 
 */

#include <iostream>
#include <fstream>
#include <windows.h>
#include <sstream>

//Used by the console for drawing lines.
#include <cstdlib>

using namespace std;

//--     Includes for this library.

//--  Low Level Classes.

//Include for the definitions used in the NeuroTox.2 such as u_Data.
#include "Definitions.h"

//For working with console graphics.
#include "Console_Graphics/Console_Graphics.h"

//Includes the binary (and non-binary) trees.
#include "Bin_Trees/Bin_Trees.h"

//Includes the utilities such as the memory mapping class.
#include "Utilities/Utilities.h"

//--  High Level Classes (Dependent upon lower classes)

//This include allows access to the table handler and the tables.
#include "Tables/Tables.h"

//--  NT3 Classes (Dependent on LoTd classes)

//Includes for the Neurotox.3 library.
#include "NeuroTox.3/NeuroTox.3.h"

//The init routines for libraries contained within such as console.
//Must be called or the console functions will not hook in properly.
void init_LoTd()
{
     init_Console();
	 set_Console_Color(0, 10);
}


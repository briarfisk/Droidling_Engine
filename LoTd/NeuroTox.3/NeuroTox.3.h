/*
 *   This is the main header file for the NeuroTox library. The NeuroTox library contains
 * classes that allow one to work with memory trees through a black box. Memory trees can
 * be built, queried, linked together, and hooked to ports for IO.
*/


//==--  Types of objects in sandbox.
#define NT3INPUT1D 0
#define NT3INPUT2D 1
#define NT3OUTPUT1D 10
#define NT3OUTPUT2D 11
#define NT3CONSTRUCT 20
#define NT3CONSTRUCT1D 21
#define NT3CONSTRUCT2D 22
#define NT3ASSEMBLYMSC 50
#define NT3ASSEMBLYSSCC 51
#define NT3ASSEMBLYCHRONO 52

//==--  Types of Pipes.

//Only raw constructs accept more than one dimensino of input.
//All other data is normalized to 1D through Treetop Creation.
#define NT3PIPE_IN__OUT 0
#define NT3PIPE_IN__CON 1
#define NT3PIPE_IN__ASM 2

#define NT3PIPE_CON__OUT 10
#define NT3PIPE_CON__CON 11
#define NT3PIPE_CON__ASM 12

#define NT3PIPE_ASM__OUT 20
#define NT3PIPE_ASM__CON 21
#define NT3PIPE_ASM__ASM 22




#define NT3_Angel_In_Con 0
#define NT3_Angel_Con_Con 1
#define NT3_Angel_Out_Con 2

#define NT3_Mortal_In_Out 10
#define NT3_Mortal_In_In 11
#define NT3_Mortal_Con_Con 12
#define NT3_Mortal_Out_Out 13
#define NT3_Mortal_Out_In 14

#define NT3_Demon_Con_In 20
#define NT3_Demon_Con_Con 21
#define NT3_Demon_Con_Out 22

#define NT3_Basic 0
#define NT3_Construct 1

//The base NT3 class that allows for hooking into some basic API manipulations with polymorphic classes.
class c_NT3_Base
{
private:

public:

    int X;
    int Y;
    int Width;
    int Height;

    //When value is -1 treat as NULL.
    int Object_ID;

    c_NT3_Base()
    {
        X = 0;
        Y = 0;
        Width = 0;
        Height = 0;
        Object_ID = -1;
    }

    ~c_NT3_Base()
    {

    }
};

//--     Includes for this library.


//Includes for the base nodes.
#include "Nodes/c_NT3_Node_MMap_1D.h"

//Includes the state tree.
#include "Bin Trees/c_NT3_Fractal_Tree_MMap.h"

//Includes for the Node Network.
#include "Nodes/c_NT3_Node_Network_MMap_1D.h"

//Includes the buffer tree.
#include "Buffer/c_NT3_Buffer_MMap.h"
#include "Buffer/c_NT3_Data_Buffer.h"

//Includes the Vision_buffer tree.
#include "Buffer/c_NT3_Vision_Buffer_MMap.h"

//Includes for the CAN.
#include "CAN/c_NT3_CAN_MMap_1D.h"

//Includes for the Constructs.
#include "Constructs/c_NT3_Base_Construct.h"
#include "Constructs/c_NT3_Construct_MMap_1D.h"

//Includes for the API.
#include "API/c_NT3_Pipes.h"

//Includes for the Assemblies.
#include "Assemblies/c_NT3_SANDBOX.h"

//Includes for the NT3 Engine.
#include "CMD/c_NT3_Command_Parser.h"
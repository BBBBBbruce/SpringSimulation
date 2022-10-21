// SpringSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "VR_Application.h"


#include "HookeSpring.h"


#include "GraphicsRenderer.h"
#include <windows.h>
#include <Xinput.h>

#include "openvr.h"

#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")

#define INPUT_DEADZONE 2

using namespace std;

void xboxControllerTest();

int main(int argc, char* argv[])
{

	//renderLoop();


    //xboxControllerTest();

    //Vive controller test
    std::cout << "#######################################" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "program start" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "#######################################" << std::endl;
    std::cout << "program start" << std::endl;
    std::cout << "#######################################" << std::endl;
    VR_Application* pMainApplication = new VR_Application(argc, argv);

    if (!pMainApplication->BInit())
    {
        pMainApplication->Shutdown();
        return 1;
    }

    pMainApplication->RunMainLoop();

    pMainApplication->Shutdown();




    return 0;
}

void xboxControllerTest() {
    //controller setup
// one controller supports now
    XINPUT_STATE state;
    DWORD dwResult = XInputGetState(0, &state);
    float lt;
    float force;

    //use cm and newton as units
    // spring constant 4 N/cm
    // original length 2.5 cm
    // maxforce before break 6 N
    Spring spring1 = Spring(4, 2.5, 6);

    //Assume max press force is 5N when LT/RT reaches 255


    while (true)
    {

        dwResult = XInputGetState(0, &state);
        lt = state.Gamepad.bLeftTrigger;

        cout << endl << "============" << endl;

        cout << "Raw left trigger input: " << lt << endl;
        force = lt / 255 * 5;
        spring1.set_externalforce(force);

        cout << "spring now has length: " << spring1.get_CurrentLength() << " and under pressed by external force of " << spring1.get_currentforce() << endl;;
        cout << endl << "============" << endl;
        if (GetAsyncKeyState(VK_ESCAPE)) break;
        Sleep(100);

    }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

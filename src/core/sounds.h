//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_SOUNDS_H
#define INC_4GEWINNT_CLIENT_SOUNDS_H

#include<iostream>
#include<windows.h>

#pragma comment(lib, "winmm.lib")

class sounds {
public:
    static void playsound(std::string path, std::string soundname);
private:
};


#endif //INC_4GEWINNT_CLIENT_SOUNDS_H
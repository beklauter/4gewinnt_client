//
// Created by beklauter on 04.02.2026.
//

#include "sounds.h"

void sounds::playsound(std::string path, std::string soundname) {
    const std::string a1 = path + soundname;

    PlaySound((a1.c_str()),NULL,SND_SYNC);
}

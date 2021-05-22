#include "types.h"
#include "user.h"

int main(){
    // Using video mode 13
    setvideomode(0x13);

    // Scatters some pixels using one call to the kernel per pixel
    for (int x = 0; x < 320; x += 5){
        for (int y = 0; y < 200; y += 5){
            setpixel(x, y, 5);
        }
    }

    // Draws some lines using one call to the kernel per line
    for (int y = 0; y < 200; y += 10){
        drawline(0, y, 319, 199 - y, 11);
    }

    exit();
}

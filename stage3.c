#include "types.h"
#include "user.h"

int main(){
    // Using video mode 12
    setvideomode(0x12);

    // Scatters some pixels using one call to the kernel per pixel
    for (int x = 0; x < 640; x+=10)
    {
        for (int y = 0; y < 480; y+=10)
        {
            setpixel(x, y, 15);
        }
    }

    // Draws some lines using one call to the kernel per line
    for (int i = 0; i < 16; i++)
    {
        drawline(0, i * 10, 639, 479 - (i * 10), i);
        drawline(639, i * 10, 0, 479 - (i * 10), i);
    }

    // Return to video mode 3 (text console) after key is pressed
    getch();
    setvideomode(0x03);

    exit();
}

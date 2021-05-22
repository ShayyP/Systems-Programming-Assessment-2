#include "types.h"
#include "user.h"

int main(){
    // Using video mode 12
    setvideomode(0x12);

    // Start batching calls
    begingraphics();

    // Scatters some pixels, adding requests to the batch (setpixel function handles this automatically)
    for (int x = 0; x < 640; x+=10)
    {
        for (int y = 0; y < 480; y+=10)
        {
            setpixel(x, y, 15);
        }
    }

    // Adds requests to draw some lines to the batch (drawline function handles this automatically)
    for (int i = 0; i < 16; i++)
    {
        drawline(0, i * 10, 639, 479 - (i * 10), i);
        drawline(639, i * 10, 0, 479 - (i * 10), i);
    }
    
    // Dispatch batched calls
    endgraphics();

    // Return to video mode 3 (text console) after key is pressed
    getch();
    setvideomode(0x03);

    exit();
}
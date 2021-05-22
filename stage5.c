#include "types.h"
#include "user.h"

int main(){
    // Using video mode 12
    setvideomode(0x12);

    // Start batching calls
    begingraphics();

    // Adds requests to draw some circles to the batch
    for (int i = 0; i < 16; i++)
    {
        drawcircle(320, 240, (i + 1) * 10, i);
    }

    // Adds requests to draw rectangles to the batch
    fillrect(200, 10, 200, 50, 10);
    fillrect(10, 150, 100, 200, 12);

    // Adds requests to draw mutliple polygons to the batch (unfilled and filled)
    Point pentagon[] = {{60, 10}, {110, 50}, {85, 110}, {35, 110}, {10, 50}};
    drawpoly(pentagon, 5, false, 5);

    Point star[] = {{46, 401}, {34, 425}, {10, 431}, {31, 443}, {19, 470}, {46, 455}, {73, 470}, {61, 443}, {82, 431}, {58, 425}};
    drawpoly(star, 10, true, 14);

    Point triangle[] = {{630, 470}, {630, 275}, {540, 400}};
    drawpoly(triangle, 3, true, 11);

    Point polygon[] = {{530, 10}, {630, 10}, {530, 110}, {630, 210}, {530, 210}, {630, 110}};
    drawpoly(polygon, 6, true, 6);

    // Dispatch batched calls
    endgraphics();

    // Return to video mode 3 (text console) after key is pressed
    getch();
    setvideomode(0x03);

    exit();
}
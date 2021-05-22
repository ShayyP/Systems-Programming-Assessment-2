#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "batch.h"

int sys_getch(void) {
    return consoleget();
}

// TODO: Implement your system call for switching video modes (and any other video-related syscalls)
// in here.

int sys_setvideomode(void) {
    int mode; 

    if (argint(0, &mode) < 0) {
        return -1;
    }

    // Take snapshot of memory before we switch
    if (mode != 0x03){
        backuptext();
    }
    
    // Set video mode in console.c
    consolevgamode(mode);

    if (mode == 0x03){
        // Restore memory to snapshot if switching back to text mode
        restoretext();
    }
    else{
        // Otherwise clear the screen
        clearscreen();
    }

    return 0;
}

// System call to access setpixel
int sys_setpixel(void) {
    int x, y, colour;

    if (argint(0, &x) < 0 || argint(1, &y) < 0 || argint(2, &colour) < 0) {
        return -1;
    }

    // Calls setpixel in console.c, passing the parameters from the system call
    setpixel(x, y, colour);

    return 0;
}

// Basic utility function for finding the absolute value of a number
float abs(float num){
    return num < 0 ? -num : num;
}

// Basic utility function that returns -1 if input below 0, 0 if it is 0 and 1 if above 0
int signum(float x){
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

// Draws a line from x0, y0 to x1, y1 using bresenham algorithm
void drawline(int x0, int y0, int x1, int y1, int colour){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (true){
        setpixel(x0, y0, colour);
        if (x0 == x1 && y0 == y1){ 
            break; 
        }
        e2 = 2 * err;
        if (e2 > -dy){
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx){
            err += dx;
            y0 += sy;
        }
    }
}

// System call to access drawline
int sys_drawline(void){
    int x0, y0, x1, y1, colour;

    if (argint(0, &x0) < 0 ||  argint(1, &y0) < 0 ||  argint(2, &x1) < 0 ||  argint(3, &y1) < 0 ||  argint(4, &colour) < 0){
        return -1;
    }

    drawline(x0, y0, x1, y1, colour);

    return 0;
}

// Draws one segment of the circle
void drawcirclesegment(int xc, int yc, int x, int y, int colour){
    setpixel(xc+x, yc+y, colour); 
    setpixel(xc-x, yc+y, colour); 
    setpixel(xc+x, yc-y, colour); 
    setpixel(xc-x, yc-y, colour); 
    setpixel(xc+y, yc+x, colour); 
    setpixel(xc-y, yc+x, colour); 
    setpixel(xc+y, yc-x, colour); 
    setpixel(xc-y, yc-x, colour);
}

// Draws a circle at xc, yc with specified radius using bresenham algorithm
void drawcircle(int xc, int yc, int radius, int colour){
    int x = 0, y = radius, d = 3 - 2 * radius; 

    drawcirclesegment(xc, yc, x, y, colour); 

    while (y >= x){    
        x++;
        if (d > 0){ 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else{
            d = d + 4 * x + 6;
        }   
        drawcirclesegment(xc, yc, x, y, colour); 
    } 
}

// Fills polygon using the scanline algorithm
void fillpolyscanline(Point* points, int pointcount, int colour){
    Point startpoint = points[0];
    Point nextpoint;
    int i, j, k, dy, dx, y, temp, ymin, ymax;
    int xi[pointcount];
    float slope[pointcount];

    // Calculate slope values for each edge of the polygon
    for (i = 0; i < pointcount; i++){
        nextpoint = points[i + 1];
        // If we have reached the last point, loop back to the starting point
        if (i == pointcount - 1){
            nextpoint = startpoint;
        } 
        dy = nextpoint.y - points[i].y;
        dx = nextpoint.x - points[i].x;
        if (dy == 0) slope[i] = 1;
        if (dx == 0) slope[i] = 0;
        // If not flat, calculate slope using difference in x and y
        if (dy != 0 && dx != 0){
            slope[i] = (float) dx / dy;
        }
    }

    // Get min and max y values
    ymin = 480; // Initially set to largest value possible
    ymax = 0;   // Initially set to lowest value possible
    for (i = 0; i < pointcount; i++){
        if (points[i].y < ymin) ymin = points[i].y;
        if (points[i].y > ymax) ymax = points[i].y;
    }

    // Loop through each row of the polygon
    for (y = ymin; y < ymax; y++){
        k = 0;
        // Loop through each edge
        for (i = 0; i < pointcount; i++){
            nextpoint = points[i + 1];
            // If we have reached the last point, loop back to the starting point
            if (i == pointcount - 1){
                nextpoint = startpoint;
            } 
            // If edge intersects current y pos, save the x position of the intersection position to be drawn
            if ((points[i].y <= y && nextpoint.y > y) || (points[i].y > y && nextpoint.y <= y)){
                // Calculate xpos using slope and ypos
                xi[k] = (int)(points[i].x + slope[i] * (y - points[i].y));
                // Record amount of intersections
                k++;
            }
        }

        // Order intersection points by ascending x values
        for (j = 0; j < k-1; j++){
            for (i = 0; i < k-1; i++){
                if (xi[i] > xi[i+1]){
                    temp = xi[i];
                    xi[i] = xi[i+1];
                    xi[i+1] = temp;
                }
            }
        }

        // Draw lines between intersection points
        for (i = 0; i < k; i+=2){
            drawline(xi[i], y, xi[i+1]+1, y, colour);
        }
    }

}

// Draws a polygon with specified points
void drawpoly(Point* points, int pointcount, bool fill, int colour){
    // Draw outline (unfilled polygon)
    Point startpoint = *points;
    for (int i = 0; i < pointcount - 1; i++){
        drawline(points->x, points->y, (points+1)->x, (points+1)->y, colour);
        points++;
    }
    drawline(points->x, points->y, startpoint.x, startpoint.y, colour);
    points -= (pointcount - 1);

    // Fill polygon if specified
    if (fill){
        fillpolyscanline(points, pointcount, colour);
    }
}

// Executes calls passed in a batch
int sys_sendbatch(void){
    BatchCall* buffer;
    int count;

    // Takes pointer to the buffer and amount of calls as parameters
    if (argint(1, &count) < 0 || argptr(0, (char**)&buffer, count * sizeof(BatchCall)) < 0) {
        return -1;
    }

    for (int i = 0; i < count; i++){
        /* Calls can be for different functions (pixel, line, etc.).
        We use a union in our BatchCall struct so that memory overlaps since each call will only be one type*/
        switch (buffer->type){
            case pixel:
                // Call setpixel passing the values from the struct
                setpixel(buffer->pixel.x, buffer->pixel.y, buffer->colour);
                break;
            case line:
                // Call drawline passing the values from the struct
                drawline(buffer->line.x0, buffer->line.y0, buffer->line.x1, buffer->line.y1, buffer->colour);
                break;
            case circle:
                // Call drawcircle passing the values from the struct
                drawcircle(buffer->circle.xc, buffer->circle.yc, buffer->circle.radius, buffer->colour);
                break;
            case rect:
                // Call fillrect passing the values from the struct
                fillrect(buffer->rect.x, buffer->rect.y, buffer->rect.width, buffer->rect.height, buffer->colour);
                break;
            case polygon:
                // Call drawpoly passing the values from the struct
                drawpoly(buffer->polygon.points, buffer->polygon.pointcount, buffer->polygon.fill, buffer->colour);
                break;
        }
        buffer++;
    }

    return 0;
}
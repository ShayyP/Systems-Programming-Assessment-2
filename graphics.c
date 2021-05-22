#include "types.h"
#include "user.h"

BatchCall* batchcalls; // Pointer to array of calls
int callsthisbatch; // Tracking how big our batch is
int callsperbatch = 32; // Max size of one batch, this can be adjusted (higher = quicker but uses more memory)
bool batching = false; // Tracks if we are using batching or not

// Allocates memory for array of calls to kernel
void begingraphics(){
    batchcalls = malloc(callsperbatch * sizeof(BatchCall));
    callsthisbatch = 0;
    batching = true;
}

// Sends graphics calls to the kernel and frees up the memory used
void endgraphics(){
    batchcalls -= callsthisbatch;
    sendbatch(batchcalls, callsthisbatch);
    free(batchcalls);
    batching = false;
}

// Sends graphics calls to the kernel and reuses memory for next batch
void restartbatch(){
    batchcalls -= callsthisbatch;
    sendbatch(batchcalls, callsthisbatch);
    callsthisbatch = 0;
}

// Adds call to batch, checks if batch has reached capacity
void addtobatch(BatchCall call){
    *batchcalls = call;
    batchcalls++;
    callsthisbatch++;
    // If batch is at capacity, restart it
    if (callsthisbatch >= callsperbatch){
        restartbatch();
    }
}
// If batching, add setpixel request to current batch, otherwise use system call to access setpixel function in the kernel
void setpixel(int x, int y, int colour){
    if (!batching){
        kern_setpixel(x, y, colour);
        return;
    }
    BatchCall call;
    call.type = pixel;
    call.pixel.x = x;
    call.pixel.y = y;
    call.colour = colour;
    addtobatch(call);
}

// If batching, add drawline request to current batch, otherwise use system call to access drawline function in the kernel
void drawline(int x0, int y0, int x1, int y1, int colour){
    if (!batching){
        kern_drawline(x0, y0, x1, y1, colour);
        return;
    }
    BatchCall call;
    call.type = line;
    call.line.x0 = x0;
    call.line.y0 = y0;
    call.line.x1 = x1;
    call.line.y1 = y1;
    call.colour = colour;
    addtobatch(call);
}

// Add draw circle request to current batch
void drawcircle(int xc, int yc, int radius, int colour){
    BatchCall call;
    call.type = circle;
    call.circle.xc = xc;
    call.circle.yc = yc;
    call.circle.radius = radius;
    call.colour = colour;
    addtobatch(call);
}

// Add draw rect request to current batch
void fillrect(int x, int y, int width, int height, int colour){
    BatchCall call;
    call.type = rect;
    call.rect.x = x;
    call.rect.y = y;
    call.rect.width = width;
    call.rect.height = height;
    call.colour = colour;
    addtobatch(call);
}

// Add draw poly request to current batch
void drawpoly(Point* points, int pointcount, bool fill, int colour){
    BatchCall call;
    call.type = polygon;
    call.polygon.points = points;
    call.polygon.pointcount = pointcount;
    call.polygon.fill = fill;
    call.colour = colour;
    addtobatch(call);
}
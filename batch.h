// Enumerator and structs used to batch graphics calls
typedef enum {
    pixel,
    line,
    circle,
    rect,
    polygon,
} CallType;

typedef struct{
    int x, y;
} Point;

// Used union to allow overlapping memory rather than using extra memory for unused variables
typedef struct {
    CallType type;
    int colour;

    union {
        struct {
            int x, y;
        } pixel;

        struct {
            int x0, y0, x1, y1;
        } line;

        struct {
            int xc, yc, radius;
        } circle;

        struct {
            int x, y, width, height;
        } rect;

        struct {
            Point* points;
            int pointcount;
            bool fill;
        } polygon;
    };
} BatchCall;
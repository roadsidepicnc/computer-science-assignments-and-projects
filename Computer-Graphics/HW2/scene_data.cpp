float wallVertices[] =
{
    20.0f, 10.0f, 20.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    20.0f, 0.0f, 20.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -20.0f, 0.0f, 20.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -20.0f, 10.0f, 20.0f,0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    20.0f, 10.0f, -20.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    20.0f, 0.0f, -20.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -20.0f, 0.0f, -20.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -20.0f, 10.0f, -20.0f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    -20.0f, 10.0f, 20.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -20.0f, 0.0f, 20.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -20.0f, 0.0f, -20.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -20.0f, 10.0f, -20.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    20.0f, 10.0f, 20.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    20.0f, 0.0f, 20.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    20.0f, 0.0f, -20.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    20.0f, 10.0f, -20.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

float ceilingVertices[] =
{
    20.0f, 10.0f, -20.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    20.0f, 10.0f, 20.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -20.0f, 10.0f, 20.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -20.0f, 10.0f, -20.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
};

float floorVertices[] =
{
    20.0f, 0.0f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    20.0f, 0.0f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -20.0f, 0.0f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -20.0f, 0.0f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

float cubeVertices[] =
{
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
};

float pyramidVertices[] =
{
    1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.5f, 1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.5f, 1.0f,

    1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,

    1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.5f, 1.0f
};

float halfCubeVertices[] =
{
    1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f
};

float shapeFVertices[] =
{
    0.5f, 5.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 5.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    0.5f, 5.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 5.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    0.5f, 5.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 5.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    -0.5f, 5.0f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 5.0f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    0.5f, 5.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 5.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 5.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 5.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 5.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 5.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 5.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 5.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 4.0f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 4.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 4.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 4.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 5.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    2.5f, 4.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 4.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 5.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    2.5f, 5.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    2.5f, 4.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 4.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, 5.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    2.5f, 3.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 3.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 3.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 3.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 2.0f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 2.0f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 2.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 2.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 3.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    2.5f, 2.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 2.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 3.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    2.5f, 3.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    2.5f, 2.0f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 2.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, 3.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

    2.5f, 5.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 4.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    2.5f, 4.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    2.5f, 5.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    2.5f, 3.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    2.5f, 2.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    2.5f, 2.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    2.5f, 3.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int wallIndices[] =
{
    0, 1, 3,
    1, 2, 3,

    4, 5, 7,
    5, 6, 7,

    8, 9, 11,
    9, 10, 11,

    12, 13, 15,
    13, 14, 15
};

unsigned int ceilingIndices[] =
{
    0, 1, 3,
    1, 2, 3,
};

unsigned int floorIndices[] =
{
    0, 1, 3,
    1, 2, 3
};

unsigned int cubeIndices[] =
{
    0, 1, 3,
    1, 2, 3,

    4, 5, 7,
    5, 6, 7,

    8, 9, 11,
    9, 10, 11,

    12, 13, 15,
    13, 14, 15,

    16, 17, 19,
    17, 18, 19,

    20, 21, 23,
    21, 22, 23
};

unsigned int pyramidIndices[] =
{
    0, 1, 3,
    1, 2, 3,

    4, 5, 6,
    7, 8, 9,
    10, 11, 12,
    13, 14, 15
};

unsigned int halfCubeIndices[] =
{
    0, 1, 3,
    1, 2, 3,

    4, 5, 7,
    5, 6, 7,

    8, 9, 11,
    9, 10, 11,

    12, 13, 14,
    15, 16, 17
};

unsigned int shapeFIndexes[] =
{
    0, 1, 3,
    1, 2, 3,

    4, 5, 7,
    5, 6, 7,

    8, 9, 11,
    9, 10, 11,

    12, 13, 15,
    13, 14, 15,

    16, 17, 19,
    17, 18, 19,

    20, 21, 23,
    21, 22, 23,

    24, 25, 27,
    25, 26, 27,

    28, 29, 31,
    29, 30, 31,

    32, 33, 35,
    33, 34, 35,

    36, 37, 39,
    37, 38, 39,

    40, 41, 43,
    41, 42, 43,

    44, 45, 47,
    45, 46, 47,

    48, 49, 51,
    49, 50, 51,

    52, 53, 55,
    53, 54, 55,

    56, 57, 59,
    57, 58, 59,

    60, 61, 63,
    61, 62, 63,
};

unsigned int wallsVBO;
unsigned int wallsVAO;
unsigned int wallsEBO;

unsigned int ceilingVBO;
unsigned int ceilingVAO;
unsigned int ceilingEBO;

unsigned int floorVBO;
unsigned int floorVAO;
unsigned int floorEBO;

unsigned int cubeVBO;
unsigned int cubeVAO;
unsigned int cubeEBO;

unsigned int pyramidVBO;
unsigned int pyramidVAO;
unsigned int pyramidEBO;

unsigned int halfCubeVBO;
unsigned int halfCubeVAO;
unsigned int halfCubeEBO;

unsigned int shapeFVBO;
unsigned int shapeFVAO;
unsigned int shapeFEBO;

void BindVertexData()
{
    glGenVertexArrays(1, &wallsVAO);
    glGenBuffers(1, &wallsVBO);
    glGenBuffers(1, &wallsEBO);

    glBindVertexArray(wallsVAO);

    glBindBuffer(GL_ARRAY_BUFFER, wallsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallsEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &ceilingVAO);
    glGenBuffers(1, &ceilingVBO);
    glGenBuffers(1, &ceilingEBO);

    glBindVertexArray(ceilingVAO);

    glBindBuffer(GL_ARRAY_BUFFER, ceilingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ceilingEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ceilingIndices), ceilingIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);

    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);
    glGenBuffers(1, &pyramidEBO);

    glBindVertexArray(pyramidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &halfCubeVAO);
    glGenBuffers(1, &halfCubeVBO);
    glGenBuffers(1, &halfCubeEBO);

    glBindVertexArray(halfCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, halfCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(halfCubeVertices), halfCubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, halfCubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(halfCubeIndices), halfCubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &shapeFVAO);
    glGenBuffers(1, &shapeFVBO);
    glGenBuffers(1, &shapeFEBO);

    glBindVertexArray(shapeFVAO);

    glBindBuffer(GL_ARRAY_BUFFER, shapeFVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shapeFVertices), shapeFVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeFEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shapeFIndexes), shapeFIndexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void ReleaseVertexData()
{
    glDeleteVertexArrays(1, &wallsVAO);
    glDeleteBuffers(1, &wallsVBO);
    glDeleteBuffers(1, &wallsEBO);

    glDeleteVertexArrays(1, &ceilingVAO);
    glDeleteBuffers(1, &ceilingVBO);
    glDeleteBuffers(1, &ceilingEBO);
    
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    
    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteBuffers(1, &pyramidVBO);
    glDeleteBuffers(1, &pyramidEBO);
    
    glDeleteVertexArrays(1, &halfCubeVAO);
    glDeleteBuffers(1, &halfCubeVBO);
    glDeleteBuffers(1, &halfCubeEBO);
    
    glDeleteVertexArrays(1, &shapeFVAO);
    glDeleteBuffers(1, &shapeFVBO);
    glDeleteBuffers(1, &shapeFEBO);
}
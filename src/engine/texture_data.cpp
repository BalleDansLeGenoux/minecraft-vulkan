#include "engine/texture_data.h"

FaceTextureData getCoord(int x, int y) {
    return {
        {1.0f/16*(0+x), 1.0f/16*(0+y)},
        {1.0f/16*(1+x), 1.0f/16*(0+y)},
        {1.0f/16*(1+x), 1.0f/16*(1+y)},
        {1.0f/16*(0+x), 1.0f/16*(1+y)}
    };
}

std::vector<CubeTextureData> TEXTURE_COORD = {
    {
        getCoord(0, 0),
        getCoord(3, 0),
        getCoord(2, 0)
    }, 
    {
        getCoord(2, 0),
        getCoord(2, 0),
        getCoord(2, 0)
    },
    {
        getCoord(1, 0),
        getCoord(1, 0),
        getCoord(1, 0)
    },
    {
        getCoord(5, 2),
        getCoord(5, 2),
        getCoord(5, 2)
    },
    {
        getCoord(6, 1),
        getCoord(6, 2),
        getCoord(6, 3)
    },
    {
        getCoord(1, 3),
        getCoord(1, 3),
        getCoord(1, 3)
    },
    {
        getCoord(4, 3),
        getCoord(4, 3),
        getCoord(4, 3)
    }
};
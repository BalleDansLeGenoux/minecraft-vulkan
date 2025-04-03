#include "engine/face_position.h"

FacePosition PLUS_X = FacePosition({1,1,1}, {1,1,0}, {1,0,0}, {1,0,1});
FacePosition MINUS_X = FacePosition({0,1,0}, {0,1,1}, {0,0,1}, {0,0,0});
FacePosition PLUS_Y = FacePosition({0,1,0}, {1,1,0}, {1,1,1}, {0,1,1});
FacePosition MINUS_Y = FacePosition({1,0,0}, {0,0,0}, {0,0,1}, {1,0,1});
FacePosition PLUS_Z = FacePosition({0,1,1}, {1,1,1}, {1,0,1}, {0,0,1});
FacePosition MINUS_Z = FacePosition({1,1,0}, {0,1,0}, {0,0,0}, {1,0,0});
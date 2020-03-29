#pragma once

#include <SDL.h> //General SDL header
#include <SDL_image.h> //Images management SDL header
#include <SDL_ttf.h> //Text-writting SDL header

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <math.h>

#ifdef _DEBUG
#include <vld.h>
#endif

#include <stdlib.h>
#include <crtdbg.h>

#define FRAMERATE 30
#define PX_SIZE 2  //Real size of a pixel, in centimeters
#define ZOOM_MIN 1
#define ZOOM_MAX 3
#define ZOOM_STEP 0.5f

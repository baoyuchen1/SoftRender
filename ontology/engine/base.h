#pragma once
#include"../3rd/math/m3dmath.h"
#include"../3rd/tinyxml/tinyxml.h"
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#define PRIMARY_BUFFER 0
#define BACK_BUFFER 1
#define GET_BOUNDARY(x,min,max)  (x < min)? min : ((x > max)? max : x);
#define INTERP(x1,x2,t) x1 + (x2 - x1) * t;
#define MAX(X,Y)  X>Y?X:Y
#define MIN(X,Y)  X<Y?X:Y
#define SET_BIT(word,bit) ((word) = ((word)|(bit)))
#define RESET_BIT(word,bit) ((word) = ((word)&(bit)))
#define TRANSFORM_LOCAL_ONLY 0 
#define TRANSFORM_TRANS_ONLY 1
#define TRANSFORM_LOCAL_TO_TRANS 2
#define OBJECT_NULL 0x0000
#define OBJECT_ACTIVE 0x0001
#define OBJECT_VISIBLE 0x0002
#define OBJECT_CULLED 0x0003
#define PLOYOBJECT_NULL 0x0000
#define PLOYOBJECT_ACTIVE 0x0001
#define PLOYOBJECT_CLIPPED 0x0002
#define PLOYOBJECT_BACKFACE 0x0003
#define PLOYOBJECT_LIT 0x0004
#define NOt_NUM_ERROR  -1
#define NUM_TOOLONG_ERROR -2
#define polyrendlist_iter std::vector<object::polyrendlist_ptr>::iterator 
#define polyobject_iter std::list<object::polyobject_ptr>::iterator 
#define vector4ptr_iter std::vector<vector4*>::iterator 
#define TO_INT_TYPE 1
#define TO_DOUBLE_TYPE 2
//typedef unsigned int COLOR;
typedef unsigned int VIDEO_FRAME;
typedef unsigned int POS;
typedef unsigned int IUINT32;
typedef struct { float r, g, b, a; } color_b;

#define OBJECT_MAX_NUM 8192

int char_to_int(char*,int&);
int char_to_float(char*,float&);


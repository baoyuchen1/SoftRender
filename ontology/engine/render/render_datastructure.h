#pragma once
#include"base.h"
#include<list>
#define POLY_ACTIVE 1
#define POLY_CLIPPED 2
#define POLY_BACKFACE 4
typedef struct
{
	union
	{
		struct
		{
			byte r, g, b, a;
		};
		struct
		{
			int color;
		};
	};
} COLOR;
namespace object {
	//
	typedef struct poly_rendlist {
		poly_rendlist(){}
		poly_rendlist(const vector4 &a, const vector4 &b, const vector4 &c) {
			tvlist[0] = a; tvlist[1] = b; tvlist[2] = c;
		};
		int state;//
		int attr;//
		COLOR color;
		int lit_color[3];
		vector4 vlist[3];//
		vector4 tvlist[3];// 
		poly_rendlist* prev;
		poly_rendlist* next;
	//	BITMAP_IMAGE_PTR texture;
	}polyrendlist, *polyrendlist_ptr;
	typedef struct poly_object {
		int state;//
		int attr;//
		COLOR color;//
		int lit_color[3];
		int vert[3];
	}*polyobject_ptr;
	typedef struct object_type {
		wchar_t* name;//
		int id;//
		int state;//
		int attr;//
		double avg_radius;//
		double max_radius;//
		int num_polys;//
		//BITMAP_IMAGE_PTR texture;
		vector4 ux, uy, uz;//
		std::vector<vector4*> vlist_local;
		std::vector<vector4*> vlist_trans;
		//vector4* vlist_local;//
		//vector4* vlist_trans;//
		std::list<polyobject_ptr> plist;
		vector4 word_pos;//
		vector4 dir;//
	}object, *object_ptr;
	typedef struct renderList_type {
		int state;
		int attr;
		int poly_nums;
		//
		std::vector<polyrendlist_ptr> poly_ptrs;
	};

}
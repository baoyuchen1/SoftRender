#pragma once
#include"base.h"
#include"render_datastructure.h"
class device{
public:
	void draw_line(int ,int ,int ,int ,COLOR );
	bool draw_pixel(int ,int ,COLOR);
	void draw_init(int width,int height,void* vb);
	void draw_clear(int mode);
	void draw_object(object::object_ptr obj);
	void draw_rendlist(object::renderList_type* rend_list); 
	void transform_rendlist(object::renderList_type* rend_list,vector4* mt,int coord_select);
    private:
	int width;                  // width of screen
	int height;                 // height of screen
	VIDEO_FRAME **video_buffer;    
	float **zbuffer;            // zbuffer[y] is the pointer of y col
	IUINT32 **texture;          // texture same as up
	int tex_width;              // width of texture
	int tex_height;             // height of texture
	float max_u;                // max size of tex_width：tex_width - 1
	float max_v;                // max size of tex_height：tex_height - 1
	int render_state;           // 
	COLOR background;         // color of backround
	COLOR foreground;         // color of foreground
    
private:

};
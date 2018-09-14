#pragma once
#include"../base.h"
#include"render_datastructure.h"



class camera
{
public:
	camera();
	~camera();
	void transform_object(object::renderList_type* rend_list, matrix44* mt, int coord_select);
	void transform_object(object::object_ptr, matrix44 mt, int coord_select, int transform_basis);
	void model_to_world(object::object_ptr obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);
	void model_to_world(object::renderList_type* rend_list,const vector4  &world_pos, int coord_select = TRANSFORM_LOCAL_TO_TRANS);
	void camera_init(int cam_attr, const vector4 &cam_pos,const vector4 &cam_dir,const vector4 &cam_traget,
	float cam_near_clip,float cam_far_clip,float cam_fov,float cam_screen_w,float cam_screen_h );
	void remove_backfaces(object::object_ptr obj);
	void camera_to_perspcetive(object::object_ptr);
	void build_camera_to_perspcetive_matrix44(matrix44& m);
	void camera_to_perspective(object::renderList_type* rend_list);
	void perspecive_to_screen(object::object_ptr obj);
	void perspecive_to_screen(object::renderList_type* rend_list);
	void build_screen_to_perspective_matrix44(matrix44& m);

private:
	int state;
	int attr;
	vector4 wpos;//the pos in the world
	vector4 dir;//the pos we are looking at
	vector4 u, v, n; //uvn vector
	vector4 traget;  //traget pos
	float view_dist; 
	float fov;
	float near_clip_z;
	float far_clip_z;
	plane r_clip_plane, l_clip_plane, t_clip_plane, b_clip_plane;
	float viewplane_width, viewplane_height;
	float screen_width, screen_height;
	float screen_center_x, screen_center_y;
	float aspect_ratio;
	void build_matrix(int mode);
	matrix44 mcam; //worldpos to camerapos
	matrix44 mper; //camerpos to perspectivepos
	matrix44 mscr; //perspectivepos to screenpos
};


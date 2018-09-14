#include"camera.h"
camera::camera()
{
}

camera::~camera()
{
} 
void camera::transform_object(object::renderList_type* rend_list, matrix44* mt, int coord_select) {
	
	if (!rend_list->poly_ptrs.empty()) 
	{
		for (polyrendlist_iter it = rend_list->poly_ptrs.begin(); it != rend_list->poly_ptrs.end(); it++)
		{
			if (!((*it)->state&POLY_ACTIVE) ||
				((*it)->state&POLY_CLIPPED) ||
				((*it)->state&POLY_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++)
			{
				switch (coord_select)
				{
				case TRANSFORM_LOCAL_ONLY:
				{
					(*it)->vlist[vertex]*=(*mt);
				}break;
				case TRANSFORM_TRANS_ONLY:
				{
					(*it)->tvlist[vertex] *= (*mt);
				}break;
				case TRANSFORM_LOCAL_TO_TRANS: 
				{
					(*it)->tvlist[vertex] = (*it)->vlist[vertex] * (*mt);
				}break;
				default:
					break;
				}

			}
		}
	}
}

void camera::transform_object(object::object_ptr obj, matrix44 mt, int coord_select, int transform_basis)
{
	vector4ptr_iter local = obj->vlist_local.begin();
	vector4ptr_iter trans = obj->vlist_trans.begin();
	for (int vertex = 0; vertex < obj->vlist_local.size(); vertex++,local++,trans++)
	{
		switch (coord_select)
		{
		case TRANSFORM_LOCAL_ONLY: 
		{
			(**local) *= (*mt);  
		}break;
		case TRANSFORM_TRANS_ONLY:
		{
			(**trans) *= (*mt);
		}break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			(**trans) = (**local)*(*mt);
		}break;
		default:
			break;
		}
	}
	if (transform_basis)
	{
		obj->ux *= (*mt);
		obj->uy *= (*mt);
		obj->uy *= (*mt);
	}
}

void camera::model_to_world(object::object_ptr obj, int coord_select)
{
	vector4ptr_iter local = obj->vlist_local.begin();
	vector4ptr_iter trans = obj->vlist_trans.begin();
	for (int vertex = 0; vertex < obj->vlist_local.size(); vertex++, local++, trans++)
	{
		if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
		{
			(**trans) = (**local) + obj->word_pos;
		}
		else //transform_trans_only
		{
			(**trans) += obj->word_pos;
		}
	}
}

void camera::model_to_world(object::renderList_type * rend_list, const vector4 &world_pos, int coord_select)
{
	if (!rend_list->poly_ptrs.empty()) 
	{
		for (polyrendlist_iter it = rend_list->poly_ptrs.begin(); it != rend_list->poly_ptrs.end(); it++)
		{
			if (!((*it)->state&POLY_ACTIVE) ||
				((*it)->state&POLY_CLIPPED) ||
				((*it)->state&POLY_BACKFACE))
				continue;
			for (int vertex = 0; vertex < 3; vertex++)
			{
				if (coord_select == TRANSFORM_LOCAL_TO_TRANS)
				{
					(*it)->tvlist[vertex] = (*it)->vlist[vertex] + world_pos;
				}
				else//transform_trans_only
				{
					(*it)->tvlist[vertex] += world_pos;
				}
			}
		}
	}
}

void camera::camera_init(int cam_attr, const vector4 & cam_pos, const vector4 & cam_dir, const vector4 & cam_traget,
	float cam_near_clip, float cam_far_clip, float cam_fov, float cam_screen_w, float cam_screen_h)
{
	attr = cam_attr;
	wpos = cam_pos;
	dir = cam_dir;
	if (cam_traget != NULL)
		traget = cam_traget;
	else
		traget = 0;
	near_clip_z = cam_near_clip;
	far_clip_z = cam_far_clip;
	fov = cam_fov;
	screen_width = cam_screen_w;
	screen_height = cam_screen_h;
	screen_center_x = (screen_width - 1) / 2;
	screen_center_y = (screen_height - 1) / 2;
	aspect_ratio = screen_width / screen_height;
	fov = cam_fov;
	viewplane_width = 2.0;
	viewplane_height = 2.0 / aspect_ratio;
	view_dist = (viewplane_width)*tanf(fov)*(0.5);
	matMatrix44Identity(mcam);
	matMatrix44Identity(mper);
	matMatrix44Identity(mscr);
	vector3 vn;
	vector3 po(0, 0, 0);
	vn = vector3(view_dist, 0, (-screen_width / 2));
	r_clip_plane = plane(vn, po);
	vn = vector3(-view_dist, 0, (-screen_width / 2));
	l_clip_plane = plane(vn, po);
	vn = vector3(0, view_dist, (-screen_height / 2));
	t_clip_plane = plane(vn, po);
	vn = vector3(0, view_dist, (-screen_height / 2));
	b_clip_plane = plane(vn, po);
}

void camera::remove_backfaces(object::object_ptr obj)
{
	if (obj->state&OBJECT_CULLED)
		return;
	for (polyobject_iter it = obj->plist.begin(); it !=obj->plist.end(); it++)
	{
		if (!((*it)->state&POLY_ACTIVE) ||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		vector4 u, v, n;
		u = *(obj->vlist_trans[(*it)->vert[0]]) - *(obj->vlist_trans[(*it)->vert[1]]);
		v = *(obj->vlist_trans[(*it)->vert[0]]) - *(obj->vlist_trans[(*it)->vert[2]]);
		n = u*v;
		vector4 view;
		view = *(obj->vlist_trans[(*it)->vert[0]]) - wpos;
		float d = fVector4Dot(view, n);
		if (d <= 0.0f)
			SET_BIT((*it)->state, PLOYOBJECT_BACKFACE);
	}
}

void camera::camera_to_perspcetive(object::object_ptr obj)
{
	for (vector4ptr_iter it = obj->vlist_trans.begin(); it !=obj->vlist_trans.end(); it++)
	{
		float z = (*it)->z;
		(*it)->x = (view_dist*(*it)->x )/ z;
		(*it)->y = ((view_dist*(*it)->y)*aspect_ratio) / z;
	}
}

void camera::build_camera_to_perspcetive_matrix44(matrix44 & m)
{
	m = matrix44(view_dist, 0, 0, 0,
		0, view_dist*aspect_ratio, 0, 0,
		0, 0, 1, 1,
		0, 0, 0, 0);
}

void camera::camera_to_perspective(object::renderList_type * rend_list)
{
	for (polyrendlist_iter it = rend_list->poly_ptrs.begin(); it != rend_list->poly_ptrs.end(); it++)
	{
		if (!((*it)->state&POLY_ACTIVE) ||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		for (int vertex= 0; vertex < 3; vertex++)
		{
			float z = (*it)->tvlist[vertex].z;
			(*it)->tvlist[vertex].x = (view_dist*(*it)->tvlist[vertex].x) / z;
			(*it)->tvlist[vertex].y = ((view_dist*(*it)->tvlist[vertex].y)*aspect_ratio) / z;
		}
	}
}

void camera::perspecive_to_screen(object::object_ptr obj)
{
	float  alpha = (0.5*screen_width - 0.5);
	float beta = (0.5*screen_height - 0.5);
	for (vector4ptr_iter it = obj->vlist_trans.begin(); it != obj->vlist_trans.end(); it++)
	{
		(*it)->x = alpha + alpha*(*it)->x;
		(*it)->y = beta - beta*(*it)->y;
	}
}

void camera::perspecive_to_screen(object::renderList_type * rend_list)
{
	for (polyrendlist_iter it = rend_list->poly_ptrs.begin(); it != rend_list->poly_ptrs.end(); it++)
	{
		if (!((*it)->state&POLY_ACTIVE) ||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		float  alpha = (0.5*screen_width - 0.5);
		float beta = (0.5*screen_height - 0.5);
		for (int vertex = 0; vertex < 3; vertex++)
		{
			(*it)->tvlist[vertex].x = alpha + alpha*(*it)->tvlist[vertex].x;
			(*it)->tvlist[vertex].y = beta - beta*(*it)->tvlist[vertex].y;
		}
	}
}

void camera::build_screen_to_perspective_matrix44(matrix44 & m)
{
	float  alpha = (0.5*screen_width - 0.5);
	float beta = (0.5*screen_height - 0.5);
	m = matrix44(alpha, 0, 0, 0,
		0, -beta, 0, 0,
		alpha, beta, 1, 0,
		0, 0, 0, 1);
}
  
void camera::build_matrix(int mode)
{
	matrix44 mt_inv, mt_uvn, mtmp;
	mt_inv = matrix44( 1, 0, 0, 0,
		               0, 1, 0, 0,
		               0, 0, 1, 0,
		               -wpos.x, -wpos.y, -wpos.z, 1);
	//get  unv vector 
	n = traget - wpos;
	v = vector4(0, 0, 0, 1);
	u = n*v;
	v = n*u;
	u.normalize();
	v.normalize();
	n.normalize();
	mt_uvn = matrix44(
		u.x, v.x, n.x, 0,
		u.y, v.y, n.y, 0,
		u.z, v.z, n.z, 0,
		0,   0,   0,   1);
	mcam = mt_inv*mt_uvn;
}




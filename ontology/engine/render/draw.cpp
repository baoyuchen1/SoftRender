#include"draw.h"

bool device::draw_pixel(int x,int y,COLOR color){
    if (((POS)x<width)&&((POS)y<height)){
    	video_buffer[y][x] = color.color;
        return true;
    }
    else return false;
}
void device::draw_line(int x,int y,int tx,int ty,COLOR color){
	int dx = x - tx;
	int dy = y - ty;
	int base[4];
	int change_x, change_y;
	if (abs(dx) > abs(dy)) {
		base[0] = x;
		base[1] = tx;
		base[2] = y;
		base[3] = ty;
		change_x = 0;
		change_y = 2;
	}
	else
	{
		base[0] = y;
		base[1] = ty;
		base[2] = x;
		base[3] = tx;
		change_x = 2;
		change_y = 0;
	}
	int turn = 0, turn2 = 0;
	if (base[0]!=base[1])
	turn = (base[0] - base[1]) / abs(base[0] - base[1]);
	if (y!=ty)
	turn2 = (ty - y) / abs(ty - y);
	double detle;
	bool straightcheck = false;
	if(base[2]!=base[3])
		detle = ((double)base[2] - (double)base[3] )/ ((double)base[0] - (double)base[1]);
	else {
		detle = 0;
		straightcheck = true;
	}
	double t = (double)base[2];
	if (base[2] < base[3])
		detle = fabs(detle);
	else
		detle = -fabs(detle);
	while ((((base[0] != base[1])&&(base[2]!=base[3])))|| straightcheck) {
		draw_pixel(base[change_x], base[change_y], color);
		base[0] -= turn;
		t += detle;
		if (fabs(t - floor(t)) < 0.5)
			base[2] = floor(t);
		else
			base[2] = ceil(t);
		if ((base[1] == base[0])){
			straightcheck = false;
		}
	}
}

void device::draw_init(int w,int h,void* vb){
	int need = sizeof(void*) * (h * 2 + 1024) + w * h * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	video_buffer = (IUINT32**)ptr;
	zbuffer = (float**)(ptr + sizeof(void*) * h);
	ptr += sizeof(void*) * h * 2;
	texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + w * height * 4;
	ptr += w * h * 8;
	if (vb != NULL) framebuf = (char*)vb;
	for (j = 0; j < h; j++) {
		video_buffer[j] = (IUINT32*)(framebuf + w * 4 * j);
		zbuffer[j] = (float*)(zbuf + w * 4 * j);
	}
	texture[0] = (IUINT32*)ptr;
	texture[1] = (IUINT32*)(ptr + 16);
	memset(texture[0], 0, 64);
	tex_width = 2;
	tex_height = 2;
	max_u = 1.0f;
	max_v = 1.0f;
	width = w;
	height = h;
	background.color = 0xc0c0c0;
	foreground.color = 0;
	//transform_init(&transform, width, height);
//	render_state = RENDER_STATE_WIREFRAME;
	
}

void device::draw_clear(int mode){
	int y, x;
	for (y = 0; y < height; y++) {
		IUINT32 *dst = video_buffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = background.color;
		for (x = width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < height; y++) {
		float *dst = zbuffer[y];
		for (x = width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

void device::draw_object(object::object_ptr obj)
{
	for (polyobject_iter it = obj->plist.begin(); it != obj->plist.end(); it++)
	{
		if (!((*it)->state&POLY_ACTIVE) ||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		draw_line(obj->vlist_trans[(*it)->vert[0]]->x, obj->vlist_trans[(*it)->vert[0]]->y, obj->vlist_trans[(*it)->vert[1]]->x, obj->vlist_trans[(*it)->vert[1]]->y,
			(*it)->color);
		draw_line(obj->vlist_trans[(*it)->vert[1]]->x, obj->vlist_trans[(*it)->vert[1]]->y,
			obj->vlist_trans[(*it)->vert[2]]->x, obj->vlist_trans[(*it)->vert[2]]->y,
			(*it)->color);
		draw_line(obj->vlist_trans[(*it)->vert[2]]->x, obj->vlist_trans[(*it)->vert[2]]->y,
			obj->vlist_trans[(*it)->vert[0]]->x, obj->vlist_trans[(*it)->vert[0]]->y,
			(*it)->color);
	}
}

void device::draw_rendlist(object::renderList_type * rend_list)
{
	for (polyrendlist_iter it = rend_list->poly_ptrs.begin(); it !=rend_list->poly_ptrs.end(); it++)
	{
		int a = (*it)->state;

		if (!((*it)->state&POLY_ACTIVE)||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		draw_line((*it)->tvlist[0].x, (*it)->tvlist[0].y,
			(*it)->tvlist[1].x, (*it)->tvlist[1].y, 
			(*it)->color);
		draw_line((*it)->tvlist[1].x, (*it)->tvlist[1].y,
			(*it)->tvlist[2].x, (*it)->tvlist[2].y,
			(*it)->color);
		draw_line((*it)->tvlist[2].x, (*it)->tvlist[2].y,
			(*it)->tvlist[0].x, (*it)->tvlist[0].y,
			(*it)->color);
	}
}

void device::transform_rendlist(object::renderList_type * rend_list, vector4 * mt, int coord_select)
{
	polyrendlist_iter it = rend_list->poly_ptrs.begin();
	for (; it !=rend_list->poly_ptrs.end(); it++)
	{
		object::polyrendlist_ptr curr_poly = *it;
		if (!((*it)->state&POLY_ACTIVE) ||
			((*it)->state&POLY_CLIPPED) ||
			((*it)->state&POLY_BACKFACE))
			continue;
		for (int vertex = 0; vertex < 3; vertex++)
		{
			switch (coord_select)
			{
			case TRANSFORM_LOCAL_ONLY:
				curr_poly->vlist[vertex] *= *mt;
			case TRANSFORM_TRANS_ONLY:
				curr_poly->tvlist[vertex] *= *mt;
			case TRANSFORM_LOCAL_TO_TRANS:
				curr_poly->tvlist[vertex] = curr_poly->vlist[vertex]*(*mt);
			default:
				break;
			}
		}
	}
}





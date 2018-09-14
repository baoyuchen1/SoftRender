#include "initx3d.h"

void resource::x3d::init_renderlist(const char * flie_name,object::renderList_type& render_list)
{
	TiXmlDocument doc(flie_name);
	TiXmlAttribute* attr;
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
      printf("Could not load test file %s. Error='%s'. Exiting.\n",
		  flie_name, doc.ErrorDesc());
		    exit(1);
	}
	TiXmlElement* root = doc.RootElement();
	TiXmlElement* temp_ele = root;
	save_data_form_file(root);
	set_renderlist_data(render_list);
}


int resource::x3d::analyze_string_to_int( std::string input,const std::string& name)
{
	string_to_num(input, name, TO_INT_TYPE);
	return 0;
}

int resource::x3d::analyze_string_to_double(std::string input,const std::string& name)
{
	string_to_num(input, name, TO_DOUBLE_TYPE);
	return 0;
}

void resource::x3d::string_to_num(std::string& input, const std::string &name, int type) {
	std::string::iterator it = input.begin();
	int i = 0;
	int start = 0;
	int end = 0;
	std::string analzye_string;
	for (; it != input.end(); end++, it++)
	{
		start = end;
		if ((*it) == ' ') {
			
		}
		else {
			while (it != input.end()) {
				if (CHECK_STRING_IS_NUM(*it)) {
					it++; end++;
				}
				else {
					break;
				}
			}
			analzye_string = input.substr(start, end - start);
			if (!analzye_string.empty()) {
				switch (type)
				{
				case TO_INT_TYPE:
					int_container[name].push_back(atoi(analzye_string.c_str()));
					break;
				case TO_DOUBLE_TYPE:
					double_container[name].push_back(atof(analzye_string.c_str()));
					break;
				default:
					break;
				}
			}

		}
	}
}

bool resource::x3d::get_node_pointer_byName(TiXmlElement * pRootEle, std::string & strNodeName, TiXmlElement *& Node)
{
	// 假如等于根节点名，就退出
	if (strNodeName == pRootEle->ValueStr())
	{
		Node = pRootEle;
		return true;
	}

	TiXmlElement* pEle = pRootEle;
	for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		//递归处理子节点，获取节点指针
		if (get_node_pointer_byName(pEle, strNodeName, Node))
			return true;
	}

	return false;
}

bool resource::x3d::get_ele_attribute(const std::string & name, std::string & value, TiXmlAttribute* attr)
{
	for (; attr; attr = attr->Next())
	{
		if (attr->NameTStr()==name)
		{
			value = attr->Value();
			return true;
		}
	}
	return false;
}

void resource::x3d::save_data_form_file(TiXmlElement * root)
{
	TiXmlAttribute* attr;
	if (NULL != root) {
		TiXmlElement* temp_ele = root;
		std::string name("Coordinate");
		get_node_pointer_byName(root, name, temp_ele);
		attr = temp_ele->FirstAttribute();
		for (; attr; attr = attr->Next())
		{
			if (attr->NameTStr() == "point") {
				analyze_string_to_double(attr->Value(), "point");
			}
		}
		name = "IndexedFaceSet";
		get_node_pointer_byName(root, name, temp_ele);
		attr = temp_ele->FirstAttribute();
		for (; attr; attr = attr->Next())
		{
			if (attr->NameTStr() == "colorIndex") {
				analyze_string_to_int(attr->Value(), "colorIndex");
			}
			if (attr->NameTStr() == "coordIndex") {
				analyze_string_to_int(attr->Value(), "coordIndex");
			}
		}
		name = "Color";
		get_node_pointer_byName(root, name, temp_ele);
		attr = temp_ele->FirstAttribute();
		for (; attr; attr = attr->Next())
		{
			if (attr->NameTStr() == "color") {
				analyze_string_to_double(attr->Value(), "color");
			}
		}
	}
}

typedef std::is_function<void(void)> pf_void_void;
void resource::x3d::set_renderlist_data(object::renderList_type &render_list)
{
	render_list.state = PLOYOBJECT_ACTIVE;
	std::vector<int>::iterator it= int_container["coordIndex"].begin();
	std::vector<int>::iterator color_iter = int_container["colorIndex"].begin();
	int count = 1;
	render_list.poly_nums = 1;
	while (it != int_container["coordIndex"].end()) {
		object::polyrendlist_ptr p = new object::poly_rendlist();
		if ((int_container["coordIndex"].end() - it) >=4) {
			for (int i = 0; i < 3; i++,it++)
			{
				p->state = PLOYOBJECT_ACTIVE;
				p->tvlist[i].x = abs(double_container["point"][(*it)*3])*50;
				p->tvlist[i].y = double_container["point"][(*it)*3 + 1]*50;
				p->tvlist[i].z = double_container["point"][(*it)*3 + 2]*50;
				p->vlist[i].x = abs(double_container["point"][(*it)*3])*50;
				p->vlist[i].y = double_container["point"][(*it)*3 + 1]*50;
				p->vlist[i].z = double_container["point"][(*it)*3 + 2]*50;
			}
			p->color.r = double_container["color"][(*color_iter) * 3]+100;
			p->color.b = double_container["color"][(*color_iter) * 3+1]+100;
			p->color.g = double_container["color"][(*color_iter) * 3+2]+100;
			color_iter++;
			count++;
			it++; 		}
		render_list.poly_ptrs.push_back(p);
		render_list.poly_nums++;
	}
}



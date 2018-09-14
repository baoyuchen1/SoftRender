#include"../../3rd/math/m3d.h"
#include"../../3rd/tinyxml/tinyxml.h"
#include"../base.h"
#include"../render/render_datastructure.h"
#define CHECK_STRING_IS_NUM(it)   ((it>='0'&&it<='9')||it=='-'||it=='.')  
namespace resource {
	class x3d {
	public:
		void init_renderlist(const char* flie_name,object::renderList_type&);
		int analyze_string_to_int(std::string input, const std::string& name);
		int analyze_string_to_double(std::string input, const std::string& name);
	private:
		std::map < std::string, std::vector<int>> int_container;
		std::map < std::string, std::vector<double>> double_container;
		void string_to_num(std::string&,const std::string&,int type);
		bool get_node_pointer_byName(TiXmlElement* pRootEle, std::string &strNodeName, TiXmlElement* &Node);
		bool get_ele_attribute(const std::string& name,std::string& value,TiXmlAttribute* attr);
		void save_data_form_file(TiXmlElement* root);
		void set_renderlist_data(object::renderList_type&);
	};
}
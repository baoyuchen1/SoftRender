#include"base.h"
int char_to_int(char* num,int& s) {
	int savenum[10];
	char* temp = num;
	int count = 0;
	int count_move = 0;
	bool is_minus = false;
	while (true)
	{
		count_move++;
		if (((*temp)=='-')&&(is_minus==false))
		{
			is_minus = true;
			continue;
		}
		if ((*temp) == '.'){
			while (true) {
				count_move++;
				if (((*temp) < '0') || (*temp) > '9')
					break;
			}
			break;
		}
		if (((*temp) < '0') || (*temp) > '9')
			return NOt_NUM_ERROR;
		savenum[count] = (*temp) - '0';
		count++;
		temp++;
		if (count > 9) return NUM_TOOLONG_ERROR;
	}
	int result = 0;
	int up = 1;
	while(count > 0) {
		result += savenum[count] * up;
		up *= 10;
		count--;
	}
	s = result;
	//the data we return  is the position pointer going to
	return count_move;
	}
int char_to_float(char* num, float& s) {
	int save_intnum[8];
	int save_floatnum[8];
	char* temp = num;
	int count = 0;
	int count_move = 0;
	bool is_minus = false;
	bool is_float = false;
	float result = 0;
	int up = 1;
	while (true) {
		count_move++;
		if (((*temp) == '-') && (is_minus == false))
		{
			is_minus = true;
			continue;
		}
		if ((*temp) == '.') {
			is_float = true;
			temp++;
			break;
		}
		if (((*temp) < '0') || (*temp) > '9')
			return NOt_NUM_ERROR;
		save_intnum[count] = (*temp) - '0';
		count++;		
		temp++;
		if (count > 7) return NUM_TOOLONG_ERROR;
	}

	while (count > 0) {
		result += save_intnum[count] * up;
		up *= 10;
		count--;
	}
	s = result;
	result = 0;
	up = 1;
	if (is_float) {
		while (true)
		{
			if (((*temp) < '0') || (*temp) > '9')
				return NOt_NUM_ERROR;
			save_floatnum[count] = (*temp) - '0';
			count++;
			count_move++;
			temp++;
		}
		int div = 1;
		while (count > 0) {
			result += save_intnum[count] * up;
			up *= 10;
			count--;
		}
		s += result / up;
	}
	return count_move;
}


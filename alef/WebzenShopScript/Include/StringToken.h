
#pragma once

#include "include.h"

class CStringToken  
{
public:
	CStringToken();
	virtual ~CStringToken();
	CStringToken(const tstring& dataLine, const tstring& delim);
	
	size_t countTokens();							//��ū�� ���� 
	bool hasMoreTokens();							//��ū�� �����ϴ��� Ȯ�� 
	tstring nextToken();							//���� ��ū

	static 	tstring GetTstring(char * szData);
	static 	std::string GetAstring(TCHAR * szData);

private: 
	tstring data;
	tstring delimiter;								//������, ������ 
	std::vector<tstring> tokens;					//��ū�� ���Ϳ� ���� 
	std::vector<tstring>::iterator index;			//���Ϳ� ���� �ݺ��� 

	void split();									//��Ʈ���� �����ڷ� ������ ���Ϳ� ���� 
	void IsNullString(tstring::size_type pos);		//��ū�� ���� ������ ���Ϳ� �ΰ� �־��ֱ�
};

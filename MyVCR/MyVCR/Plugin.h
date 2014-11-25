#pragma once
class CPlugin
{
public:
	CPlugin(wchar_t* strPlugin);
	CPlugin();
	~CPlugin(void);
	CString& getFileName();
	bool isDisabled();
	void setDisabled(bool b);
	void setFileName(wchar_t* strPlugin);
private:
	CString _szFileName;
	bool _isDisabled;
};


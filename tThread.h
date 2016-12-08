#pragma once

class tThread :public CObject
{
public:
	tThread(void);
	~tThread(void);
	tThread(CEdit *StatusBar);
	CEdit *pStatusBar;
	void TestDisplay(int ind);
	void DisplayStatus(CString inputText);
};

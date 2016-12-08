#pragma once
#include "EachEquation.h"

class CEquationPool
{
public:
	CEquationPool(void);
	CEquationPool(CEdit *StatusBar,CEdit *Output,unsigned int nParam);

	virtual ~CEquationPool(void);
	std::vector<CEachEquation> vctEquation;
	unsigned int total_param;

	//display message functions
	int *strOutput_line;
	CEdit *pStatusBar;
	CEdit *pOutput;
	CString ms;
	void AddDisplayText(CString txt);
	void AddDisplayText(CString *txt);
	void DisplayStatus(CString inputText);
	void DisplayStatus();
	void DisplayInformation();


	void WriteFile(CString filename);
};

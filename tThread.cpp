#include "stdafx.h"
#include ".\tthread.h"
#include "Tree Analyser.h"

tThread::tThread(void)
{
}

tThread::~tThread(void)
{
}
tThread::tThread(CEdit *StatusBar)
{
	pStatusBar=StatusBar;
}

void tThread::DisplayStatus(CString inputText)
{
	pStatusBar->SetWindowText(inputText);
	pStatusBar->UpdateWindow();
}

void tThread::TestDisplay(int ind)
{
	for (int i=1; i<ind;i++)
	{
		CString ms;
		ms.Format("Current index=%i",i);
		DisplayStatus(ms);
	}
}
#include "stdafx.h"
#include ".\equationpool.h"

CEquationPool::CEquationPool(void)
{
}

CEquationPool::CEquationPool(CEdit *StatusBar,CEdit *Output,unsigned int nParam)
{
	CEquationPool();
	pStatusBar=StatusBar;
	pOutput=Output;	
	total_param=nParam;

}

CEquationPool::~CEquationPool(void)
{
}


void CEquationPool::DisplayStatus(CString inputText)
{
	pStatusBar->SetWindowText(inputText);
	pStatusBar->UpdateWindow();
}

void CEquationPool::DisplayStatus()
{
	pStatusBar->SetWindowText("Ready");
	pStatusBar->UpdateWindow();
}

void CEquationPool::AddDisplayText(CString *txt)
{
	*txt+="\r\n";

	pOutput->SetSel(pOutput->GetWindowTextLength(),pOutput->GetWindowTextLength(), FALSE);
	pOutput->ReplaceSel(*txt);
}

void CEquationPool::AddDisplayText(CString txt)
{
	txt+="\r\n";

	pOutput->SetSel(pOutput->GetWindowTextLength(),pOutput->GetWindowTextLength(), FALSE);
	pOutput->ReplaceSel(txt);
}

void CEquationPool::DisplayInformation()
{
	std::vector<unsigned int>vctUnknownParam;

	vctUnknownParam.resize(total_param);
	for(unsigned int i=0;i<total_param;i++)
	{//reset all
		vctUnknownParam.push_back(0);
	}

	for(unsigned int i=0;i<vctEquation.size();i++)
	{//for every equation
			vctUnknownParam[vctEquation[i].vctID.size()]++;
	}
	//unsigned int temp1=vctUnknownParam.size();
	for(unsigned int i=0;i<vctUnknownParam.size();i++)
	{//reset all
		if(vctUnknownParam[i]>0)
		{
			//unsigned int temp2=vctUnknownParam[i];
			if(vctUnknownParam[i]==1){ms.Format("Unkonwn %i parameter = 1 equation",i);}
			else {ms.Format("Unkonwn %i parameters = %i equations",i,vctUnknownParam[i]);}
			AddDisplayText(ms);
		}
	}

}


void CEquationPool::WriteFile(CString filename)
{
	FILE * pFile;
	pFile=fopen(filename,"wt");
	fprintf(pFile,"lnP0\tG\tVoxelID,PathLength\n");

	for(unsigned int i=0;i<this->vctEquation.size();i++)
	{
		fprintf(pFile,"%6.4f\t%6.4f",vctEquation[i].lnP0,vctEquation[i].G);
		for(unsigned int j=0;j<vctEquation[i].vctID.size();j++)
		{
			fprintf(pFile,"\t%i,%6.4f",vctEquation[i].vctID[j],vctEquation[i].vctPathLength[j]);
		}
		fprintf(pFile,"\n");
	}
	fclose(pFile);
}

// VirtualPlant.cpp: implementation of the CVirtualPlant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Tree Analyser.h"
#include "VirtualPlant.h"
#include ".\virtualplant.h"

#include <algorithm> //add for random_shuffle
#include <functional>  //add for random_shuffle

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//#include "Tree AnalyserDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//constructor
cVirtualPlant::cVirtualPlant()
{
	canopy_center.PutXyz(9e9,9e9,9e9);
	nx=0;ny=0;nz=0;dx=0;dy=0;dz=0;pack_index=0;
	xmin=-1e9;ymin=-1e9;zmin=-1e9;xmax=9e9;ymax=9e9;zmax=9e9;
	begin_nz=1;
	lifecell=0;
}
cVirtualPlant::cVirtualPlant(CEdit *StatusBar,CEdit *Output)
{
	cVirtualPlant();
	pStatusBar=StatusBar;
	pOutput=Output;	
}

void cVirtualPlant::Reset()
{
	canopy_center.PutXyz(9e9,9e9,9e9);
	nx=0;ny=0;nz=0;dx=0;dy=0;dz=0;pack_index=0;
	xmin=-1e9;ymin=-1e9;zmin=-1e9;xmax=9e9;ymax=9e9;zmax=9e9;
	begin_nz=1;
	lifecell=0;
	acell.clear();
}

cVirtualPlant::cVirtualPlant(int num_x,int num_y,int num_z)
{
	cVirtualPlant();
	nx=num_x;
	ny=num_y;
	nz=num_z;
	acell.resize(num_x*num_y*num_z);
}



//destructor
cVirtualPlant::~cVirtualPlant(){}



void cVirtualPlant::DisplayStatus(CString inputText)
{
	pStatusBar->SetWindowText(inputText);
	pStatusBar->UpdateWindow();
}

void cVirtualPlant::DisplayStatus()
{
	pStatusBar->SetWindowText("Ready");
	pStatusBar->UpdateWindow();
}

void cVirtualPlant::AddDisplayText(CString *txt)
{
	*txt+="\r\n";

	pOutput->SetSel(pOutput->GetWindowTextLength(),pOutput->GetWindowTextLength(), FALSE);
	pOutput->ReplaceSel(*txt);
}

void cVirtualPlant::AddDisplayText(CString txt)
{
	txt+="\r\n";

	pOutput->SetSel(pOutput->GetWindowTextLength(),pOutput->GetWindowTextLength(), FALSE);
	pOutput->ReplaceSel(txt);
}



bool cVirtualPlant::Resize(unsigned int num_x,unsigned int num_y,unsigned int num_z)
{
	nx=num_x;
	ny=num_y;
	nz=num_z;
	acell.clear();
	acell.resize(num_x*num_y*num_z);
	return 1;

}

bool cVirtualPlant::Resize(unsigned int num_cell)
{
	acell.resize(num_cell);
	if (num_cell==0){acell.clear();}
	return 1;
}


int cVirtualPlant::GetCellVectorID(int inx,int iny,int inz)
{
	/***** must add algorithm to 
	1. check if nx,ny,nz have been assigned
	2. check if inx>nx or iny>ny or inz>nz

	*******/
	return (nz*ny*(inx-1)+nz*(iny-1)+inz)-1;
}


unsigned int cVirtualPlant::ConvertIDtoNxyz(unsigned int inID,short int optionXYZ)
{
	int temp=0;
	if (optionXYZ==1) {temp=int(inID/(ny*nz))+1;}
	if (optionXYZ==2) {temp=int((inID-int(inID/(ny*nz))*ny*nz)/nz)+1;}
	if (optionXYZ==3) {temp=inID%nz+1;}
	return temp;
}



bool cVirtualPlant::PutCell(int inx,int iny,int inz,cell incell)
{
	int vector_id=GetCellVectorID(inx,iny,inz);
	acell[vector_id]=incell;
	return 1;
}

bool cVirtualPlant::PutCell(int vector_id,cell incell)
{
	acell[vector_id]=incell;
	return 1;
}


cell cVirtualPlant::GetCell(int inx,int iny,int inz)
{
	int vector_id=GetCellVectorID(inx,iny,inz);
	return acell[vector_id];
}

cell *cVirtualPlant::GetCellPointer(int inx,int iny,int inz)
{
	int vector_id=GetCellVectorID(inx,iny,inz);
	return &acell[vector_id];
}

/* removed to save memory
bool cVirtualPlant::PutLeaftoCell(int inx,int iny,int inz,leaf in_leaf)
{
	int vector_id=GetCellVectorID(inx,iny,inz);

	acell[vector_id].PutLeaf(in_leaf);
	return 1;
}


unsigned int cVirtualPlant::CountCell(bool option)
{ //count empty or non-empty cell;
	unsigned int temp=0,i=0;
	unsigned int total_cells=nx*ny*nz;
	switch (option)
	{
	case 0:
		for (i=0;i<total_cells;i++)
		{
			if (acell[i].GetNumber_of_leaf()==0) {++temp;}
		}
		break;
	case 1:
		for (i=0;i<total_cells;i++)
		{
			if (acell[i].GetNumber_of_leaf()>0) {++temp;}
		}
		break;
	}
	return temp;
}
*/
bool cVirtualPlant::PutMaxmin(double dxmin,double dymin,double dzmin,double dxmax,double dymax,double dzmax)
{
	xmin=dxmin;ymin=dymin;zmin=dzmin;
	xmax=dxmax;ymax=dymax;zmax=dzmax;
	return 1;
}

double cVirtualPlant::GetMinMax(bool max_min, short int xyz)
{
	double temp=9e9;
	this->CalculBasic();

	if (xyz>=1 && xyz<=3) 
	{
		switch (max_min)
		{
		case 0:
			if (xyz==1) {temp=xmin;}
			if (xyz==2) {temp=ymin;}
			if (xyz==3) {temp=zmin;}
			break;
		case 1:
			if (xyz==1) {temp=xmax;}
			if (xyz==2) {temp=ymax;}
			if (xyz==3) {temp=zmax;}
			break;
		}
	}
	else {temp=1e9;}
	return temp;
}

double cVirtualPlant::CalculVLAD()
{
	double sumLAD=0,sumLAD2=0;
//	UpdateLeafArea(); removed to save memory
	int totalcell=this->GetTotal_cells();
	for (int i=0;i<totalcell;i++)
	{
		double LAD=acell[i].GetSumSurface()/(dx*dy*dz);
		sumLAD+=LAD;
		sumLAD2+=pow(LAD,2);
	}

	double meanLAD=sumLAD/(totalcell);
	return sumLAD2/(totalcell)- pow(meanLAD,2);


/*
	xmoyenneLAD=sommeLAD/totalcell;
	double vlad=sommeLAD2/totalcell - pow(xmoyenneLAD,2);
	MeanLAD=xmoyenneLAD;
	MeanLADadj=sommeLAD/nadj;
	vadj=sommeLAD2/nadj - pow(xmoyenneLAD,2);
*/
}


double cVirtualPlant::CalculMeanLAD()
{
	double sumLAD=0;
	//UpdateLeafArea();
	for (unsigned int i=0;i<this->GetTotal_cells();i++)
	{
		double LAD=acell[i].GetSumSurface()/(dx*dy*dz);
		sumLAD+=LAD;
	}

	return sumLAD/this->GetTotal_cells();
}


short int cVirtualPlant::RayCellIntersect(cPoint *ray_origin,cPoint *ray_direction,cell *incell)
{// test box/ray intersection return 1=ray hit 0=missed -1=ray behind the box

	short int result=99;
	//start algorithm
	double tnear=-9e9,tfar=9e9;
	double t1=0,t2=0,temp=0;
//	AddDisplayText("OK1");
	//for x pair
	if (ray_direction->GetX()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetX()<incell->GetX() || ray_origin->GetX()>(incell->GetX()+dx)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to x plane
	{
		t1=(incell->GetX()-ray_origin->GetX())/ray_direction->GetX();
		t2=(incell->GetX()+dx-ray_origin->GetX())/ray_direction->GetX();
		if (t1>t2) //do swap
		{
			temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK2");

	//for y pair
	if (ray_direction->GetY()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetY()<incell->GetY() || ray_origin->GetY()>(incell->GetY()+dy)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to x plane
	{
		t1=(incell->GetY()-ray_origin->GetY())/ray_direction->GetY();
		t2=(incell->GetY()+dy-ray_origin->GetY())/ray_direction->GetY();
		if (t1>t2) //do swap
		{
			temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK3");

	//for z pair
	if (ray_direction->GetZ()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetZ()<incell->GetZ() || ray_origin->GetZ()>(incell->GetZ()+dz)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to z plane
	{
		t1=(incell->GetZ()-ray_origin->GetZ())/ray_direction->GetZ();
		t2=(incell->GetZ()+dz-ray_origin->GetZ())/ray_direction->GetZ();
		if (t1>t2) //do swap
		{
			temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK4");

	result=1;
end_check:

	//if (result==1){printf("Box is hit tnear=%6.4f	tfar=%6.4f	intersect=%6.4f\n",tnear,tfar,tfar-tnear);}
	return result;
}

	
void cVirtualPlant::ExportCellVGS(CString fname, unsigned int optionShape,unsigned int cellType)
// option 1=skeleton 2=box
// cell type 0=canopy cell(include some empty cell) 1=non-empty 2=all
{
	FILE *ExportFile;
	ExportFile=fopen(fname,"w");	
	fprintf(ExportFile,"Obj	EchX	EchY	EchZ	TransX	TransY	TransZ	RotX	RotY	RotZ	R	G	B\n");
	fclose(ExportFile);

//	ms.Format("		Output cells VegeSTAR filename=%s",fname);
//	AddDisplayText(ms);
	PrintSkeleton(fname,optionShape,cellType);

}





void cVirtualPlant::PrintSkeleton(CString fname,unsigned int shape, unsigned int cellType)
// cell type 0=all 1=convex 2=tyep3 3=non-empty 
{
	
	int total_line=0,exported_cell=0,i;
	if (shape==1) //output skeleton
	{
		switch (cellType) 
		{
		case 0:
			for (i=0;i<nx*ny*nz;i++) //all cell
			{
				PutSkeleton(i);total_line+=12;exported_cell++;
			}

			break;
		case 1:
			for (i=0;i<nx*ny*nz;i++) //convex
			{
				if (acell[i].GetCanopyCellIndex()==1) {PutSkeleton(i);total_line+=12;++exported_cell;}			
			}
			break;

		case 2:
			for (i=0;i<nx*ny*nz;i++) //volume tyep 3
			{
				if (acell[i].GetCanopyCellIndex()==1) {PutSkeleton(i);total_line+=12;++exported_cell;}			
			}
			break;


		case 3:
			for (i=0;i<nx*ny*nz;i++) //non-empty
			{
				if (acell[i].GetType()>0) {PutSkeleton(i);total_line+=12;++exported_cell;}			
			}
			break;

		case 4:
			for (i=0;i<nx*ny*nz;i++) //non-empty
			{
				if (acell[i].sum_surface>0) {PutSkeleton(i);total_line+=12;++exported_cell;}			
			}
			break;
		default:
			for (i=0;i<nx*ny*nz;i++) //non-empty
			{
				if (acell[i].sum_surface>0) {PutSkeleton(i);total_line+=12;++exported_cell;}
			}
			break;
		}
	}//if (shape==1)

	if (shape==2) //output box
	{
		unsigned int i;
		switch (cellType)
		{
		case 0: //bounding box
			
			PutBox(start_point.GetX(),start_point.GetY(),start_point.GetZ(),dx*nx,dy*ny,dz*nz);total_line+=6;++exported_cell;
			break;
		case 1: //non-empty only
			for (i=0;i<this->GetTotal_cells();i++)
			{
				//{PutBox(0);total_line+=6;++exported_cell;}			
				if (acell[i].sum_surface>0) {PutBox(i);total_line+=6;++exported_cell;}			
			}
			break;
		case 2: //non-empty + inner cell that have no way connect to external
			for (i=0;i<this->GetTotal_cells();i++)
			{
				if (acell[i].GetType()>0 && acell[i].GetType()<=2) {PutBox(i);total_line+=6;exported_cell++;}			
			}
			break;
		case 3:
			for (i=0;i<this->GetTotal_cells();i++)
			{
				if (acell[i].GetType()>0 && acell[i].GetType()<=3) {PutBox(i);total_line+=6;exported_cell++;}			
			}
			break;
		case 4:
			for (i=0;i<this->GetTotal_cells();i++)
			{
				if (acell[i].GetType()>0 && acell[i].GetType()<=4) {PutBox(i);total_line+=6;exported_cell++;}			
			}
			break;
		case 5: 
			for (i=0;i<this->GetTotal_cells();i++)
			{
				if (acell[i].GetType()>0 && acell[i].GetType()<=5) {PutBox(i);total_line+=6;exported_cell++;}			
			}
			break;
		case 6: //all cell
			for (i=0;i<this->GetTotal_cells();i++)
			{
				PutBox(i);total_line+=6;exported_cell++;			
			}
			break;
		case 7: //only keepit=1
			for (i=0;i<this->GetTotal_cells();i++)
			{
				//if (acell[i].keepit==1) 
				//{
					PutBox(i);total_line+=6;exported_cell++;
				//}			
			}

			break;
		default:
			for (i=0;i<this->GetTotal_cells();i++)
			{
				if (acell[i].sum_surface>0) {PutBox(i);total_line+=6;exported_cell++;}
			}
			break;
		}

	} //if (shape==2)

	//ms.Format("		total line=%i\n",total_line);
	//AddDisplayText(ms);


//********* Start of packing process****************************************************/
	//start find duplicate
	/*
	double unit=0;
	int opercent=-1;
	int percent=0;
	total_line=each_line.size();
	for (i=0;i<total_line;i++)
	{
		unit=total_line/100.00;
		percent=int(i/unit);
		if(percent>opercent)
		{
			opercent=percent;
			ms.Format("Packing VegeSTAR file %i%%",percent);DisplayStatus(ms);
		}
			int j=i+1;
			int check_line=nz*ny*2;
			while (j<total_line ||j-i<check_line)
			{
				if(each_line[j].dupIndex==0)
				{
					if (CompareLine(&each_line[i],&each_line[j]))
					{
						each_line[j].dupIndex=1;
						//total_line=each_line.size();
						//j--;
						//j=total_line;
							//vctImage.erase(&vctImage[ImageID]);
					}
				}
				j++;
			}
	} //end find duplicate
	
	VctVgsLine temp_line;
	for (i=0;i<total_line;i++)
	{
		if(each_line[i].dupIndex==0)
		{
			temp_line.push_back(each_line[i]);
		}
	}
	total_line=temp_line.size();
	each_line.resize(0);
	for (i=0;i<total_line;i++)
	{
		each_line.push_back(temp_line[i]);
	}
*/
//********* End of packing process****************************************************/
	ms.Format("Exporting VegeSTAR file.......");DisplayStatus(ms);
	total_line=each_line.size();
	FILE * pFileOutput;
	pFileOutput=fopen(fname,"at");
	int exported_line=0;
	for (i=0;i<total_line;i++)
	{
			exported_line++;
		fprintf(pFileOutput,"%i	%6.2f	%6.2f	%6.2f	%6.2f	%6.2f	%6.2f	%6.2f	%6.2f	%6.2f	255	0	0\n",
			each_line[i].Getobject(),each_line[i].GetEachX(),each_line[i].GetEachY(),each_line[i].GetEachZ(),
			each_line[i].GetTranX(),each_line[i].GetTranY(),each_line[i].GetTranZ(),
			each_line[i].GetRotX(),each_line[i].GetRotY(),each_line[i].GetRotZ());
	}
	
	fclose(pFileOutput);
	ms.Format("		Exported voxels=%i with %i data lines",exported_cell,exported_line);
	AddDisplayText(ms);
	
	if (cellType==0) {ms.Format("		Volume=%6.3f m3",nx*ny*nz*dx*dy*dz);AddDisplayText(ms);}
	else {ms.Format("		Volume=%6.3f m3",exported_cell*dx*dy*dz);AddDisplayText(ms);}

	each_line.clear(); //clear memory
}


void cVirtualPlant::PutSkeleton(int icell)
{
		double line_size=4;
		cVGSline templine;
		for (int j=1;j<=12;j++)
		{
			templine.Putobject(32);
				if(j<=3) //orgin point
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*line_size);templine.PutEachZ(dz*line_size);
					templine.PutTranX(acell[icell].GetX()*-100);templine.PutTranY(acell[icell].GetY()*-100);templine.PutTranZ(acell[icell].GetZ()*-100);
					templine.PutRotX(0);
					if (j==1) {templine.PutRotY(0);templine.PutRotZ(0+180);each_line.push_back(templine);}
					if (j==2) {templine.PutRotY(0);templine.PutRotZ(90+180);each_line.push_back(templine);}
					if (j==3) {templine.PutRotY(-270);templine.PutRotZ(0+180);each_line.push_back(templine);}
				}
				if(j>3 && j<=5) //slide x
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*line_size);templine.PutEachZ(dz*line_size);
					templine.PutTranX((acell[icell].GetX()+dx)*-100);templine.PutTranY(acell[icell].GetY()*-100);templine.PutTranZ(acell[icell].GetZ()*-100);
					templine.PutRotX(0);
					if (j==4) {templine.PutRotY(0);templine.PutRotZ(90+180);each_line.push_back(templine);}
					if (j==5) {templine.PutRotY(-270);templine.PutRotZ(0+180);each_line.push_back(templine);}
				}
				if(j>5 && j<=7) //slide y
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*line_size);templine.PutEachZ(dz*line_size);
					templine.PutTranX(acell[icell].GetX()*-100);templine.PutTranY((acell[icell].GetY()+dy)*-100);templine.PutTranZ(acell[icell].GetZ()*-100);
					templine.PutRotX(0);
					if (j==6) {templine.PutRotY(-270);templine.PutRotZ(0);each_line.push_back(templine);}
					if (j==7) {templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);}
				}
				if (j>7 && j<=10) //across orgin point
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*line_size);templine.PutEachZ(dz*line_size);
					templine.PutTranX((acell[icell].GetX()+dx)*-100);templine.PutTranY((acell[icell].GetY()+dy)*-100);templine.PutTranZ((acell[icell].GetZ()-dz)*-100);
					templine.PutRotX(0);
					if (j==8) {templine.PutRotY(-90);templine.PutRotZ(0+180);each_line.push_back(templine);}
					if (j==9) {templine.PutRotY(-180);templine.PutRotZ(0+180);each_line.push_back(templine);}
					if (j==10) {templine.PutRotY(0);templine.PutRotZ(-90+180);each_line.push_back(templine);}
				}
				if (j>10)
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*line_size);templine.PutEachZ(dz*line_size);
					templine.PutTranX(acell[icell].GetX()*-100);templine.PutTranY(acell[icell].GetY()*-100);templine.PutTranZ((acell[icell].GetZ()-dz)*-100);
					templine.PutRotX(0);
					if (j==11) {templine.PutRotY(0);templine.PutRotZ(0+180);each_line.push_back(templine);}
					if (j==12) {templine.PutRotY(0);templine.PutRotZ(90+180);each_line.push_back(templine);}
				}

		} //for 12 line round each box
}



bool cVirtualPlant::CompareLine(cVGSline *iline1,cVGSline  *iline2)
{
	bool temp=1;
	if (iline1->tranX!=iline2->tranX) {temp=0;goto end_check;}
	if (iline1->rotZ!=iline2->rotZ) {temp=0;goto end_check;}
	if (iline1->tranY!=iline2->tranY) {temp=0;goto end_check;}
	if (iline1->rotX!=iline2->rotX) {temp=0;goto end_check;}
	if (iline1->tranZ!=iline2->tranZ) {temp=0;goto end_check;}
	if (iline1->rotY!=iline2->rotY) {temp=0;goto end_check;}
end_check:
	return temp;
}


void cVirtualPlant::PutBox(int icell)
{
		cVGSline templine;
		for (int j=1;j<=6;j++)
		{
			cell tcell;
			tcell.PutXyz(acell[icell].GetY(),acell[icell].GetX(),acell[icell].GetZ());
			//tcell.PutXyz(0,0,0);
			templine.Putobject(22);
				if(j==1) //upper lower
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*100);templine.PutEachZ(1);
					templine.PutTranX(tcell.GetX()*100);
					templine.PutTranY((tcell.GetY()+dy/2)*100);
					templine.PutTranZ((tcell.GetZ()+dz)*-100);
					templine.PutRotX(0);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}
				if(j==2) //upper side
				{
					templine.PutEachX(dx*100);templine.PutEachY(dy*100);templine.PutEachZ(1);
					templine.PutTranX(tcell.GetX()*100);
					templine.PutTranY((tcell.GetY()+dy/2)*100);
					templine.PutTranZ(tcell.GetZ()*-100);
					templine.PutRotX(0);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}

				if(j==3) //x- side
				{
					templine.PutEachX(dz*100);templine.PutEachY(dy*100);templine.PutEachZ(1);
					templine.PutTranX(tcell.GetX()*100);
					templine.PutTranY((tcell.GetY()+dy/2)*100);
					templine.PutTranZ(tcell.GetZ()*-100);
					templine.PutRotX(0);
					templine.PutRotY(90);templine.PutRotZ(0);each_line.push_back(templine);
				}
				
				if(j==4) //x+ side
				{
					templine.PutEachX(dz*100);templine.PutEachY(dy*100);templine.PutEachZ(1);
					templine.PutTranX((tcell.GetX()+dx)*100);
					templine.PutTranY((tcell.GetY()+dy/2)*100);
					templine.PutTranZ(tcell.GetZ()*-100);
					templine.PutRotX(0);
					templine.PutRotY(90);templine.PutRotZ(0);each_line.push_back(templine);
				}
				
				if(j==5) //y- side
				{
					templine.PutEachX(dx*100);templine.PutEachY(dz*100);templine.PutEachZ(1);
					templine.PutTranX(tcell.GetX()*100);
					templine.PutTranY(tcell.GetY()*100);
					templine.PutTranZ((tcell.GetZ()+dz/2)*-100);
					templine.PutRotX(90);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);

				}
				
				if(j==6) //y- side
				{
					templine.PutEachX(dx*100);templine.PutEachY(dz*100);templine.PutEachZ(1);
					templine.PutTranX(tcell.GetX()*100);
					templine.PutTranY((tcell.GetY()+dy)*100);
					templine.PutTranZ((tcell.GetZ()+dz/2)*-100);
					templine.PutRotX(90);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}
		} 
}

void cVirtualPlant::PutBox(double inx,double iny, double inz,double indx,double indy,double indz)
{
		cVGSline templine;
		for (int j=1;j<=6;j++)
		{
			templine.Putobject(22);
			
				if(j==1) //upper side
				{
					templine.PutEachX(indx*100);templine.PutEachY(indy*100);templine.PutEachZ(1);
					templine.PutTranX((inx)*100);
					templine.PutTranY((iny+indy/2)*100);
					templine.PutTranZ((inz+indz)*-100);
					templine.PutRotX(0);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}
				
				if(j==2) //lower side
				{
					templine.PutEachX(indx*100);templine.PutEachY(indy*100);templine.PutEachZ(1);
					templine.PutTranX((inx)*100);
					templine.PutTranY((iny+indy/2)*100);
					templine.PutTranZ((inz)*-100);
					templine.PutRotX(0);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}

				if(j==3) //x- side
				{
					templine.PutEachX(indz*100);templine.PutEachY(indy*100);templine.PutEachZ(1);
					templine.PutTranX((inx)*100);
					templine.PutTranY((iny+indy/2)*100);
					templine.PutTranZ((inz)*-100);
					templine.PutRotX(0);
					templine.PutRotY(90);templine.PutRotZ(0);each_line.push_back(templine);
				}
			
				if(j==4) //x+ side
				{
					templine.PutEachX(indz*100);templine.PutEachY(indy*100);templine.PutEachZ(1);
					templine.PutTranX((inx+indx)*100);
					templine.PutTranY((iny+indy/2)*100);
					templine.PutTranZ((inz)*-100);
					templine.PutRotX(0);
					templine.PutRotY(90);templine.PutRotZ(0);each_line.push_back(templine);
				} 
			
				if(j==5) //Y+ side
				{
					templine.PutEachX(indx*100);templine.PutEachY(indz*100);templine.PutEachZ(1);
					templine.PutTranX((inx)*100);
					templine.PutTranY((iny)*100);
					templine.PutTranZ((inz+indz/2)*-100);
					templine.PutRotX(90);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}
				
				if(j==6) //Y- side
				{
					templine.PutEachX(indx*100);templine.PutEachY(indz*100);templine.PutEachZ(1);
					templine.PutTranX((inx)*100);
					templine.PutTranY((iny+indy)*100);
					templine.PutTranZ((inz+indz/2)*-100);
					templine.PutRotX(90);
					templine.PutRotY(0);templine.PutRotZ(0);each_line.push_back(templine);
				}
		} 
}

/*
void cVirtualPlant::ConsierCellType()
/ cell type 
	0=empty cell 
	1=non-empty 
	2=1 + empty cell that have no empty cell connection to outer
	3=2 + empty cell that all of 6 direction have canopy cell (cell type>0)
	4=3 + empty cell that locate between canopy cell in plane x,y 
	5=4 + empty cell that have canopy cell both up and down direction /
{
	int i=0,j=0,k=0,idnx=0,idny=0,idnz=0;

// first considered empty and non-empty cell
	for (i=0;i<nx*ny*nz;i++) // first considered empty and non-empty cell
	{
		if (acell[i].sum_surface>0) {acell[i].PutType(1);}
		else {acell[i].PutType(0);}	
	}

// second considered  empty cell that have no empty cell connection to outer
	//set connect_border to 1 for boarder which is empty

	// fill 6 side of border 
	
	for (j=1;j<=ny;j++) //side x-
	{
		for (k=1;k<=nz;k++)
		{
			if (acell[GetCellVectorID(1,j,k)].GetType()==0){acell[GetCellVectorID(1,j,k)].PutConnect(1);}
		}
	}
	
	for (j=1;j<=ny;j++) //side x+
	{
		for (k=1;k<=nz;k++)
		{
			if (acell[GetCellVectorID(nx,j,k)].GetType()==0)
			{acell[GetCellVectorID(nx,j,k)].PutConnect(1);}
		}
	}

	for (i=1;i<=nx;i++) //side y-
	{
		for (k=1;k<=nz;k++)
		{
			if (acell[GetCellVectorID(i,1,k)].GetType()==0){acell[GetCellVectorID(i,1,k)].PutConnect(1);}
		}
	}

	for (i=1;i<=nx;i++) //side y+
	{
		for (k=1;k<=nz;k++)
		{
			if (acell[GetCellVectorID(i,ny,k)].GetType()==0)
			{acell[GetCellVectorID(i,ny,k)].PutConnect(1);}
		}
	}

	for (j=1;j<=ny;j++) //side z-
	{
		for (i=1;i<=nx;i++)
		{
			if (acell[GetCellVectorID(i,j,1)].GetType()==0){acell[GetCellVectorID(i,j,1)].PutConnect(1);}
		}
	}
	
	for (j=1;j<=ny;j++) //side z+
	{
		for (i=1;i<=nx;i++)
		{
			if (acell[GetCellVectorID(i,j,nz)].GetType()==0)
			{acell[GetCellVectorID(i,j,nz)].PutConnect(1);}
		}
	}

	// End fill 6 side of border 
	
	int update_found=1;
	// scan inner cell until no found new update cell
	while (update_found>0) // scan inner cell until no found new update cell
	{
		update_found=0;
		for (i=2;i<=nx;i++) // scan inner cell form x-
		{
			int count=0;
			for (j=1;j<=ny;j++)
			{
				for (k=1;k<=nz;k++)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i-1,j,k)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
		//	printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 

		for (i=nx-1;i>=1;i--) // scan inner cell form x+ 
		{
			int count=0;
			for (j=ny;j>=1;j--)
			{
				for (k=nz;k>=1;k--)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i+1,j,k)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
			//printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 
		for (j=2;j<=ny;j++) // scan inner cell form y-
		{
			int count=0;
			for (i=1;i<=nx;i++)
			{
				for (k=1;k<=nz;k++)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i,j-1,k)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
			//printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 

		for (j=ny-1;j>=1;j--) // scan inner cell form y+ 
		{
			int count=0;
			for (i=nx;i>=1;i--)
			{
				for (k=nz;k>=1;k--)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i,j+1,k)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
			//printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 

		for (k=2;k<=nz;k++) // scan inner cell form z-
		{
			int count=0;
			for (j=1;j<=ny;j++)
			{
				for (i=1;i<=nx;i++)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i,j,k-1)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
			//printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 

		for (k=nz-1;k>=1;k--) // scan inner cell form x+ 
		{
			int count=0;
			for (j=ny;j>=1;j--)
			{
				for (i=nx;i>=1;i--)
				{
					if (acell[GetCellVectorID(i,j,k)].GetType()==0 &&
						acell[GetCellVectorID(i,j,k)].GetConnect()==0 &&
						acell[GetCellVectorID(i,j,k+1)].GetConnect()==1)
					{acell[GetCellVectorID(i,j,k)].PutConnect(1);count++;}
				}
			}
			//printf("nx=%i Border add=%i\n",i,count);
			update_found+=count;
		} 


	} //while (update_found>0) // scan inner cell until no found new update cell
	int count=0;
	for (i=0;i<nx*ny*nz;i++) // first considered empty and non-empty cell
	{
		if (acell[i].GetNumber_of_leaf()==0 && 
			acell[i].GetConnect()==0) {acell[i].PutType(2);count++;}
	}
	//printf("cell type 2 have %i cell\n",count);
// End second considered  empty cell that have no empty cell connection to outer

// Begin third consideration all of 6 direction have canopy cell
	update_found=1;
	while(update_found>0)
	{
		update_found=0;
		for (i=0;i<nx*ny*nz;i++) // third all of 6 direction have non-empty cell
		{
			if (acell[i].GetType()==0)
			{
				idnx=ConvertIDtoNxyz(i,1);
				idny=ConvertIDtoNxyz(i,2);
				idnz=ConvertIDtoNxyz(i,3);
				int side=1,count=0;
				//check downward
				j=idnz-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(idnx,idny,j)].GetType()>0){count++;break;}
					j--;
				}

				//check upward
				j=idnz+1;
				while (j<=nz)
				{
					if (acell[GetCellVectorID(idnx,idny,j)].GetType()>0){count++;break;}
					j++;

				}

				//check x-
				j=idnx-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(j,idny,idnz)].GetType()>0){count++;break;}
					j--;
				}

				//check x+
				j=idnx+1;
				while (j<=nx)
				{
					if (acell[GetCellVectorID(j,idny,idnz)].GetType()>0){count++;break;}
					j++;

				}

				//check y-
				j=idny-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(idnx,j,idnz)].GetType()>0){count++;break;}
					j--;
				}
				//check y+
				j=idny+1;
				while (j<=ny)
				{
					if (acell[GetCellVectorID(idnx,j,idnz)].GetType()>0){count++;break;}
					j++;

				}
				if (count==6) {acell[i].PutType(3);update_found++;} //printf("cell %i is empty count=%i\n",i,count);}

			} //if (acell[i].GetType()==0)
		}//for (i=0;i<nx*ny*nz;i++) // forth considered empty inside canopy
		//printf("update_found=%i\n",update_found);
	}
// End third consideration all of 6 direction have canopy cell

	Consider4();


//Begin considered	5=4 + empty cell that have canopy cell both up and down direction 
	update_found=1;
	while(update_found>0)
	{
		update_found=0;
		for (i=0;i<nx*ny*nz;i++) // first considered empty and non-empty cell
		{
				idnx=ConvertIDtoNxyz(i,1);
				idny=ConvertIDtoNxyz(i,2);
				idnz=ConvertIDtoNxyz(i,3);

		//for y
			if (acell[i].GetType()==0) 
			{
				count=0;
				//check downward
				j=idnz-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(idnx,idny,j)].GetType()>0){count++;break;}
					j--;
				}

				//check upward
				j=idnz+1;
				while (j<=nz)
				{
					if (acell[GetCellVectorID(idnx,idny,j)].GetType()>0){count++;break;}
					j++;

				}
				if (count==2) {acell[i].PutType(5);update_found++;}
			}
		}
		if (Consider4()==1) {update_found++;}
	}

//End considered	5=4 + empty cell that have canopy cell both up and down direction /

} 

bool cVirtualPlant::Consider4()
{
	int i,j,update_found,idnx,idny,idnz,count;
	bool rtn=0;
	//Begin considered 4=3 + empty cell that locate between canopy cell in plane x,y
	update_found=1;
	while(update_found>0)
	{
		update_found=0;
		//for x
		for (i=0;i<nx*ny*nz;i++) // first considered empty and non-empty cell
		{
				idnx=ConvertIDtoNxyz(i,1);
				idny=ConvertIDtoNxyz(i,2);
				idnz=ConvertIDtoNxyz(i,3);
			if (acell[i].GetType()==0) 
			{
				count=0;
				//check x-
				j=idnx-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(j,idny,idnz)].GetType()>0){count++;break;}
					j--;
				}

				//check x+
				j=idnx+1;
				while (j<=nx)
				{
					if (acell[GetCellVectorID(j,idny,idnz)].GetType()>0){count++;break;}
					j++;

				}
				if (count==2) {acell[i].PutType(4);update_found++;rtn=1;}
			}
		}
		for (i=0;i<nx*ny*nz;i++) // first considered empty and non-empty cell
		{
				idnx=ConvertIDtoNxyz(i,1);
				idny=ConvertIDtoNxyz(i,2);
				idnz=ConvertIDtoNxyz(i,3);

		//for y
			if (acell[i].GetType()==0) 
			{
				count=0;
				//check y-
				j=idny-1;
				while (j>0)
				{
					if (acell[GetCellVectorID(idnx,j,idnz)].GetType()>0){count++;break;}
					j--;
				}
				//check y+
				j=idny+1;
				while (j<=ny)
				{
					if (acell[GetCellVectorID(idnx,j,idnz)].GetType()>0){count++;break;}
					j++;

				}
				if (count==2) {acell[i].PutType(4);update_found++;rtn=1;}
			}
		}
	}
//End considered 4=3 + empty cell that locate between canopy cell in plane x,y
	return rtn;
} */

int cVirtualPlant::Reconstruct(int nx,int ny,int nz,double dx,double dy,double dz,cPoint startPoint)
{//reconstruction virtual plant and return number of total cells
	int id=1;
	acell.clear();
	PutDxyz(dx,dy,dz);
	start_point=startPoint; //set starting point of virtual canopy=xmin,ymin,zmin of bounding box
	//Resize(nx,ny,nz);
	lifecell=nx*ny*nz;
	cell tempcell;
	int i,j,k;
	for (i=0;i<nx;i++)
	{
		for(j=0;j<ny;j++)
		{
			for (k=0;k<nz;k++)
			{
//				tempcell.PutNxyz(i,j,k);
				tempcell.included_in_canopy=1;
				tempcell.keepit=1;
				tempcell.haveleaf=1;
				tempcell.ID=id;

				//calculate point for each cell
				tempcell.PutXyz(start_point.GetX()+(i*dx),start_point.GetY()+(j*dy),start_point.GetZ()+(k*dz));
				acell.push_back(tempcell);
				id++;
				//PutCell(i+1,j+1,k+1,tempcell);
			}
		}
	}
	/*
	for (i=1;i<100;i++)
	{
		CTreeAnalyserDlg::DisplayStatus("test Ok");
	}
*/	double xmin,ymin,zmin,xmax,ymax,zmax;
	xmin=start_point.GetX();
	ymin=start_point.GetY();
	zmin=start_point.GetZ();

	xmax=start_point.GetX()+nx*dx;
	ymax=start_point.GetY()+ny*dy;
	zmax=start_point.GetZ()+nz*dz;

	this->PutMaxmin(xmin,ymin,zmin,xmax,ymax,zmax);
	return nx*ny*nz;
}

void cVirtualPlant::ClearHaveLeaf()
{
	for (unsigned int i=0;i<this->GetTotal_cells();i++)
	{
		acell[i].haveleaf=0;
	}

	/*
	cell *tempcell;
	int i,j,k;
	for (i=1;i<=nx;i++)
	{
		for(j=1;j<=ny;j++)
		{
			for (k=1;k<=nz;k++)
			{
				tempcell=GetCellPointer(i,j,k);
				tempcell->haveleaf=0;
				
			}
		}
	}
*/

}


void cVirtualPlant::ResetKeepIt()
{
	lifecell=0;
	CString ms;
	unsigned int allbefore=this->GetTotal_cells();
	int percent=0,oldpercent=-1;
	//for (unsigned int i=0;i<this->GetTotal_cells();i++)
	double new_xmin=9e9,new_ymin=9e9,new_zmin=9e9;
	double new_xmax=-9e9,new_ymax=-9e9,new_zmax=-9e9;
	VctCell bcell;
	for (unsigned int i=allbefore;i>0;i--)
	{
		percent=allbefore-i;
		percent=int(percent*100.0/allbefore);
		ms.Format("Resizing virtual canopy %i %%",percent);
		if(percent!=oldpercent){this->DisplayStatus(ms);}
		oldpercent=percent;
		

		if(acell[i-1].haveleaf==1) 
		{	bcell.push_back(acell[i-1]); 
			lifecell++;
			//acell[i].keepit=1;
			if(acell[i-1].GetX()<new_xmin){new_xmin=acell[i-1].GetX();}
			if(acell[i-1].GetY()<new_ymin){new_ymin=acell[i-1].GetY();}
			if(acell[i-1].GetZ()<new_zmin){new_zmin=acell[i-1].GetZ();}
			if(acell[i-1].GetX()+dx>new_xmax){new_xmax=acell[i-1].GetX()+dx;}
			if(acell[i-1].GetY()+dy>new_ymax){new_ymax=acell[i-1].GetY()+dy;}
			if(acell[i-1].GetZ()+dz>new_zmax){new_zmax=acell[i-1].GetZ()+dz;}
		} //
		
		//else
		//{
			//acell[i].keepit=0;
		//	acell.erase(&acell[i-1]);
		//}
		
	}//for (unsigned int i=allbefore;i>0;i--)
	acell.clear(); //clear memory
	for(i=0;i<bcell.size();i++)
	{
		acell.push_back(bcell[i]);
	}
	bcell.clear(); //clear memory
	xmin=new_xmin;
	ymin=new_ymin;
	zmin=new_zmin;
	xmax=new_xmax;
	ymax=new_ymax;
	zmax=new_zmax;

	this->DisplayStatus();

}


short int cVirtualPlant::RayCanopyIntersect(cPoint *ray_origin,cPoint *ray_direction,double *tin,double *tout)
{// test box/ray intersection return 1=ray hit 0=missed -1=ray behind the box

	short int result=99;
	//start algorithm
	double tnear=-9e9,tfar=9e9;
//	AddDisplayText("OK1");
	//for x pair
	if (ray_direction->GetX()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetX()<this->GetMinMax(0,1) || ray_origin->GetX()>this->GetMinMax(1,1)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to x plane
	{
		double t1=(this->GetMinMax(0,1)-ray_origin->GetX())/ray_direction->GetX();
		double t2=(this->GetMinMax(1,1)-ray_origin->GetX())/ray_direction->GetX();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK2");

	//for y plane
	if (ray_direction->GetY()==0) //if ray is parallel to y plane
	{
		if (ray_origin->GetY()<this->GetMinMax(0,2) || ray_origin->GetY()>this->GetMinMax(1,2)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to y plane
	{
		double t1=(this->GetMinMax(0,2)-ray_origin->GetY())/ray_direction->GetY();
		double t2=(this->GetMinMax(1,2)-ray_origin->GetY())/ray_direction->GetY();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK3");

	//for z pair
	if (ray_direction->GetZ()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetZ()<this->GetMinMax(0,3) || ray_origin->GetZ()>this->GetMinMax(1,3)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to z plane
	{
		double t1=(this->GetMinMax(0,3)-ray_origin->GetZ())/ray_direction->GetZ();
		double t2=(this->GetMinMax(1,3)-ray_origin->GetZ())/ray_direction->GetZ();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK4");
	*tin=tnear; *tout=tfar;
	result=1;
end_check:

	//if (result==1){printf("Box is hit tnear=%6.4f	tfar=%6.4f	intersect=%6.4f\n",tnear,tfar,tfar-tnear);}
	return result;
}


short int cVirtualPlant::RayCellIntersect(cPoint *ray_origin,cPoint *ray_direction,cell *incell,double *tin,double *tout)
{// test box/ray intersection return 1=ray hit 0=missed -1=ray behind the box

	short int result=99;
	//start algorithm
	double tnear=-9e9,tfar=9e9;
	double t1x=0,t1y=0,t1z=0,t2x=0,t2y=0,t2z=0;
//	AddDisplayText("OK1");
	//for x pair
	if (ray_direction->GetX()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetX()<incell->GetX() || ray_origin->GetX()>(incell->GetX()+dx)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to x plane
	{
		double t1=(incell->GetX()-ray_origin->GetX())/ray_direction->GetX();
		double t2=(incell->GetX()+dx-ray_origin->GetX())/ray_direction->GetX();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray
		t1x=tnear;
		t2x=tfar;
		

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK2");

	//for y pair
	if (ray_direction->GetY()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetY()<incell->GetY() || ray_origin->GetY()>(incell->GetY()+dy)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to x plane
	{
		double t1=(incell->GetY()-ray_origin->GetY())/ray_direction->GetY();
		double t2=(incell->GetY()+dy-ray_origin->GetY())/ray_direction->GetY();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray
		t1y=tnear;
		t2y=tfar;

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK3");

	//for z pair
	if (ray_direction->GetZ()==0) //if ray is parallel to x plane
	{
		if (ray_origin->GetZ()<incell->GetZ() || ray_origin->GetZ()>(incell->GetZ()+dz)) { result=0; goto end_check;}
	}
	else //if ray is not parallel to z plane
	{
		double t1=(incell->GetZ()-ray_origin->GetZ())/ray_direction->GetZ();
		double t2=(incell->GetZ()+dz-ray_origin->GetZ())/ray_direction->GetZ();
		if (t1>t2) //do swap
		{
			double temp=t2;
			t2=t1;t1=temp;
		}
		if(t1>tnear) {tnear=t1;}
		if(t2<tfar) {tfar=t2;}
		if (tnear>tfar) { result=0; goto end_check;} //box is missed
		if (tfar<0) { result=-1; goto end_check;} //box is behind the ray
		t1z=tnear;
		t2z=tfar;

	} //else //if ray is not parallel to x plane
//	AddDisplayText("OK4");
	*tin=tnear;
	*tout=tfar;
	result=1;
end_check:

	//if (result==1){printf("Box is hit tnear=%6.4f	tfar=%6.4f	intersect=%6.4f\n",tnear,tfar,tfar-tnear);}
	return result; 
}
int cVirtualPlant::AddCell(cell incell)
{
	acell.push_back(incell);
	return acell.size();
}

bool cVirtualPlant::IsLeafInVolume(leaf in_leaf)
{
	bool inside=false;
	int total_cell=acell.size();
	for(int i=0;i<total_cell;i++)
	{
		if(in_leaf.GetX()>=acell[i].GetX()*100 && in_leaf.GetX()<=(acell[i].GetX()+dx)*100)
		{
			if(in_leaf.GetY()>=acell[i].GetY()*100 && in_leaf.GetY()<=(acell[i].GetY()+dy)*100)
			{
				if(in_leaf.GetZ()>=acell[i].GetZ()*100 && in_leaf.GetZ()<=(acell[i].GetZ()+dz)*100)
				{
					inside=true;
					goto end;
				}
			}
		}
	}
end:;
	return inside;
}

void cVirtualPlant::ClearIntersectVolume(void)
{
	for(unsigned int i=0;i<GetTotal_cells();i++)
	{
		acell[i].intersect_volume=0;
	}
}

void cVirtualPlant::ShareLeafToVoxel(double InputArea)
{
	double sum_intersected_volume=0;
	for(unsigned int i=0;i<GetTotal_cells();i++)
	{
		sum_intersected_volume+=acell[i].intersect_volume;
	}

	double actual_volume=this->GetCanopyVolume()-sum_intersected_volume;
	double LAD=InputArea/actual_volume;
	unsigned int totalcell=GetTotal_cells();
	double cellvolume=this->GetCellVolume();
	for(i=0;i<totalcell;i++)
	{
		if(acell[i].sum_surface>0)
		{
			acell[i].sum_surface+=(cellvolume-acell[i].intersect_volume)*LAD;
		}
	}
}

void cVirtualPlant::OutputVerticalLAD(CString fname, double range)
{
	double canopy_height=this->GetMinMax(1,3); //get height of canopy
	int number_cat=0;
	//calculate number of category
	if(mod(canopy_height,range)==0){number_cat=int(canopy_height/range);}
	else {number_cat=int(canopy_height/range)+1;}

	//create array of each layer
	std::vector<double> vctCategory;
	for(int i=0;i<number_cat;i++)
	{
		vctCategory.push_back(0);
	}


	//ms.Format("Height=%6.4f  Range=%6.4f  Cat=%i",canopy_height,range,number_cat);
	//MessageBox(NULL,ms,"Test",0);

	unsigned int totalcell=GetTotal_cells();
	double cell_height=Getdz();
	//int overlap=0;
	for(unsigned int i=0;i<totalcell;i++)
	{
		double locatez=acell[i].GetZ();
		//find if current cell is between the range
		int position1=int((locatez+0.00001)/range);
		int position2=int((locatez+cell_height-0.00001)/range);
		if (position1==position2)//if current cell is not overlap
		{
			vctCategory[position1]+=acell[i].sum_surface;
		}
		else //if current cell is overlap
		{
			//see if current overlap more than 1 layer
			//int n_layer=position2-position1+1; //number of layer overlap the current cell
			//The lowest point is in the layer position1
			double old_point=locatez;
			for(int j=position1;j<=position2;j++)
			{
				//calculate point of the next layer
				double next_point=position1*range;
				double width=next_point-old_point;
				vctCategory[j]+=(width/cell_height)*acell[i].sum_surface;
				old_point=next_point;
			}
		}
	}

	FILE * pFile;
	pFile=fopen(fname,"wt");
	fprintf(pFile,"Layer\tleaf_area\n");

	for(i=0;i<vctCategory.size();i++)
	{
		fprintf(pFile,"%i	%6.4f\n",i,vctCategory[i]);
	}
	fclose(pFile);
}

double cVirtualPlant::GetTotalLeafArea(void)
{
	double SumLeafArea=0;
	for(unsigned int i=0;i<GetTotal_cells();i++)
	{
		SumLeafArea+=acell[i].sum_surface;
	}
	return SumLeafArea;
}

void cVirtualPlant::DisplayCanopyProperties(void)
{
		
	this->AddDisplayText("\r\nVirtual canopy properties:");
	ms.Format("	Total voxels=%i  size = %2.1f x %2.1f x %2.1f cm",GetTotal_cells(),Getdx()*100,Getdy()*100,Getdz()*100);
	this->AddDisplayText(ms);
	ms.Format("	Height =%6.2f m  Vegetation height =%6.2f m",GetMinMax(1,3),GetMinMax(1,3)-GetMinMax(0,3));
	this->AddDisplayText(ms);
	ms.Format("	Diameter N-S =%6.2f m  Diameter E-W =%6.2f m",GetMinMax(1,2)-GetMinMax(0,2),GetMinMax(1,1)-GetMinMax(0,1));
	this->AddDisplayText(ms);
	ms.Format("	Volume=%6.4f m3",GetTotal_cells()*GetCellVolume());
	this->AddDisplayText(ms);
	ms.Format("	Total leaf area=%6.4f m2",GetTotalLeafArea());
	this->AddDisplayText(ms);
	ms.Format("	Mean LAD=%6.4f",this->CalculMeanLAD());
	this->AddDisplayText(ms);
	ms.Format("	Variance of LAD=%6.4f",this->CalculVLAD());
	this->AddDisplayText(ms);

}

double cVirtualPlant::CalculP0(cPoint *ray_origin,cPoint *ray_direction)
{
	double t1=0,t2=0;
	if(this->RayCanopyIntersect(ray_origin,ray_direction,&t1,&t2))
	{
		double p0=1;
		return p0;
	}
	else // ray missed for behind the canopy
	{
		return 1;
	}
}

void cVirtualPlant::ResetVoxelPathLength()
{
	//int ncell=acell.size();
	for (unsigned int i=0;i<acell.size();i++)
	{
		acell[i].pathLength=0;
	}
}

//! A function to to display profile of volume occupied by the canopy
/*!
*	Created: 11-Jul-04
*	\n Last modification: 21-Sep-04.\n
*	Called by:
*	- CTreeAnalyserDlg::OnCanopyVerticalprofileofleafarea() 
*	
*/
void cVirtualPlant::CalculVerticalProfileLeafArea(void)
{
	double top=GetMinMax(1,3)+ dz;
	int cat=int(top/dz);
	int i=0;


	std::vector<double> vctdouble;
	std::vector<int> vctncell;
	for(i=0;i<cat;i++){vctdouble.push_back(0);vctncell.push_back(0);} // set initial value

	for(unsigned int j=0;j<this->GetTotal_cells();j++)
	{
		int layer=int((acell[j].GetZ()+dz-0.00000001)/dz); //-0.00000001 prevent bug of floating point
		vctdouble[layer]+=acell[j].GetSumSurface();
		vctncell[layer]+=1;
	}

	//display result
	AddDisplayText("");
    ms.Format("Layer(m)\tSumArea\t%%\tVolume");
	AddDisplayText(ms);

	for(i=0;i<cat;i++) 
	{
		double lower=i*dz;
		double upper=(i+1)*dz;
		double percent=0;
		if(this->GetTotalLeafArea()>0){percent=100*vctdouble[i]/this->GetTotalLeafArea();}
		ms.Format("%4.2f-%4.2f\t%6.4f\t%4.4f\t%6.4f",lower,upper,vctdouble[i],percent,vctncell[i]*this->GetCellVolume());
		AddDisplayText(ms);
	}
	vctdouble.clear();
	
}


//! A function to display profile of volume occupied by the canopy
/*!
*	Created: 21-Sep-04
*	\n Last modification: 28-Sep-04.\n
*	Called by:
*	- CTreeAnalyserDlg::OnCanopyVerticalprofileofvolume() 
*	
*/
void cVirtualPlant::CalculVerticalProfileVolume(void)
{
	
	double top=GetMinMax(1,3);
	int cat=int(top/dz);
	int i=0;


	std::vector<double> vctdouble;
	for(i=0;i<cat;i++){vctdouble.push_back(0);} // set initial value

	for(unsigned int j=0;j<this->GetTotal_cells();j++)
	{
		int layer=int((acell[j].GetZ()+(dz/2))/dz); //-0.00000001 prevent bug of floating point
		vctdouble[layer]+=dx*dy*dz;
	}

	//display result
	AddDisplayText("");
    AddDisplayText("Vertical profile of occupied volume");
	ms.Format("Layer(m)\tSumVolume(m3)\t%%");
	AddDisplayText(ms);

	for(i=0;i<cat;i++) 
	{
		double lower=i*dz;
		double upper=(i+1)*dz;
		double percent=0;
		if(this->GetCanopyVolume()>0){percent=100*vctdouble[i]/this->GetCanopyVolume();}
		ms.Format("%4.2f-%4.2f\t%6.6f\t%4.4f",lower,upper,vctdouble[i],percent);
		AddDisplayText(ms);
	}
	vctdouble.clear();
	
}//void cVirtualPlant::CalculVerticalProfileVolume(void)

double cVirtualPlant::CalculCellDistance(int cell1,int cell2)
{
	double temp=pow(acell[cell1].GetX()-acell[cell2].GetX(),2)+pow(acell[cell1].GetY()-acell[cell2].GetY(),2)+pow(acell[cell1].GetZ()-acell[cell2].GetZ(),2);
	return sqrt(temp);
}

//! Reset leaf area in every voxels =0
/*!
*	Created: 2003
*	\n Last modification: 2003\n
*	Called by:
*	-# CTreeAnalyserDlg::CaculLAD1(void)
*/
void cVirtualPlant::ClearSumsurface(void)
{
		for (unsigned int i=0;i<this->GetTotal_cells();i++)
	{
		acell[i].sum_surface=0;
	}
}


//! A function to calculate basic information of the canopy, ie. max, min of x,y,z
/*!
*	Created: 2003
*	\n Last modification: 28-Sep-04.\n
*	Called by:
*	-# CTreeAnalyserDlg::CaculLAD1(void)
*	-# cVirtualPlant::GetMinMax(bool max_min, short int xyz)
*/
void cVirtualPlant::CalculBasic(void)
{
	xmax=-9e9;ymax=-9e9;zmax=-9e9;
	xmin=9e9;ymin=9e9;zmin=9e9;

	for(unsigned int i=0;i<acell.size();i++)
	{
		if(acell[i].GetX()<xmin){xmin=acell[i].GetX();}
		if(acell[i].GetY()<ymin){ymin=acell[i].GetY();}
		if(acell[i].GetZ()<zmin){zmin=acell[i].GetZ();}

		if(acell[i].GetX()+dx>xmax){xmax=acell[i].GetX()+dx;}
		if(acell[i].GetY()+dy>ymax){ymax=acell[i].GetY()+dy;}
		if(acell[i].GetZ()+dz>zmax){zmax=acell[i].GetZ()+dz;}
	}
}

void cVirtualPlant::ExportRATPvoxel(CString filename)
{
	DisplayStatus("Saving RATP voxels....");
	FILE * pFile;
	pFile=fopen(filename,"wt");
	fprintf(pFile,"#voxel_x\t#voxel_y\t#voxel_z\t#vt\tLAD(m2 m-3)\tN(g m-2\n");
	int count=0;
	int total_nx=int((xmax-xmin)/dx),total_ny=int((ymax-ymin)/dy),total_nz=int(zmax/dz);
	if((xmax-xmin)-total_nx*dx>0){total_nx+=1;}
	if((ymax-ymin)-total_ny*dy>0){total_ny+=1;}
	if(zmax-total_nz*dz>0){total_nz+=1;} //z start from 0

	this->nx=total_nx;
	this->ny=total_ny;
	this->nz=total_nz;

	for(unsigned int i=GetTotal_cells()-1;i<GetTotal_cells();i--)
	{
		if(acell[i].GetSumSurface()>0)
		{	count++;
			
			int nx=int((acell[i].GetX()-xmin-0.00001)/dx)+1;
			int ny=int((acell[i].GetY()-ymin-0.00001)/dy)+1;
			int nz=int((acell[i].GetZ()+1e-9)/dz)+1;

			if(acell[i].GetX()-xmin-(nx-1)*dx>0){nx+=1;}
			if(acell[i].GetY()-ymin-(ny-1)*dy>0){ny+=1;}
			if(zmax-(nz-1)*dz<1e-9){nz+=1;} //z start from 0
/*
			if (nx==3)
			{
				double a=acell[i].GetX();
				int b=3;
			}*/
			fprintf(pFile,"%i\t%i\t%i\t1\t%6.4f\t2.0\n",nx,ny,total_nz-nz+1,acell[i].GetSumSurface()/(dx*dy*dz));
		}
	}

	fclose(pFile);
	ms.Format("Exported %i voxels",count);
	AddDisplayText(ms);
	DisplayStatus();
}


void cVirtualPlant::SaveGridParameters(CString filename)
{
	DisplayStatus("Saving grid parameters....");
	FILE * pFile;
	pFile=fopen(filename,"wt");
	fprintf(pFile,"%i %i %i\t\t!nx,ny,nz\n",this->GetNx(),this->GetNy(),this->GetNz());
	fprintf(pFile,"%6.4f %6.4f ",this->Getdx(),this->Getdy());
	for (int i=0;i<this->GetNz();i++)
	{
		fprintf(pFile,"%6.4f ",this->Getdz());
	}
	fprintf(pFile,"\t!dx,dy,(dz(jz),jz=1,njz)\n");
	fprintf(pFile,"%6.4f %6.4f %6.4f\t\t!Grid origin\n",0,0,0);
	fprintf(pFile,"45.78 3.08 0.0\t\t!latitude, longitude, time zone (Clermont-Ferrand)\n");
	fprintf(pFile,"0.0\t\t!Angle between X+ and North\n");
	fprintf(pFile,"0\t\t!offset between canopy units along Y axis\n");
	fprintf(pFile,"1\t\t!Number of vegetation entities in the 3D grid\n");

	fclose(pFile);
	DisplayStatus();
}

void cVirtualPlant::RandomLeafArea(void)
{

	srand( (unsigned)time( NULL ) );
	unsigned int sw;
	//32767

	std::vector<double> vctsf;
	for(unsigned int i=0; i<acell.size();i++)
	{
		vctsf.push_back(acell[i].GetSumSurface());
	}
	int ncount=0;

	while(vctsf.size()) 
	{
		sw=unsigned int(vctsf.size()*rand()/32767.0); //random number voxel
		if(sw>=vctsf.size()){sw=vctsf.size()-1;} 
		acell[ncount].sum_surface=vctsf[sw]; //set new value to voxel
		vctsf.erase(&vctsf[sw]); //delete setted value
		ncount++;
	}
}


void cVirtualPlant::ResetVoxelID(void)
{
	for(unsigned int i=0; i<acell.size();i++)
	{
		acell[i].ID=i;
	}
}

void cVirtualPlant::ExportPlantGL(CString filename)
{
	DisplayStatus("Saving PlantGL file....");
	FILE * pFile;
	pFile=fopen(filename,"wt");
	fprintf(pFile,"Material m\x7B Ambient<255,0,0>\x7D \n");
	double cx=0,cy=0,cz=0;
	for(unsigned int i=0;i<acell.size();i++)
	{
		cx=acell[i].GetX()+dx/2;
		cy=acell[i].GetY()+dy/2;
		cz=acell[i].GetZ()+dz/2;
		fprintf(pFile,"Translated voxel%i \x7B Translation<%6.4f,%6.4f,%6.4f> ",i,cx,cy,cz);
		
		fprintf(pFile,"Geometry Box\x7B Size<%6.4f,%6.4f,%6.4f> \x7D \x7D\n",dx/2,dy/2,dz/2);
		fprintf(pFile,"Shape \x7B voxel%i,m \x7D\n",i);
	}
	fclose(pFile);
	ms.Format("Exported %i voxels\x7B\x7D",acell.size());
	AddDisplayText(ms);
	DisplayStatus();
	
	
}

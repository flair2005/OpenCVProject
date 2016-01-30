// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVMat.h"

UCVMat::UCVMat()
{
	myCVMat = new cv::Mat();
}

UCVMat::~UCVMat()
{
	delete myCVMat;
}

void UCVMat::CVMatInfo(int32 &colsOut, int32 &rowsOut, int32 &stepOut, int32 &totalOut, int32 &elemSizeOut, int32 &channelsOut)
{
	if (myCVMat)
	{
		colsOut = myCVMat->cols;
		rowsOut = myCVMat->rows;
		stepOut = myCVMat->step;
		totalOut = myCVMat->total();
		elemSizeOut = myCVMat->elemSize();
		channelsOut = myCVMat->channels();
	}
	else
		return;
}

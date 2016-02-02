// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVMat.h"

UCVMat::UCVMat()
{
	cvMat = cv::Mat();
}

void UCVMat::CVMatInfo(int32 &colsOut, int32 &rowsOut, int32 &stepOut, int32 &totalOut, int32 &elemSizeOut, int32 &channelsOut)
{
	colsOut = cvMat.cols;
	rowsOut = cvMat.rows;
	stepOut = cvMat.step;
	totalOut = cvMat.total();
	elemSizeOut = cvMat.elemSize();
	channelsOut = cvMat.channels();
}

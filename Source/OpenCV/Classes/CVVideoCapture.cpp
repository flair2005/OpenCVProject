// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVVideoCapture.h"

UCVVideoCapture::UCVVideoCapture()
{
	// using new/delete caused crashes... ?
	//cvVideoCapturePointer = new cv::VideoCapture();
	cvVideoCapture = static_cast<cv::VideoCapture *>(FMemory::Malloc(sizeof(cv::VideoCapture)));
	// manually construct the cv::VideoCapture 
	new (cvVideoCapture) cv::VideoCapture();

}

UCVVideoCapture::~UCVVideoCapture()
{
	// manually destruct the cv::VideoCapture
	cvVideoCapture->~VideoCapture();
	FMemory::Free(cvVideoCapture);
}

bool UCVVideoCapture::OpenCam(int32 CameraID)
{
	if (cvVideoCapture)
	{
		return cvVideoCapture->open(CameraID);
	}
	else
	{
		return false;
	}
}

bool UCVVideoCapture::UCVVideoCapture::IsOpened()
{
	if (cvVideoCapture)
		return cvVideoCapture->isOpened();
	else
		return false;
}

bool UCVVideoCapture::Capture(UCVMat * UCVMat)
{
	if (cvVideoCapture && cvVideoCapture->isOpened() && UCVMat)
		return cvVideoCapture->read(UCVMat->cvMat);
	else
		return false;
}
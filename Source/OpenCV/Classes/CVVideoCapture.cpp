// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVVideoCapture.h"

UCVVideoCapture::UCVVideoCapture()
{
	// using new/delete caused crashes... ?
	//cvVideoCapturePointer = new cv::VideoCapture();
	myCVVideoCapture = static_cast<cv::VideoCapture *>(FMemory::Malloc(sizeof(cv::VideoCapture)));
	// manually construct the cv::VideoCapture 
	new (myCVVideoCapture) cv::VideoCapture();

}

UCVVideoCapture::~UCVVideoCapture()
{
	// manually destruct the cv::VideoCapture
	myCVVideoCapture->~VideoCapture();
	FMemory::Free(myCVVideoCapture);
}

bool UCVVideoCapture::OpenCam(int32 CameraID)
{
	if (myCVVideoCapture)
	{
		return myCVVideoCapture->open(CameraID);
	}
	else
	{
		return false;
	}
}

bool UCVVideoCapture::UCVVideoCapture::IsOpened()
{
	if (myCVVideoCapture)
		return myCVVideoCapture->isOpened();
	else
		return false;
}

bool UCVVideoCapture::Capture(UCVMat * UCVMatIn)
{
	if (myCVVideoCapture && myCVVideoCapture->isOpened() && UCVMatIn)
		return myCVVideoCapture->read(UCVMatIn->cvMat);
	else
		return false;
}
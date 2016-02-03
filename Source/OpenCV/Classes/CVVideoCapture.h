// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "opencv2/videoio.hpp"
#include "CVMat.h"
#include "CVVideoCapture.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class OPENCV_API UCVVideoCapture : public UObject
{
	GENERATED_BODY()

public:
	UCVVideoCapture();
	~UCVVideoCapture();

	UFUNCTION(BlueprintCallable, Category = "CVVideoCapture")
		bool OpenCam(int32 CameraID);

	UFUNCTION(BlueprintCallable, Category = "CVVideoCapture")
		bool IsOpened();

	UFUNCTION(BlueprintCallable, Category = "CVVideoCapture")
		bool Capture(UCVMat * UCVMatIn);

	cv::VideoCapture * cvVideoCapture;
};

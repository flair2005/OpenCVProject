// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "opencv2/core.hpp"
#include "CVMat.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class OPENCV_API UCVMat : public UObject
{
	GENERATED_BODY()

public:
	UCVMat();
	~UCVMat();

	UFUNCTION(BlueprintPure, Category = "CVMat")
		void CVMatInfo(int32 & colsOut, int32 & rowsOut, int32 & stepOut, int32 & totalOut, int32 & elemSizeOut, int32 & channelsOut);

	cv::Mat * myCVMat;
};

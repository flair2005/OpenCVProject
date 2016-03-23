// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "opencv2/core.hpp"
#include "Mat.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct OPENCV_API FMat
{
	GENERATED_USTRUCT_BODY()
	cv::Mat cvMat;
};

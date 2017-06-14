// Released under the Creative Commons Alexis Andersen 2017

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

// Released under the Creative Commons Alexis Andersen 2017

#pragma once

#include "Engine.h"
#include "opencv2/videoio.hpp"
#include "VideoCapture.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct OPENCV_API FVideoCapture
{
	GENERATED_USTRUCT_BODY()
	cv::VideoCapture cvVideoCapture;
};

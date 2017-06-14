// Released under the Creative Commons Alexis Andersen 2017

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Mat.h"
#include "VideoCapture.h"
#include "OpenCVBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OPENCV_API UOpenCVBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OpenCV,VideoIO")
		static void OpenCam(FVideoCapture videoCapture, FVideoCapture & videoCaptureOut, int32 CameraID, bool &isOpen);

	UFUNCTION(BlueprintPure, Category = "OpenCV,VideoIO")
		static void IsOpened(FVideoCapture videoCapture, bool &isOpen);

	UFUNCTION(BlueprintCallable, Category = "OpenCV,VideoIO")
		static void Capture(FMat mat, FMat & matOut, FVideoCapture videoCapture, bool &isCaptured);

	UFUNCTION(BlueprintPure, Category = "OpenCV,Mat")
		static void MatInfo(FMat mat, int32 &colsOut, int32 &rowsOut, int32 &stepOut, int32 &totalOut, int32 &elemSizeOut, int32 &channelsOut);

	UFUNCTION(BlueprintCallable, Category = "OpenCV,Tools")
		static void CreateTransient(int32 width, int32 height, UTexture2D * Texture);

	UFUNCTION(BlueprintCallable, Category = "OpenCV,Mat")
		static void RenderToTexture(FMat mat, UTexture2D * Texture);
};

// All Rights Reserved Benjamin David Andersen (SteamyThePunk) 2016

#include "OpenCV.h"
#include "OpenCVBPLibrary.h"

void UOpenCVBPLibrary::OpenCam(FVideoCapture videoCapture, FVideoCapture & videoCaptureOut, int32 CameraID, bool &isOpen)
{
	isOpen = videoCapture.cvVideoCapture.open(CameraID);
	videoCaptureOut = videoCapture;
}

void UOpenCVBPLibrary::IsOpened(FVideoCapture videoCapture, bool &isOpen)
{
	isOpen = videoCapture.cvVideoCapture.isOpened();
}

void UOpenCVBPLibrary::Capture(FMat mat, FMat & matOut, FVideoCapture videoCapture, bool &isCaptured)
{
	isCaptured = videoCapture.cvVideoCapture.read(mat.cvMat);
	matOut = mat;
}

void UOpenCVBPLibrary::MatInfo(FMat mat, int32 &colsOut, int32 &rowsOut, int32 &stepOut, int32 &totalOut, int32 &elemSizeOut, int32 &channelsOut)
{
	colsOut = mat.cvMat.cols;
	rowsOut = mat.cvMat.rows;
	stepOut = mat.cvMat.step;
	totalOut = mat.cvMat.total();
	elemSizeOut = mat.cvMat.elemSize();
	channelsOut = mat.cvMat.channels();
}

// we need a transient texture for the RenderToTexture function
void UOpenCVBPLibrary::CreateTransient(int32 width, int32 height, UTexture2D * Texture)
{
	Texture = UTexture2D::CreateTransient(width, height);
}

void UOpenCVBPLibrary::RenderToTexture(FMat mat, UTexture2D * Texture)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = 0;
		RegionData->NumRegions = mat.cvMat.channels();
		RegionData->Regions = new FUpdateTextureRegion2D(0, 0, 0, 0, mat.cvMat.size().width, mat.cvMat.size().height);
		RegionData->SrcPitch = (uint32)(mat.cvMat.elemSize() * mat.cvMat.size().width);
		RegionData->SrcBpp = (uint32)mat.cvMat.elemSize();
		RegionData->SrcData = (uint8*)mat.cvMat.data;

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
					}
				}
				delete RegionData;
			});
	}
}
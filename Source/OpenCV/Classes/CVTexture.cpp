// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVTexture.h"

UCVTexture::UCVTexture()
{
	// default texture
	myTexture2D = UTexture2D::CreateTransient(8, 8);
}

UCVTexture::~UCVTexture()
{
	if (myUpdateTextureRegion2D) delete myUpdateTextureRegion2D;
}

void UCVTexture::InitializeTexture(FVector2D TextureSize)
{
	myTexture2D = UTexture2D::CreateTransient(TextureSize.X, TextureSize.Y);

	if (myUpdateTextureRegion2D) delete myUpdateTextureRegion2D;
	myUpdateTextureRegion2D = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureSize.X, TextureSize.Y);

	Data.Init(FColor(0, 0, 0, 255), TextureSize.X * TextureSize.Y);
}

void UCVTexture::UpdateResource()
{
	if (myTexture2D)
		myTexture2D->UpdateResource();
}

void UCVTexture::DataFromCVMat(UCVMat * CVMatIn)
{
	if (CVMatIn)
	{
		for (int y = 0; y < CVMatIn->cvMat.rows; y++)
		{
			for (int x = 0; x < CVMatIn->cvMat.cols; x++)
			{
				int i = x + (y * CVMatIn->cvMat.cols);
				Data[i].B = CVMatIn->cvMat.data[i * 3 + 0];
				Data[i].G = CVMatIn->cvMat.data[i * 3 + 1];
				Data[i].R = CVMatIn->cvMat.data[i * 3 + 2];
			}
		}
	}
}

void UCVTexture::UpdateTexture()
{
	if (
		myTexture2D &&
		myUpdateTextureRegion2D &&
		((uint32)Data.Num() >= myUpdateTextureRegion2D->Width * myUpdateTextureRegion2D->Height)
		)
	{
		UpdateTextureRegions(myTexture2D, (int32)0, (uint32)3, myUpdateTextureRegion2D, (uint32)(4 * myUpdateTextureRegion2D->Width), (uint32)4, (uint8 *)Data.GetData(), false);
	}
}

UTexture2D * UCVTexture::GetTexture2D()
{
	return myTexture2D;
}

void UCVTexture::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
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
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
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
				if (bFreeData)
				{
					FMemory::Free(RegionData->Regions);
					FMemory::Free(RegionData->SrcData);
				}
				delete RegionData;
			});
	}
}
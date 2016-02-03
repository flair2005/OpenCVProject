// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenCVPrivatePCH.h"
#include "CVTexture.h"

UCVTexture::UCVTexture()
{
	// default texture
	Texture2D = UTexture2D::CreateTransient(8, 8);
}

UCVTexture::~UCVTexture()
{
	if (UpdateTextureRegion2D) delete UpdateTextureRegion2D;
}

void UCVTexture::InitializeTexture(FVector2D TextureSize)
{
	Texture2D = UTexture2D::CreateTransient(TextureSize.X, TextureSize.Y);
	Texture2D->UpdateResource();

	if (UpdateTextureRegion2D) delete UpdateTextureRegion2D;
	UpdateTextureRegion2D = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureSize.X, TextureSize.Y);

	Data.Init(FColor(0, 0, 0, 255), TextureSize.X * TextureSize.Y);
}

bool UCVTexture::UpdateResource()
{
	if (Texture2D)
	{
		Texture2D->UpdateResource();
		return true;
	}
	else
		return false;
}

bool UCVTexture::DataFromCVMat(UCVMat * CVMatIn)
{
	if (CVMatIn && CVMatIn->cvMat.data)
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
		return true;
	}
	else
		return false;
}

bool UCVTexture::UpdateTexture()
{
	if (
		Texture2D &&
		UpdateTextureRegion2D &&
		((uint32)Data.Num() >= UpdateTextureRegion2D->Width * UpdateTextureRegion2D->Height)
		)
	{
		UpdateTextureRegions(Texture2D, (int32)0, (uint32)3, UpdateTextureRegion2D, (uint32)(4 * UpdateTextureRegion2D->Width), (uint32)4, (uint8 *)Data.GetData(), false);
		return true;
	}
	else
		return false;
}

UTexture2D * UCVTexture::GetTexture2D()
{
	return Texture2D;
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
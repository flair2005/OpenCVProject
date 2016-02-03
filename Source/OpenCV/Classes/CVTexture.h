// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CVMat.h"
#include "opencv2/core.hpp"
#include "CVTexture.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class OPENCV_API UCVTexture : public UObject
{
	GENERATED_BODY()

public:
	UCVTexture();
	~UCVTexture();

	UFUNCTION(BlueprintCallable, Category = "CVTexture")
		void InitializeTexture(FVector2D TextureSize);
	
	UFUNCTION(BlueprintCallable, Category = "CVTexture")
		bool UpdateResource();

	UFUNCTION(BlueprintCallable, Category = "CVTexture")
		bool DataFromCVMat(UCVMat * CVMatIn);

	UFUNCTION(BlueprintCallable, Category = "CVTexture")
		bool UpdateTexture();
	
	void UCVTexture::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	UFUNCTION(BlueprintPure, Category = "CVTexture")
		UTexture2D * GetTexture2D();

	TArray<FColor> Data;
	UTexture2D * Texture2D;
	FUpdateTextureRegion2D * UpdateTextureRegion2D;
};

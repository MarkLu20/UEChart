// Fill out your copyright notice in the Description page of Project Settings.

#include "HotTexture.h"
#include "Engine/Texture2D.h"
#include "Engine/CurveTable.h"
#include "Math.h"

// Sets default values
AHotTexture::AHotTexture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHotTexture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHotTexture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
UTexture2D*	AHotTexture::CreateDynTexture(int32 width, int32 height)
{
	pTexture = UTexture2D::CreateTransient(width, height);
	pTexture->AddressX = TextureAddress::TA_Clamp;
	pTexture->AddressY = TextureAddress::TA_Clamp;
	pTexture->Filter = TextureFilter::TF_Bilinear;
	FTexture2DMipMap& Mip = pTexture->PlatformData->Mips[0];
	FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	for (int y = 0; y < height; ++y)
	{
		int start_idx = y * width;
		for (int x = 0; x < width; ++x)
		{
			int idx = start_idx + x;

			float value = 0;
			value = sin(x / float(width - 1) * 2 * PI) + 1;
			//value = FMath::Clamp<float>(value, 0, 1.0f);
			value *= 0.5 * 255;

			Data[idx].R = 0;
			Data[idx].B = 0;
			Data[idx].G = 255;
			Data[idx].A = 255;
		}
	}
	Mip.BulkData.Unlock();
	pTexture->UpdateResource();

	return pTexture;
}

void	AHotTexture::UpdateDyntexture(UTexture2D* pDynTexture, TArray<FVector4> values,float Limit,bool IsAdd)
{

	FTexture2DMipMap& Mip = pDynTexture->PlatformData->Mips[0];
	FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);

	for (size_t i = 0; i < values.Num(); i++)
	{
		FVector4 point = values[i];
		for (int y = 0; y < pDynTexture->GetSizeY(); ++y)
		{
			int start_idx = y * pDynTexture->GetSizeX();
			for (int x = 0; x < pDynTexture->GetSizeX(); ++x)
			{
				int idx = start_idx + x;

				float disance = FMath::Pow(FMath::Square(x - point.X) + FMath::Square(y - point.Y), 0.5f);

				FPixelData nowData = Data[idx];
				nowData.B = 0;
				nowData.G = 0;
				if (disance < point.W)
				{
					float power = point.Z * ((point.W - disance) / point.W);

					float alpha = 255 * power / Limit;
					if (alpha > 255) alpha = 255;
					if (IsAdd)
					{
						if (nowData.R + alpha >= 255)nowData.R = 255;
						else nowData.R += alpha;
					}
					else if (Data[idx].R < alpha)
					{
						nowData.R = alpha;
					}
				}
				Data[idx] = nowData;
			}
		}
	}
	Mip.BulkData.Unlock();
	pDynTexture->UpdateResource();
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HotTexture.generated.h"

UCLASS()
class MK_ECHARTS_API AHotTexture : public AActor
{
	GENERATED_BODY()
	struct FPixelData
	{
		UINT8 B;
		UINT8 G;
		UINT8 R;
		UINT8 A;
	};
	
public:	
	// Sets default values for this actor's properties
	AHotTexture();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blueprints)
		class UTexture2D* pTexture;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = Blueprints)
		UTexture2D*		CreateDynTexture(int32 x, int32 y);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void			UpdateDyntexture(UTexture2D* pDynTexture, TArray<FVector4> values, float Limit, bool IsAdd);

};

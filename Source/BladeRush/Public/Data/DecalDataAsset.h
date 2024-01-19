// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DecalDataAsset.generated.h"

/**
 * Decal data asset. Contains TMap<EPhysicalSurface,UMaterialInstance*> & default decal material
 */
UCLASS()
class BLADERUSH_API UDecalDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetMaterialFromPhysicalSurface(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetDefaultDecalMaterial() const;
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<EPhysicalSurface>,UMaterialInstance*> DecalData;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	UMaterialInstance* DefaultDecalMaterial;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DecalDataAsset.h"

UMaterialInstance* UDecalDataAsset::GetMaterialFromPhysicalSurface(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	if (DecalData.Contains(SurfaceType))
	{
		return DecalData[SurfaceType];
	}

	return nullptr;
}

UMaterialInstance* UDecalDataAsset::GetDefaultDecalMaterial() const
{
	return DefaultDecalMaterial;
}

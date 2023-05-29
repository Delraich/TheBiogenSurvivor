// Fill out your copyright notice in the Description page of Project Settings.


#include "/UE/Projects/TheBiogenSurvivor 5.1/Source/TheBiogenSurvivor/Public/WorldGenerators/TBSWorldGenerator.h"
#include "FastNoise/VoxelFastNoise.inl"
#include "VoxelMaterialBuilder.h"

TVoxelSharedRef<FVoxelGeneratorInstance> UTBSWorldGenerator::GetInstance()
{
	return MakeVoxelShared<FTBSWorldGeneratorInstance>(*this);
}

///////////////////////////////////////////////////////////////////////////////

FTBSWorldGeneratorInstance::FTBSWorldGeneratorInstance(const UTBSWorldGenerator& MyGenerator)
	: Super(&MyGenerator)
	, BaseHeight(MyGenerator.BaseHeight)
	, BaseFrequency(MyGenerator.BaseFrequency)
	, MountainHeight(MyGenerator.BaseHeight)
	, MountainFrequency(MyGenerator.BaseFrequency)
	, MountainPeakHeight(MyGenerator.BaseHeight)
	, MountainPeakFrequency(MyGenerator.BaseFrequency)
	, TemperatureNoise(MyGenerator.TemperatureNoise)
	, MoistureNoise(MyGenerator.MoistureNoise)
	, TemperatureFrequency(MyGenerator.TemperatureFrequency)
	, MoistureFrequency(MyGenerator.MoistureFrequency)
	, Seed(MyGenerator.Seed)
{
}

void FTBSWorldGeneratorInstance::Init(const FVoxelGeneratorInit& InitStruct)
{
	Noise.SetSeed(Seed);
}

v_flt FTBSWorldGeneratorInstance::GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	const float Height = Noise.GetPerlin_2D(X, Y, BaseFrequency) * BaseHeight;
	const float Temperature = Noise.GetPerlin_2D(X, Y, TemperatureFrequency);
	const float Moisture = Noise.GetPerlin_2D(X, Y, MoistureFrequency);
	const float Height2 = Noise.GetPerlin_2D(X, Y, MountainFrequency) * MountainHeight;
	// Positive value -> empty voxel
	// Negative value -> full voxel
	// Value positive when Z > Height, and negative Z < Height
	float Value = Z - Height;

	if (Temperature < -0.5f) {

	}
	else if (Temperature <= 0.5f)
	{
		if (Moisture < -0.75f) {
			Value = Z - Height2;
		}
		else if (Moisture < -0.5f) {
			Value = Z - Height2;
		}
	}
	else {

	}

	// The voxel value is clamped between -1 and 1. That can result in a bad gradient/normal. To solve that we divide it
	Value /= 5;

	return Value;
}

FVoxelMaterial FTBSWorldGeneratorInstance::GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	const float Temperature = Noise.GetPerlin_2D(X, Y, TemperatureFrequency);
	const float Moisture = Noise.GetPerlin_2D(X, Y, MoistureFrequency);

	// RGB
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::RGB);
	//Builder.SetColor(FColor::Red);

	// Single index
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::SingleIndex);
	//Builder.SetSingleIndex(0); 

	// Multi index
	FVoxelMaterialBuilder Builder;
	Builder.SetMaterialConfig(EVoxelMaterialConfig::MultiIndex);

	float DryWeight = FMath::Clamp(100 - (100 * FMath::Abs((DryMid)-Moisture) / (DryMid)-(DryMin)), 0, 100) / 100.f;
	float SemiDryWeight = FMath::Clamp(100 - (100 * FMath::Abs((SemiDryMid)-Moisture) / (SemiDryMid)-(SemiDryMin)), 0, 100) / 100.f;
	float SemiWetWeight = FMath::Clamp(100 - (100 * FMath::Abs((SemiWetMid)-Moisture) / (SemiWetMid)-(SemiWetMin)), 0, 100) / 100.f;
	float WetWeight = FMath::Clamp(100 - (100 * FMath::Abs((WetMid)-Moisture) / (WetMid)-(WetMin)), 0, 100) / 100.f;

	if (Temperature <= -0.5f)
	{
		if (Moisture < -0.75f) {
			Builder.AddMultiIndex(6, 1.0f);
		}
		else if (Moisture < -0.5f) {
			Builder.AddMultiIndex(6, 0.7f);
			Builder.AddMultiIndex(9, 0.3f);
		}
		else if (Moisture < -0.25f) {
			Builder.AddMultiIndex(9, 1.0f);
		}
		else if (Moisture < 0.0f) {
			Builder.AddMultiIndex(9, 0.7f);
			Builder.AddMultiIndex(7, 0.3f);
		}
		else if (Moisture < 0.25f) {
			Builder.AddMultiIndex(7, 1.0f);
		}
		else if (Moisture < 0.5f) {
			Builder.AddMultiIndex(7, 0.7f);
			Builder.AddMultiIndex(8, 0.3f);
		}
		else if (Moisture < 0.75f) {
			Builder.AddMultiIndex(8, 1.0f);
		}
		else {
			Builder.AddMultiIndex(8, 1.0f);
		}
	}
	else if (Temperature <= 0.5f)
	{
		if (Moisture < -0.75f) {
			Builder.AddMultiIndex(0, 1.0f);
		}
		else if (Moisture < -0.5f) {
			Builder.AddMultiIndex(0, 0.7f);
			Builder.AddMultiIndex(3, 0.3f);
		}
		else if (Moisture < -0.25f) {
			Builder.AddMultiIndex(3, 1.0f);
		}
		else if (Moisture < 0.0f) {
			Builder.AddMultiIndex(3, 0.7f);
			Builder.AddMultiIndex(1, 0.3f);
		}
		else if (Moisture < 0.25f) {
			Builder.AddMultiIndex(1, 1.0f);
		}
		else if (Moisture < 0.5f) {
			Builder.AddMultiIndex(1, 0.7f);
			Builder.AddMultiIndex(12, 0.3f);
		}
		else if (Moisture < 0.75f) {
			Builder.AddMultiIndex(12, 1.0f);
		}
		else {
			Builder.AddMultiIndex(12, 1.0f);
		}
	}
	else
	{
		if (Moisture < -0.75f) {
			Builder.AddMultiIndex(5, 1.0f);
		}
		else if (Moisture < -0.5f) {
			Builder.AddMultiIndex(5, 0.7f);
			Builder.AddMultiIndex(13, 0.3f);
		}
		else if (Moisture < -0.25f) {
			Builder.AddMultiIndex(13, 1.0f);
		}
		else if (Moisture < 0.0f) {
			Builder.AddMultiIndex(13, 0.7f);
			Builder.AddMultiIndex(14, 0.3f);
		}
		else if (Moisture < 0.25f) {
			Builder.AddMultiIndex(14, 1.0f);
		}
		else if (Moisture < 0.5f) {
			Builder.AddMultiIndex(14, 0.7f);
			Builder.AddMultiIndex(11, 0.3f);
		}
		else if (Moisture < 0.75f) {
			Builder.AddMultiIndex(11, 1.0f);
		}
		else {
			Builder.AddMultiIndex(11, 1.0f);
		}

	}

	/*if (Z >= 150) {
		Builder.AddMultiIndex(6, 1.0f);
	}
	else if (Z >= 75) {
		Builder.AddMultiIndex(1, 1.0f);
	}
	else if (Z >= 20) {
		Builder.AddMultiIndex(3, 1.0f);
	}
	else  {
		Builder.AddMultiIndex(11, 1.0f);
	}*/
	return Builder.Build();
}

TVoxelRange<v_flt> FTBSWorldGeneratorInstance::GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const
{
	// Return the values that GetValueImpl can return in Bounds
	// Used to skip chunks where the value does not change
	// Be careful, if wrong your world will have holes!
	// By default return infinite range to be safe
	return TVoxelRange<v_flt>::Infinite();

	// Example for the GetValueImpl above

	// Noise is between -1 and 1
	const TVoxelRange<v_flt> Height = TVoxelRange<v_flt>(-1, 1) * BaseHeight;

	// Z can go from min to max
	TVoxelRange<v_flt> Value = TVoxelRange<v_flt>(Bounds.Min.Z, Bounds.Max.Z) - Height;


	Value /= 5;

	return Value;
}

FVector FTBSWorldGeneratorInstance::GetUpVector(v_flt X, v_flt Y, v_flt Z) const
{
	// Used by spawners
	return FVector::UpVector;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastNoise/VoxelFastNoise.h"
#include "VoxelGenerators/VoxelGeneratorHelpers.h"
#include "TBSWorldGenerator.generated.h"


UCLASS(Blueprintable)
class UTBSWorldGenerator : public UVoxelGenerator
{
	GENERATED_BODY()

public:

	/*General Terrain*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		int32 Seed = 1337;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float BaseHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
		float BaseFrequency = 0.001f;

	/*Biome Settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biomes")
		float TemperatureNoise = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biomes")
		float TemperatureFrequency = 0.0001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biomes")
		float MoistureNoise = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biomes")
		float MoistureFrequency = 0.0002f;


	/*Desert*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desert", meta = (DisplayName = "Dune Direction X"))
		float Dune_Direction_X = 0.4;
	// The direction of the noise. Will be normalized
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desert", meta = (DisplayName = "Dune Direction Y"))
		float Dune_Direction_Y = 1.0;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desert", meta = (DisplayName = "Dune Frequency"))
		float Dune_Frequency = 0.002;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desert", meta = (DisplayName = "Dune Height"))
		float Height = 75.0;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desert", meta = (DisplayName = "Noise Frequency"))
		float Dune_Noise_Frequency = 0.001;

	/*Mountains*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountain")
		float MountainHeight = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountain")
		float MountainFrequency = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountain")
		float MountainPeakHeight = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountain")
		float MountainPeakFrequency = 0.01f;

	

	//~ Begin UVoxelGenerator Interface
	virtual TVoxelSharedRef<FVoxelGeneratorInstance> GetInstance() override;
	//~ End UVoxelGenerator Interface
};

class FTBSWorldGeneratorInstance : public TVoxelGeneratorInstanceHelper<FTBSWorldGeneratorInstance, UTBSWorldGenerator>
{
public:
	using Super = TVoxelGeneratorInstanceHelper<FTBSWorldGeneratorInstance, UTBSWorldGenerator>;

	explicit FTBSWorldGeneratorInstance(const UTBSWorldGenerator& MyGenerator);

	//~ Begin FVoxelGeneratorInstance Interface
	virtual void Init(const FVoxelGeneratorInit& InitStruct) override;

	v_flt GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;
	FVoxelMaterial GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const;

	TVoxelRange<v_flt> GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const;

	virtual FVector GetUpVector(v_flt X, v_flt Y, v_flt Z) const override final;
	//~ End FVoxelGeneratorInstance Interface


private:
	// General Terrain
	const float BaseHeight;
	const float BaseFrequency;
	// Mountains
	const float MountainHeight;
	const float MountainFrequency;
	const float MountainPeakHeight;
	const float MountainPeakFrequency;
	//
	const float TemperatureNoise;
	const float MoistureNoise;
	const float TemperatureFrequency;
	const float MoistureFrequency;
	const int32 Seed;
	FVoxelFastNoise Noise;

	const float DryMid = -1.0f;
	const float DryMin = -1.67f;
	const float SemiDryMid = -0.33f;
	const float SemiDryMin = -1.0f;
	const float SemiWetMid = 0.33f;
	const float SemiWetMin = -0.33f;
	const float WetMid = 1.0f;
	const float WetMin = 0.33f;

};

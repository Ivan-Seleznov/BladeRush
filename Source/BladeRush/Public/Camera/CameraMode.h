
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CameraMode.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FCameraTransitionFinished, bool,bEnteredCameraMode);

class USpringArmComponent;
/**
 * Camera mode class
 */
UCLASS(BlueprintType,Blueprintable,Abstract)
class BLADERUSH_API UCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool IsCameraInTransition() const {return bTransitionCamera;}
	UFUNCTION(BlueprintCallable)
	const float& GetInterpSpeed() const {return InterpSpeed;}

	UFUNCTION(BlueprintCallable)
	bool IsEnteredCameraMode() const {return bShouldEnter;}
	
	virtual void SetCameraManager(APlayerCameraManager* CameraManager);
	virtual void UpdateView(float DeltaTime,USpringArmComponent* SpringArm);

	UFUNCTION(BlueprintCallable)
	void EnterCameraMode();
	UFUNCTION(BlueprintCallable)
	void ExitCameraMode();

	UPROPERTY(BlueprintReadWrite)
	FCameraTransitionFinished OnCameraTransitionFinished;
	
protected:
	virtual bool UpdateCameraTransition(float DeltaTime,USpringArmComponent* SpringArm);

	UPROPERTY()
	APlayerCameraManager* PlayerCameraManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InterpSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bTransitionCamera;
	
	UPROPERTY(BlueprintReadOnly)
	bool bShouldEnter;
};

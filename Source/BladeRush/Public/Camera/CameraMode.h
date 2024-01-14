
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CameraMode.generated.h"

DECLARE_DELEGATE_OneParam(FCameraTransitionFinished,/**/ bool) 

class USpringArmComponent;
/**
 * Camera mode class
 */
UCLASS(BlueprintType,Blueprintable,Abstract)
class BLADERUSH_API UCameraMode : public UObject
{
	GENERATED_BODY()

public:
	bool IsCameraInTransition() const {return bTransitionCamera;}
	const float& GetInterpSpeed() const {return InterpSpeed;}

	virtual void SetCameraManager(APlayerCameraManager* CameraManager);
	virtual void UpdateView(float DeltaTime,USpringArmComponent* SpringArm);

	void EnterCameraMode();
	void ExitCameraMode();
	
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

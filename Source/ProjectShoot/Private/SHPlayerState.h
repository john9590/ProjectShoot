#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SHPlayerState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTSHOOT_API ASHPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ASHPlayerState();

    UPROPERTY(ReplicatedUsing = OnRep_HP, VisibleAnywhere, BlueprintReadWrite, Category = "HP")
    float HP;

    UFUNCTION()
    void OnRep_HP();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MaxHP")
    float MaxHP;

    void UpdateHealthVisuals();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 클라이언트에서 서버로 데미지 요청을 보내는 함수
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_damaged(float DamageAmount);

    void Server_damaged_Implementation(float DamageAmount);
    bool Server_damaged_Validate(float DamageAmount);

    void ApplyDamage(float DamageAmount); // 실제 데미지 적용 로직
};

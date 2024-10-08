#include "SHPlayerState.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

ASHPlayerState::ASHPlayerState()
{
    HP = 100.f;
    MaxHP = 100.f;
}

void ASHPlayerState::OnRep_HP()
{
    // HP 값이 변경되었을 때 클라이언트에서 실행할 코드

    // 예: HP가 변경될 때 UI를 업데이트하는 함수 호출

    // 또는 HP에 따른 시각적 효과 적용
    UpdateHealthVisuals();
}


void ASHPlayerState::UpdateHealthVisuals()
{
    // HP에 따라 캐릭터의 시각적 효과를 업데이트하는 코드
    // 예: HP가 낮아질수록 화면에 붉은 색이 더 나타나도록
    if (ACharacter* Character = Cast<ACharacter>(GetPawn()))
    {
        UMaterialInstanceDynamic* DynamicMaterial = Character->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
        if (DynamicMaterial)
        {
            float HealthPercentage = HP / MaxHP;
            DynamicMaterial->SetScalarParameterValue("HealthPercentage", HealthPercentage);
        }
    }
}
void ASHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASHPlayerState, HP);
    DOREPLIFETIME(ASHPlayerState, MaxHP);
}

void ASHPlayerState::Server_damaged_Implementation(float DamageAmount)
{
    ApplyDamage(DamageAmount); // 서버에서 실제 데미지를 적용
}

bool ASHPlayerState::Server_damaged_Validate(float DamageAmount)
{
    // 데미지 요청이 유효한지 검증하는 로직을 추가할 수 있습니다
    return true;
}

void ASHPlayerState::ApplyDamage(float DamageAmount)
{
    HP -= DamageAmount;
    HP = FMath::Clamp(HP, 0.f, MaxHP); // HP 값이 0 이하로 내려가지 않도록 클램프합니다
    OnRep_HP(); // HP 업데이트를 클라이언트에 알림
}

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
    // HP ���� ����Ǿ��� �� Ŭ���̾�Ʈ���� ������ �ڵ�

    // ��: HP�� ����� �� UI�� ������Ʈ�ϴ� �Լ� ȣ��

    // �Ǵ� HP�� ���� �ð��� ȿ�� ����
    UpdateHealthVisuals();
}


void ASHPlayerState::UpdateHealthVisuals()
{
    // HP�� ���� ĳ������ �ð��� ȿ���� ������Ʈ�ϴ� �ڵ�
    // ��: HP�� ���������� ȭ�鿡 ���� ���� �� ��Ÿ������
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
    ApplyDamage(DamageAmount); // �������� ���� �������� ����
}

bool ASHPlayerState::Server_damaged_Validate(float DamageAmount)
{
    // ������ ��û�� ��ȿ���� �����ϴ� ������ �߰��� �� �ֽ��ϴ�
    return true;
}

void ASHPlayerState::ApplyDamage(float DamageAmount)
{
    HP -= DamageAmount;
    HP = FMath::Clamp(HP, 0.f, MaxHP); // HP ���� 0 ���Ϸ� �������� �ʵ��� Ŭ�����մϴ�
    OnRep_HP(); // HP ������Ʈ�� Ŭ���̾�Ʈ�� �˸�
}

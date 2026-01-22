#include "SeventhPawnFuction.h"
// 반드시 필요한 헤더들
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h" 
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <SeventhPlayerController.h>


ASeventhPawnFuction::ASeventhPawnFuction()
{
    PrimaryActorTick.bCanEverTick = true;



    /* 1. CapsuleComponent, SpringArmComponent, CameraComponent 추가
    충돌 컴포넌트를 루프 컴포넌트로 설정, 나머지 컴포넌트 부착*/
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    RootComponent = CapsuleComp;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bInheritPitch = true;
	SpringArmComp->bInheritYaw = true;
	SpringArmComp->bInheritRoll = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.2f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    

    if (CapsuleComp)
    {
        CapsuleComp->SetSimulatePhysics(false);
    }

    // 2. 메쉬 컴포넌트 물리 끄기
    if (MeshComp)
    {
        MeshComp->SetSimulatePhysics(false);
    }
}

void ASeventhPawnFuction::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetNameSafe(Controller));
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}


void ASeventhPawnFuction::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 1. MoveAction이 Null인지 반드시 로그로 확인해보세요.


        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
        {
            if (ASeventhPlayerController* PlayerController = Cast<ASeventhPlayerController>(GetController()))
            {
                if (PlayerController->MoveAction)
                {
                    EnhancedInput->BindAction(
                        PlayerController->MoveAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ASeventhPawnFuction::Move
                    );
                }
                if (PlayerController->LookAction)
                {
                    EnhancedInput->BindAction(
                        PlayerController->LookAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ASeventhPawnFuction::Look
                    );
                }

            }

        }

    }
    
}
void ASeventhPawnFuction::Move(const FInputActionValue& Value)
{
    if (!Controller)return;

    const FVector2D MoveInput = Value.Get<FVector2D>();    

    AddActorLocalOffset(GetActorForwardVector() * MoveInput.X * MoveSpeed, true);
    AddActorLocalOffset(GetActorRightVector() * MoveInput.Y * MoveSpeed, true);
}
//void ASeventhPawnFuction::Look(const FInputActionValue& Value)
//{
//    FVector2D LookInput = Value.Get<FVector2D>();
//
//    if (Controller != nullptr)
//    {
//        // 1. 감도를 충분히 높여줍니다. (직접 회전은 큰 값이 필요할 수 있음)
//        float Sensitivity = 45.0f;
//        float DeltaTime = GetWorld()->GetDeltaSeconds();
//
//        // 2. 입력값에 시간을 곱해 프레임 독립적인 회전량을 계산합니다.
//        float YawRotation = LookInput.X * Sensitivity * DeltaTime;
//        float PitchRotation = -LookInput.Y * Sensitivity * DeltaTime; // 위아래 반전 방지
//
//        // 3. FRotator 생성 (Pitch, Yaw, Roll)
//        FRotator DeltaRot = FRotator(PitchRotation, YawRotation, 0.f);
//
//        // 4. 로컬 회전 적용
//        // bSweep을 false로, bTeleport를 true로 하면 물리 충돌 무시하고 강제 회전합니다.
//        AddActorLocalRotation(DeltaRot, false, nullptr, ETeleportType::TeleportPhysics);
//    }
//}

void ASeventhPawnFuction::Look(const FInputActionValue& Value)
{
    
    FVector2D LookValue = Value.Get<FVector2D>();

    float Yaw = LookValue.X * RotationSpeed;
    float Pitch = -LookValue.Y * RotationSpeed;

    AddActorLocalRotation(FRotator(Pitch, Yaw, 0.0f));
    
    FRotator NewYaw = SpringArmComp->GetRelativeRotation();
    NewYaw.Yaw = FMath::Clamp(NewYaw.Yaw + Yaw, -80.f, 80.f);
    
    FRotator NewPitch = SpringArmComp->GetRelativeRotation();
    NewPitch.Pitch = FMath::Clamp(NewPitch.Pitch + Pitch, -80.f, 80.f);

    SpringArmComp->SetRelativeRotation(NewYaw);
    SpringArmComp->SetRelativeRotation(NewPitch);
   
}
//void ASeventhPawnFuction::Look(const FInputActionValue& Value)
//{
//    FVector2D LookInput = Value.Get<FVector2D>();
//
//    AddControllerYawInput(LookInput.X);
//    AddControllerPitchInput(LookInput.Y);
//
//}

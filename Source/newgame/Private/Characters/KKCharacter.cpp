 // Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KKCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ComponentAttributes.h"
#include "items/Items.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"
#include "items/Soul.h"
#include "items/Treasure/Treasure.h"
#include "HUD/MainHUD.h"
#include "HUD/Charoverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemies/Enemy.h"

AKKCharacter::AKKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBone = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBone->SetupAttachment(GetRootComponent());
	CameraBone->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBone);

}

void AKKCharacter::Tick(float DeltaTime) {
	if (Attributes) {
		Attributes->RegenStamina(DeltaTime);
			GetOverlay->SetStaminabarPercent(Attributes->GetStaminapercent());
		if (Actionstate == EActionstate::EAS_Unoccupied) {
			Attributes->RegenHealth(DeltaTime);
			GetOverlay->SetHealthbarPercent(Attributes->GetHealthpercent());
		}
	}

	if (bIsLockedOn && LockonActor && Controller)
	{
		FVector ToEnemy = LockonActor->GetActorLocation() - GetActorLocation();
		ToEnemy.Z = 0.f;

		FRotator DesiredRotation = ToEnemy.Rotation();
		FRotator CurrentRotation = Controller->GetControlRotation();

		// Smoothly rotate controller to enemy
		FRotator Smoothed = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, 6.0f);
		Controller->SetControlRotation(Smoothed);
	}

}

void AKKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Moveforward"), this, &AKKCharacter::Moveforward);
	PlayerInputComponent->BindAxis(FName("Moveside"), this, &AKKCharacter::Moveside);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AKKCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("Lookup"), this, &AKKCharacter::Lookup);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AKKCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AKKCharacter::EKeypress);
	PlayerInputComponent->BindAction(FName("Detach"), IE_Pressed, this, &AKKCharacter::QKeypress);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AKKCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Item1"), IE_Pressed, this, &AKKCharacter::Item1);
	PlayerInputComponent->BindAction(FName("Item2"), IE_Pressed, this, &AKKCharacter::Item2);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AKKCharacter::Dodge);
	PlayerInputComponent->BindAction(FName("Lock"), IE_Pressed, this, &AKKCharacter::Targetlock);
	//PlayerInputComponent->BindAction(FName("Abillity"), IE_Pressed, this, &AKKCharacter::Abillity);

}

void AKKCharacter::Jump() {

	if (IsUnoccupied()) {
		Super::Jump();
	}
}

float AKKCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {

	Handledamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AKKCharacter::GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) {

	Super::GetHit_Implementation(Impactpoint, Hitter );
	SetweaponcollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthpercent() > 0.f) {
		Actionstate = EActionstate::EAS_Hitreaction;
	}
}

void AKKCharacter::SetOverlappingItem(AItems* Item) {

	OverLappingItem = Item;
}

void AKKCharacter::AddSouls(ASoul* Soul) {
	if (Attributes && GetOverlay) {
		Attributes->AddSouls(Soul->Getsouls());
		GetOverlay->SetSoul(Attributes->GetSoul());
	}
}

void AKKCharacter::AddGolds(ATreasure* Treasure) {
	if (Attributes) {
		Attributes->AddGolds(Treasure->GetGolds());
		GetOverlay->SetGold(Attributes->GetGold());
	}
}

void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableCharacter"));

	ImplementOverlay();

}

void AKKCharacter::Moveforward(float Value)
{
	if (Actionstate == EActionstate::EAS_Attacking) return;
	if (Itemtakingstate == EItemtaking::EIT_Itemtaking) return;
	if ((Controller != nullptr) && (Value != 0.f))
	{
		//to find which way is fwd
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AKKCharacter::Moveside(float Value)
{
	if (Actionstate == EActionstate::EAS_Attacking) return;
	if (Itemtakingstate == EItemtaking::EIT_Itemtaking) return;
	if (Controller && (Value != 0.f))
	{
		//to find which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AKKCharacter::Turn(float Value)
{

	if (!bIsLockedOn) AddControllerYawInput(Value);
}

void AKKCharacter::Lookup(float Value)
{
	if (!bIsLockedOn) AddControllerPitchInput(Value);
}

void AKKCharacter::EKeypress()
{
	if (Equippedweapon) return;
	AWeapon* Overlappingweapon = Cast<AWeapon>(OverLappingItem);
	if (Overlappingweapon && Charstate == ECharstate::ECS_Unequipped) {
		Overlappingweapon->Equip(GetMesh(), FName("Righthandsocket"), this, this);
		Charstate = ECharstate::ECS_Equipped;
		Equippedweapon = Overlappingweapon;
	}
}

void AKKCharacter::QKeypress()
{
	if (Actionstate == EActionstate::EAS_Attacking) return;
	if (Actionstate == EActionstate::EAS_Hitreaction) return;
	if (!Equippedweapon) return;
	if (Equippedweapon && Charstate == ECharstate::ECS_Equipped) {
		Equippedweapon->DetachItem();
		Charstate = ECharstate::ECS_Unequipped;
		Equippedweapon = nullptr;
	}
}

void AKKCharacter::Item1()
{
	if (CcanDisarm()) {
		PlayEquipmontage(FName("Unequip"));
		Charstate = ECharstate::ECS_Unequipped;
		Itemtakingstate = EItemtaking::EIT_Itemtaking;
	}
}

void AKKCharacter::Item2()
{
	if (Ccanarm()) {
		PlayEquipmontage(FName("Equip"));
		Charstate = ECharstate::ECS_Equipped;
		Itemtakingstate = EItemtaking::EIT_Itemtaking;
	}
}

void AKKCharacter::Dodge()
{
	bIsLockedOn = false;
	if (IsOccupied() || !HasEnoughStamina()) return;
	if (Actionstate == EActionstate::EAS_Dodge) return;
	PlayDodgeMontage();
	Actionstate = EActionstate::EAS_Dodge;
	if (Attributes && GetOverlay) {
		Attributes->UseStamina(Attributes->GetDodgecost());
		GetOverlay->SetStaminabarPercent(Attributes->GetStaminapercent());
	}
}

void AKKCharacter::Targetlock()
{
	if (bIsLockedOn)
	{
		// Unlock
		bIsLockedOn = false;
		LockonActor = nullptr;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (CameraBone)
		{
			CameraBone->SocketOffset = FVector::ZeroVector;
		}

		return;
	}

	// Trace forward from player to find enemy
	FVector Start = GetActorLocation();
	FVector Forward = GetControlRotation().Vector();
	FVector End = Start + Forward * LockRange;

	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECC_Pawn); // Enemy must be Pawn collision

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ObjectQuery,
		FCollisionShape::MakeSphere(LockRadius),
		Params
	);

	if (bHit && Hit.GetActor())
	{
		LockonActor = Hit.GetActor();
		bIsLockedOn = true;

		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		CameraBone->SocketOffset = LockonSocketOffset;
	}
}

void AKKCharacter::Abillity()
{

	if (Attributes && GetOverlay) {
		Attributes->UseStamina(Attributes->GetAbillitycost());
		GetOverlay->SetStaminabarPercent(Attributes->GetStaminapercent());
	}

	// bIsAbilityActive = true;
	/*
	PlaymontageSection(Abilitymontage, FName("Ability"));
	*/

	GetWorld()->GetTimerManager().SetTimer(
		AbilityDelayHandle,
		this,
		&AKKCharacter::AbilityDamage,
		3.75f,
		false
	);
}

void AKKCharacter::AbilityDamage()
{
	AActor* Enemy = Cast<AActor>(UGameplayStatics::GetActorOfClass(GetWorld(), EnemyClass));
	if (Enemy)
	{
		UGameplayStatics::ApplyDamage(Enemy, 60.0f, GetController(), this, nullptr);
	}
}

void AKKCharacter::Attack()
{
	Super::Attack();
	if (CcanAttack()) {
		PlayAttackmontage();
		Actionstate = EActionstate::EAS_Attacking;
	}
}

void AKKCharacter::Attackend()
{
	Actionstate = EActionstate::EAS_Unoccupied;
}

void AKKCharacter::Dodgeend() {
	Super::Dodgeend();
	Actionstate = EActionstate::EAS_Unoccupied;
}

bool AKKCharacter::CcanAttack()
{
	return Actionstate == EActionstate::EAS_Unoccupied && 
		Charstate != ECharstate::ECS_Unequipped;
}

bool AKKCharacter::CcanDisarm()
{
	return Actionstate == EActionstate::EAS_Unoccupied && 
		   Charstate != ECharstate::ECS_Unequipped;
}

bool AKKCharacter::Ccanarm()
{
	return Actionstate == EActionstate::EAS_Unoccupied && 
		Charstate == ECharstate::ECS_Unequipped &&
		Equippedweapon;
}

void AKKCharacter::PlayEquipmontage(const FName Sectionname)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();
	if (Animinstance && EquipMontage) {
		Animinstance->Montage_Play(EquipMontage);
		Animinstance->Montage_JumpToSection(Sectionname, EquipMontage);
	}
}

void  AKKCharacter::Die() {
	Super::Die();

	Actionstate = EActionstate::EAS_Dead;
	DisablemeshCollision();

	HandleDeath();
}

void AKKCharacter::HandleDeath()
{
	DisableInput(nullptr);

	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AKKCharacter::RestartGame, 6.0f, false);
}

void AKKCharacter::RestartGame() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

bool AKKCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgecost();
}

bool AKKCharacter::HasEnoughAbilityStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetAbillitycost();
}

bool AKKCharacter::IsOccupied()
{
	return Actionstate != EActionstate::EAS_Unoccupied;
}

void AKKCharacter::Itemtakingend()
{
	Itemtakingstate = EItemtaking::EIT_Nottaking;
}

void AKKCharacter::Disarm()
{
	if (Equippedweapon) {
		Equippedweapon->Attachmeshtosocket(GetMesh(), FName("Waistswordsocket"));
	}
}

void AKKCharacter::Arm()
{
	if (Equippedweapon) {
		Equippedweapon->Attachmeshtosocket(GetMesh(), FName("Righthandsocket"));
	}
}

void AKKCharacter::HitReactend()
{
	Actionstate = EActionstate::EAS_Unoccupied;
}

void AKKCharacter::DodgeZend()
{
	if (LockonActor) {
		bIsLockedOn = true;
	}
}

/*
void AKKCharacter::SpawnlightningStrike()
{

	AActor* EnemyActor = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyClass);

	if (!EnemyActor || !AOEEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy or AOEEffectClass not valid."));
		return;
	}

	FVector SpawnLocation = EnemyActor->GetActorLocation() + FVector(0, 0, -100);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(AOEEffectClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("AOE Effect spawned successfully."));
	}

}
*/
/*
void AKKCharacter::AbilityActiveEnd()
{
	bIsAbilityActive = false;
}
*/

bool AKKCharacter::IsUnoccupied()
{
	return Actionstate == EActionstate::EAS_Unoccupied;
}

void AKKCharacter::ImplementOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD) {
			GetOverlay = MainHUD->GetGreyoverlay();
			if (GetOverlay && Attributes) {
				GetOverlay->SetHealthbarPercent(Attributes->GetHealthpercent());
				GetOverlay->SetStaminabarPercent(1.f);
				GetOverlay->SetGold(0);
				GetOverlay->SetSoul(0);
			}
		}
	}
}

void AKKCharacter::SetHUDHealth()
{
	if (GetOverlay && Attributes) {
		GetOverlay->SetHealthbarPercent(Attributes->GetHealthpercent());
	}
}
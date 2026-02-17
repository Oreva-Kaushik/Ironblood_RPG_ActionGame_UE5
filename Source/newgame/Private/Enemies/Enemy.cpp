// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/ComponentAttributes.h"
#include "Components/WidgetComponent.h"
#include "Items/Weapons/Weapon.h"
#include "HUD/WCHealthbar.h"
#include "items/Soul.h"
#include "Patrolpoints/FakePatrolpawn.h"
#include "Components/SphereComponent.h"

AEnemy::AEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	WidgetHealthbar = CreateDefaultSubobject<UWCHealthbar>(TEXT("Healthbar"));
	WidgetHealthbar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensingCp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingCP"));
	PawnSensingCp->SightRadius = 2500.f;
	PawnSensingCp->SetPeripheralVisionAngle(42.5f);

	if (bIsBossEnemy) {
		OuterDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterDetectionSphere"));
		OuterDetectionSphere->SetupAttachment(RootComponent);
		OuterDetectionSphere->SetSphereRadius(1200.f);
		OuterDetectionSphere->SetCollisionProfileName(TEXT("Trigger"));

		/* Boss Enemy Components */
		DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
		DetectionSphere->SetupAttachment(RootComponent);
		DetectionSphere->SetSphereRadius(800.f); // Customize as needed
		DetectionSphere->SetCollisionProfileName(TEXT("Trigger"));
	}

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (!bIsBossEnemy) {
		if (Enemystate > EEnemystate::EES_Patrolling) {
			CheckcombatTarget();
		}
		else {
			CheckpatrolTarget();
		}
	}

	if (bIsBossEnemy && bHasWokenUp)
	{
		if (Enemystate > EEnemystate::EES_Patrolling)
		{
			CheckcombatTarget();
		}
	}

}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Handledamage(DamageAmount);
	Combattarget = EventInstigator->GetPawn();
	if (InsideAttackradius()) {
		Enemystate = EEnemystate::EES_Attacking;
	}
	else if(IsOutsideAttackRadius()){
		ChasingTarget();
	}

	if ( Attributes &&  Attributes->GetHealthpercent() <= 0.f)
	{
		Die();
	}

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (Equippedweapon) {
		Equippedweapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter)
{
	Super::GetHit_Implementation(Impactpoint, Hitter);
	if(!IsDead()) ShowHealthbar();
	ClearPatroltimer();
	Clearattacktimer();
	SetweaponcollisionEnabled(ECollisionEnabled::NoCollision);

	StopAttackMontage();

	if (InsideAttackradius() && bQuickResponsivetoAttckradius ) {
		if(!IsDead()) Startattacktimer();
	}

}

void AEnemy::FreezeEnemy(float Duration)
{
	if (bIsFrozen) return;

	bIsFrozen = true;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	Clearattacktimer();
	ClearPatroltimer();

	GetWorld()->GetTimerManager().SetTimer(
		FreezeTimerHandle,
		this,
		&AEnemy::UnFreezeEnemy,
		Duration,
		false
	);
}

void AEnemy::UnFreezeEnemy()
{
	bIsFrozen = false;

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));

	if (PawnSensingCp) {
		PawnSensingCp->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	if (OuterDetectionSphere) {
		OuterDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerOuterRange);
	}
	if (DetectionSphere) {
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerEnterRange);
	}

	if (bIsBossEnemy) {
		HideHealthbar();
	}

	if (!bIsBossEnemy) {
		IntializeEnemy();
		FTimerHandle FakePawnTimerHandle;
		GetWorldTimerManager().SetTimer(FakePawnTimerHandle, this, &AEnemy::SpawnFakePawn, 0.5f, false);
	}

}

void AEnemy::SpawnFakePawn()
{
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 150.f + FVector(0, 0, 50.f);
	AFakePatrolpawn* Fake = GetWorld()->SpawnActor<AFakePatrolpawn>(AFakePatrolpawn::StaticClass(), SpawnLocation, GetActorRotation());

	if (Fake)
	{
		Fake->Tags.Add(FName("FakePawn"));
	}
}

void AEnemy::Die()
{
	Super::Die();

	Enemystate = EEnemystate::EES_Dead;
	Clearattacktimer();
	HideHealthbar();
	DisableCapsule();
	SetLifeSpan(DeathlifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetweaponcollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSouls();

	if (bIsBossEnemy) {

		GetWorld()->GetTimerManager().SetTimer(
			WeaponSpawntimer,
			this,
			&AEnemy::SpawnWeapon,
			3.f,
			false
		);
	}
}

void AEnemy::SpawnSouls()
{
	UWorld* World = GetWorld();
	if (World && Soulclass && Attributes) {

		const FVector Spawnlocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* Spawnedsoul = World->SpawnActor<ASoul>(Soulclass, Spawnlocation, GetActorRotation());
		if (Spawnedsoul) {
			Spawnedsoul->SetSouls(Attributes->GetSoul());
			Spawnedsoul->SetOwner(this);
		}
	}
}

void AEnemy::SpawnWeapon()
{
	UWorld* World = GetWorld();
	if (World) {
		const FVector Spawnlocation = GetActorLocation() + FVector(0.f, 25.f, 25.f);
		if (WeaponSpawnclass) {
			AWeapon* SpawnedWeapon = World->SpawnActor<AWeapon>(WeaponSpawnclass, Spawnlocation, GetActorRotation());
		}
	}
}

void  AEnemy::Attack() {
	
	Super::Attack();
	if (Combattarget == nullptr) return;
	Enemystate = EEnemystate::EES_Engaged;
	PlayAttackmontage();

}

bool AEnemy::CcanAttack() {

	bool bCanattack =
		InsideAttackradius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanattack;
}

void AEnemy::Attackend() {
	Enemystate = EEnemystate::EES_Nostate;
	CheckcombatTarget();

}

void AEnemy::Handledamage(float DamageAmount) {

	Super::Handledamage(DamageAmount);

	if (Attributes && WidgetHealthbar) {
		Attributes->ReceiveDamage(DamageAmount);
		WidgetHealthbar->SetHealthPercent(Attributes->GetHealthpercent());
	}
}

void AEnemy::IntializeEnemy()
{
	EnemyAIController = Cast<AAIController>(GetController());
	HideHealthbar();
	MovetoTarget(Patroltarget);
	SpawnDefaultWeapon();
}

void AEnemy::CheckpatrolTarget()
{
	if (InTargetRange(Patroltarget, PatrolRadius)) {
		Patroltarget = ChoosepatrolTarget();
		const float Waittime = FMath::RandRange(Waitmin, Waitmax);
		GetWorldTimerManager().SetTimer(Patroltimer, this, &AEnemy::PatroltimerFinished, Waittime);
	}
}

void AEnemy::CheckcombatTarget()
{

	if (IsOutsideCombatradius()) {

		Clearattacktimer();
		Loseinterest();
		if (!IsEngaged()) StartPatrolling();

	}
	else if (IsOutsideAttackRadius() && !IsChasing()) {
		//Outside Attack range, Chase Charcter,
		Clearattacktimer();
		if (!IsEngaged()) ChasingTarget();

	}
	else if (CcanAttack()) {
		//Inside Attack range, Attack Charcter,

		Startattacktimer();

	}
}

void AEnemy::PatroltimerFinished()
{
	MovetoTarget(Patroltarget);
}

void AEnemy::HideHealthbar()
{
	if (WidgetHealthbar) {
		WidgetHealthbar->SetVisibility(false);
	}
}

void AEnemy::ShowHealthbar()
{
	if (WidgetHealthbar) {
		WidgetHealthbar->SetVisibility(true);
	}
}

void AEnemy::Loseinterest()
{
	if (!bIsBossEnemy) {
		Combattarget = nullptr;
		HideHealthbar();
	}
}

void AEnemy::StartPatrolling()
{
	Enemystate = EEnemystate::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = Patrollingspeed;
	MovetoTarget(Patroltarget); 
}

void AEnemy::ChasingTarget()
{

	Enemystate = EEnemystate::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = chasingspeed;
	MovetoTarget(Combattarget);

}

bool AEnemy::IsOutsideCombatradius()
{
	return !InTargetRange(Combattarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(Combattarget, AttackRadius);
}

bool AEnemy::InsideAttackradius()
{
	return InTargetRange(Combattarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return Enemystate == EEnemystate::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return Enemystate == EEnemystate::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return Enemystate == EEnemystate::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return Enemystate == EEnemystate::EES_Engaged;
}

void AEnemy::ClearPatroltimer()
{
	GetWorldTimerManager().ClearTimer(Patroltimer);
}

void AEnemy::Startattacktimer()
{
	Enemystate = EEnemystate::EES_Attacking;
	const float Attacktime = FMath::RandRange(Attackmin, Attackmax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, Attacktime);
}

void AEnemy::Clearattacktimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double Distancetotarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return Distancetotarget <= Radius;
}

void AEnemy::MovetoTarget(AActor* Target)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;
	FAIMoveRequest Moverequest;
	Moverequest.SetGoalActor(Target);
	Moverequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyAIController->MoveTo(Moverequest);
}

AActor* AEnemy::ChoosepatrolTarget()
{
	TArray<AActor*> ValidTargets;

	for (auto Target : Patroltargets) {
		if (Target != Patroltarget) {
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatroltarget = ValidTargets.Num();
	if (NumPatroltarget > 0) {
		const int32 Targetselection = FMath::RandRange(0, NumPatroltarget - 1);
		return ValidTargets[Targetselection];
	}
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		Equippedweapon = DefaultWeapon;
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		(Enemystate != EEnemystate::EES_Dead) &&
		(Enemystate != EEnemystate::EES_Chasing) &&
		(Enemystate < EEnemystate::EES_Attacking) &&
		(SeenPawn->ActorHasTag(FName("EngageableCharacter")) ||
		SeenPawn->ActorHasTag(FName("FakePawn")) );

	if (bShouldChaseTarget) {
		Combattarget = SeenPawn;
		ClearPatroltimer();
		ChasingTarget();
	}
}

void AEnemy::OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsBossEnemy || bHasWokenUp) return;

	if (OtherActor && OtherActor->ActorHasTag("EngageableCharacter"))
	{
		PlayWakeUpMontage();
		bHasWokenUp = true;

		// Delay before allowing normal behavior
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnableNormalBehavior, this, &AEnemy::EnableNormalBehavior, 2.0f, false);
	}
}

void AEnemy::OnPlayerOuterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsBossEnemy || bHasEnteredOuter) return;

	if (OtherActor && OtherActor->ActorHasTag("EngageableCharacter"))
	{
		bHasEnteredOuter = true;

		// Play Pose
		PlayPoseMontage();
	}
}

void AEnemy::PlayWakeUpMontage()
{
	if (WakeUpMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(WakeUpMontage);
	}
}

void AEnemy::PlayPoseMontage()
{
	if (PoseMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PoseMontage);
	}
}

void AEnemy::EnableNormalBehavior()
{
	bHasWokenUp = true;

	if (bIsBossEnemy)
	{
		IntializeEnemy();
	}
}

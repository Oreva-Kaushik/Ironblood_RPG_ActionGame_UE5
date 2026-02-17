// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/ComponentAttributes.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UComponentAttributes>(TEXT("Attributes"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

 
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) {

	if (IsAlive() && Hitter) {
		DirectionalHitreact(Hitter->GetActorLocation());
	}

	else Die();

	PlayHitsound(Impactpoint);
	SpawnHitparticles(Impactpoint);
}

void ABaseCharacter::Attack()
{
	if (Combattarget && Combattarget->ActorHasTag(FName("Dead"))) {
		Combattarget = nullptr;
	}
}

bool ABaseCharacter::CcanAttack()
{
	return false;
}
void ABaseCharacter::Attackend()
{
}
void ABaseCharacter::Dodgeend()
{
}
void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathmontage();
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisablemeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetweaponcollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (Equippedweapon && Equippedweapon->GetWeaponbox()) {
		Equippedweapon->GetWeaponbox()->SetCollisionEnabled(CollisionEnabled);
		Equippedweapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::PlatHitreactmontage(FName Sectionname)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();
	if (Animinstance && HitReactMontage) {
		Animinstance->Montage_Play(HitReactMontage);
		Animinstance->Montage_JumpToSection(Sectionname, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitreact(const FVector& Impactpoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(Impactpoint.X, Impactpoint.Y, GetActorLocation().Z);
	const FVector Tohit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// FORWARD * Tohit = |Forward||Tohit| * cos(theta)
	// |Forward|=1, |Tohit|=1, FORWARD * Tohit = cos(theta)
	const double Costheta = FVector::DotProduct(Forward, Tohit);
	// theta = inverse cos(FORWARD * Tohit)
	double Theta = FMath::Acos(Costheta);
	// convert from Radians to Degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//If Crossproduct point down, theta shoud be negative
	const FVector Crossproduct = FVector::CrossProduct(Forward, Tohit);
	if (Crossproduct.Z < 0) {
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f) {
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f) {
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f) {
		Section = FName("FromRight");
	}

	PlatHitreactmontage(Section);
}

void ABaseCharacter::PlayHitsound(const FVector& Impactpoint)
{
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			Impactpoint
		);
	}
}

void ABaseCharacter::SpawnHitparticles(const FVector& Impactpoint)
{
	if (Hitparticles && GetWorld()) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Hitparticles,
			Impactpoint
		);
	}
}

void ABaseCharacter::Handledamage(float DamageAmount)
{
	if (Attributes) {
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlaymontageSection(UAnimMontage* Montage,const FName& SectionName)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();
	if (Animinstance && Montage) {
		Animinstance->Montage_Play(Montage);
		Animinstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionInd = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionInd);
	PlaymontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayAttackmontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathmontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDiedstate> Pose(Selection);

	if (Pose < EDiedstate::EDS_MAX) {
		Diedstate = Pose;
	}

	return Selection;
}

void ABaseCharacter::PlayDodgeMontage()
{
	PlaymontageSection(DodgeMontage, FName("Default"));
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarptarget()
{
	if (Combattarget == nullptr) return FVector();

	const FVector Combattargetlocation = Combattarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector Targettome = (Location - Combattargetlocation).GetSafeNormal();
	Targettome *= WarpTargetdistance;

	return Combattargetlocation + Targettome;
}

FVector ABaseCharacter::GetRotationWarptarget()
{
	if (Combattarget) {
		return Combattarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

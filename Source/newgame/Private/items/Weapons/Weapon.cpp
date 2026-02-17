// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapons/Weapon.h"
#include "Characters/KKCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	Weaponbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weaponbox"));
	Weaponbox->SetupAttachment(GetRootComponent());
	Weaponbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weaponbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Weaponbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);


	Boxtracestart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxtraceStart"));
	Boxtracestart->SetupAttachment(GetRootComponent());

	Boxtraceend = CreateDefaultSubobject<USceneComponent>(TEXT("BoxtraceEnd"));
	Boxtraceend->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();

	Weaponbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxoverlap);

}

void AWeapon::Equip(USceneComponent* Inparent, FName Insocketname, AActor* NewOwner, APawn* NewInstigator)
{	
	Itemstate = EItemstate::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	Attachmeshtosocket(Inparent, Insocketname);
	DisableSphereCollision();
	if (NewOwner->ActorHasTag(TEXT("EngageableCharacter"))) {
		PlayequipSound();
	}
	DeactivateEmbers();
}

void AWeapon::DeactivateEmbers()
{
	if (ItemEffect) {
		ItemEffect->Deactivate();
	}
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::PlayequipSound()
{
	if (Equipsound) {
		UGameplayStatics::PlaySoundAtLocation(
			this, Equipsound,
			GetActorLocation()
		);
	}
}

void AWeapon::DetachItem()
{
	Dettachmeshtosocket();
	Itemstate = EItemstate::EIS_Hovering;

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (ItemEffect)
	{
		ItemEffect->Activate();
	}

	SetOwner(nullptr);
	SetInstigator(nullptr);

}

void AWeapon::Attachmeshtosocket(USceneComponent* Inparent, const FName& Insocketname)
{
	FAttachmentTransformRules Transformrule(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Inparent, Transformrule, Insocketname);
}

void AWeapon::Dettachmeshtosocket()
{
	FDetachmentTransformRules Transformrule(EDetachmentRule::KeepWorld, true);
	ItemMesh->DetachFromComponent(Transformrule);
}

void AWeapon::OnBoxoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorisSametype(OtherActor)) return;

	FHitResult BoxHit;
	BOXtrace(BoxHit);

	if (BoxHit.GetActor()) {

		if (ActorisSametype(BoxHit.GetActor())) return;

		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass() );

		ExecuteGetHit(BoxHit);

		Createfields(BoxHit.ImpactPoint);
	}
}

bool AWeapon::ActorisSametype(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* Hitinterface = Cast<IHitInterface>(BoxHit.GetActor());

	if (Hitinterface) {
		Hitinterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

void AWeapon::BOXtrace(FHitResult& BoxHit)
{
	const FVector Start = Boxtracestart->GetComponentLocation();
	const FVector End = Boxtraceend->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// I added this when character was hitting himself
	if (GetOwner()) {
		ActorsToIgnore.Add(GetOwner());
	}

	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(this,
		Start, End,
		BoxTraceExtent,
		Boxtracestart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

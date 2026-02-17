#pragma once

UENUM(BlueprintType)
enum class ECharstate : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "equipped")
};

UENUM(BlueprintType)
enum class EActionstate : uint8 
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Hitreaction UMETA(DisplayName = "Hitreaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EItemtaking : uint8
{
	EIT_Itemtaking UMETA(DisplayName = "Itemtaking"),
	EIT_Nottaking UMETA(DisplayName = "Nottaking")
};

UENUM(BlueprintType)
enum EDiedstate
{
	EDS_Death1 UMETA(DisplayName = "Death1"),
	EDS_Death2 UMETA(DisplayName = "Death2"),
	EDS_Death3 UMETA(DisplayName = "Death3"),
	EDS_Death4 UMETA(DisplayName = "Death4"),
	EDS_Death5 UMETA(DisplayName = "Death5"),
	EDS_Death6 UMETA(DisplayName = "Death6"),
	EDS_Death7 UMETA(DisplayName = "Death7"),
	EDS_Death8 UMETA(DisplayName = "Death8"),

	EDS_MAX UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EEnemystate : uint8
{
	EES_Nostate UMETA(DisplayName = "Nostate"),

	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")

};
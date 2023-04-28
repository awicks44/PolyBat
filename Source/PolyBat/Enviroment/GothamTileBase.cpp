// Fill out your copyright notice in the Description page of Project Settings.


#include "GothamTileBase.h"
#include "PolyBat/Shared/GothamTileManager.h"
#include "PolyBat/Shared/Scroller.h"
#include "PolyBat/Shared/UtilityLib.h"

// Sets default values
AGothamTileBase::AGothamTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AGothamTileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGothamTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


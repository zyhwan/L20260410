// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMeshComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMyStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddRelativeRotation(FRotator(0, 0, 7200.0f * DeltaTime));
}
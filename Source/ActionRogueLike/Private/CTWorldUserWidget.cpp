﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CTWorldUserWidget.h"

#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void UCTWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor is no longer valid, removing Health widget."))
		return;	
	}

	FVector2d ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),
		AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if (IsValid(ParentSizeBox))
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}

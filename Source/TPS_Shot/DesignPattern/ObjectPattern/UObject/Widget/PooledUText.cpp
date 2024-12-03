// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledUText.h"

void UPooledUText::Release()
{
	TWeakObjectPtr<UPooledUText> weakThis = TWeakObjectPtr<UPooledUText>(this);
	ReleaseBase(weakThis);
}

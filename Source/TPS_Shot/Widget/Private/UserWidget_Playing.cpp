// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/UserWidget_Playing.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

void UUserWidget_Playing::Reset(int score)
{
	UpdateScore(score);
}

void UUserWidget_Playing::Gameover()
{
	_mainText->SetText(FText::FromString("Gameover"));
}

void UUserWidget_Playing::UpdateHPBar(float newHpPercentage)
{
	_hpBar->SetPercent(newHpPercentage);
}

void UUserWidget_Playing::ChangeBulletNumber(int newBulletNumber)
{
	FText numberOfBullet = FText::Format(NSLOCTEXT(" ", "NumberOfBullet", "The number of bullet : {0}"), FText::AsNumber(newBulletNumber));
	_numberOfBullet->SetText(numberOfBullet);
}

void UUserWidget_Playing::SetHP(float percent)
{
	_hpBar->SetPercent(percent);
}

void UUserWidget_Playing::SetScoreText(int newScore)
{
	FText scoreText = FText::Format(NSLOCTEXT(" ", "ScoreText", "Score : {0}"), FText::AsNumber(newScore));
	_scoreText->SetText(scoreText);
}


void UUserWidget_Playing::UpdateScore(int newScore)
{
	SetScoreText(newScore);
}



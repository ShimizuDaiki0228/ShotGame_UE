// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUserWidget.h"

void UGameOverUserWidget::NativeConstruct()
{
	if (_restartButton)
	{
		_restartButton->Subscribe([]()
			{

			});
	}
	if (_titleButton)
	{
		_titleButton->Subscribe([]()
			{

			});
	}

	


}

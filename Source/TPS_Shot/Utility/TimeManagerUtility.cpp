// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManagerUtility.h"

TimeManagerUtility& TimeManagerUtility::GetInstance()
{
    static TimeManagerUtility Instance;
    return Instance;
}
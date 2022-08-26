// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Current Class Name + Function Name where this is called!
#define LOGUTIL_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define LOGUTIL_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define LOGUTIL_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define LOGUTIL_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class::Func and Line Number where this is called!
#define LOGUTIL_CUR_CLASS_FUNC_LINE (LOGUTIL_CUR_CLASS_FUNC + "(" + LOGUTIL_CUR_LINE + ")")

//Current Class and Line Number where this is called!
#define LOGUTIL_CUR_CLASS_LINE (LOGUTIL_CUR_CLASS + "(" + LOGUTIL_CUR_LINE + ")")

//Current Function Signature where this is called!
#define LOGUTIL_CUR_FUNCSIG (FString(__FUNCSIG__))


// Screen Message
//	Log to Screen!
#define VSCREENLOG(Param1) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(LOGUTIL_CUR_CLASS_LINE + ": " + Param1)) )

#define VSCREENLOG2(Param1,Param2) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(LOGUTIL_CUR_CLASS_LINE + ": " + Param1 + " " + Param2)) )

#define VSCREENLOGF(Param1,Param2) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(LOGUTIL_CUR_CLASS_LINE + ": " + Param1 + " " + FString::SanitizeFloat(Param2))) )

#define VSCREENMSG(FColor_color, FormatString, ...) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor_color, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ )) )

//UE LOG!
// #define V_LOG(LogCat, Param1) 		UE_LOG(LogCat,Warning,TEXT("%s: %s"), *LOGUTIL_CUR_CLASS_FUNC_LINE, *FString(Param1))

#define V_LOGS(LogCat, Param1) 		UE_LOG(LogCat,Warning,TEXT("%s: %s"), *LOGUTIL_CUR_CLASS_LINE, *FString(Param1))

#define V_LOG2(LogCat, Param1,Param2) 	UE_LOG(LogCat,Warning,TEXT("%s: %s %s"), *LOGUTIL_CUR_CLASS_LINE, *FString(Param1),*FString(Param2))

#define V_LOGF(LogCat, Param1,Param2) 	UE_LOG(LogCat,Warning,TEXT("%s: %s %f"), *LOGUTIL_CUR_CLASS_LINE, *FString(Param1),Param2)

#define V_LOGM(LogCat, FormatString , ...) UE_LOG(LogCat,Warning,TEXT("%s: %s"), 	*LOGUTIL_CUR_CLASS_FUNC_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

#define V_LOG(FormatString, ...) UE_LOG(LogTemp,Warning,TEXT("%s: %s"), 	*LOGUTIL_CUR_CLASS_FUNC_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

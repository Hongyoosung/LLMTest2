// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Http.h"
#include "Json.h"
#include "LLM.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LLMTEST2_API ULLM : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    ULLM();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Function to send a request to the ChatGPT API
    UFUNCTION(BlueprintCallable)
    void SendRequestToChatGPT(const FString& InMessage);

private:
    // HTTP Request callback function
    void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

    // Function to handle the response from the ChatGPT API
    void HandleResponse(const FString& ResponseString);

    // ChatGPT API endpoint URL
    UPROPERTY(EditAnywhere, Category = "ChatGPT")
    FString ChatGPTApiEndpoint;

    // API key for accessing ChatGPT API (if required)
    UPROPERTY(EditAnywhere, Category = "ChatGPT")
    FString ApiKey;

    // HTTP Request object
    TSharedPtr<IHttpRequest> HttpRequest;

    // Flag to indicate whether a request is currently in progress
    bool bIsRequestInProgress;
};

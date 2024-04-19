#include "LLMComponent.h"

// Sets default values for this component's properties
ULLMComponent::ULLMComponent()
{
    // Set up default values
    ChatGPTApiEndpoint = "https://api.openai.com/v1/chat/completions";
    bIsRequestInProgress = false;
}

// Called when the game starts
void ULLMComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void ULLMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

// Function to send a request to the ChatGPT API
void ULLMComponent::SendRequestToChatGPT(const FString& InMessage)
{
    if (!bIsRequestInProgress)
    {
        // Set up HTTP Request
        HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
            {
                OnRequestComplete(Request, Response, bSuccess);
            });
        HttpRequest->SetURL(ChatGPTApiEndpoint);
        HttpRequest->SetVerb("POST");
        HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        HttpRequest->SetContentAsString(InMessage);

        // Add API key header if required
        if (!ApiKey.IsEmpty())
        {
            HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));
        }

        // Send the request
        HttpRequest->ProcessRequest();
        bIsRequestInProgress = true;

    }
}

// HTTP Request callback function
void ULLMComponent::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        FString ResponseString = Response->GetContentAsString();
        HandleResponse(ResponseString);
    }
    else
    {
        // Handle error
    }

    bIsRequestInProgress = false;
}

// Function to handle the response from the ChatGPT API
void ULLMComponent::HandleResponse(const FString& ResponseString)
{
    // Parse and process the response here
}

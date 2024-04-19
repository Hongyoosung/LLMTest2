#include "LLM.h"

// Sets default values for this component's properties
ULLM::ULLM()
{
    // Set up default values
    HttpRequest = FHttpModule::Get().CreateRequest();

    ApiKey = "";

    // Set API endpoint and key
    HttpRequest->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
    HttpRequest->SetVerb(TEXT("POST"));

    // Set content type and request content
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    FString AuthHeaderValue = FString::Printf(TEXT("Bearer %s"), *ApiKey);
    HttpRequest->SetHeader(TEXT("Authorization"), *AuthHeaderValue);

    bIsRequestInProgress = false;
}

// Called when the game starts
void ULLM::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void ULLM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

// Function to send a request to the ChatGPT API
void ULLM::SendRequestToChatGPT(const FString& InMessage)
{
    UE_LOG(LogTemp, Warning, TEXT("Sending request to ChatGPT API"));
    if (!bIsRequestInProgress)
    {
        bIsRequestInProgress = true; // 요청이 시작되었음을 플래그로 설정합니다.

        // 변수 InMessage를 문자열로 변환하여 JSON 데이터에 삽입
        FString JsonData = FString::Printf(TEXT("{ \"model\": \"gpt-3.5-turbo\", \"messages\": [{ \"role\": \"system\", \"content\": \"Hello\" }], \"max_tokens\": 20 }"));

        HttpRequest->SetContentAsString(JsonData);

        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULLM::OnRequestComplete);

        // Send the request
        HttpRequest->ProcessRequest();
    }
}

void ULLM::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        UE_LOG(LogTemp, Warning, TEXT("Request successful"));
        HandleResponse(Response->GetContentAsString());
    }
    else
    {
        // Handle error
        if (bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("Request failed with response code: %d"), Response->GetResponseCode());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Request failed"));
        }
    }

    bIsRequestInProgress = false; // 요청이 완료된 후에 플래그를 설정합니다.
}

void ULLM::HandleResponse(const FString& ResponseString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
        return;
    }

    // Get the "choices" array from the JSON object
    TArray<TSharedPtr<FJsonValue>> Choices = JsonObject->GetArrayField(TEXT("choices"));


    // Extract the message from the first choice
    if (Choices.Num() > 0)
    {
        TSharedPtr<FJsonObject> FirstChoiceObject = Choices[0]->AsObject();
        TSharedPtr<FJsonObject> MessageObject = FirstChoiceObject->GetObjectField("message");
        FString Content;
        if (MessageObject->TryGetStringField("content", Content))
        {
            UE_LOG(LogTemp, Warning, TEXT("Message content: %s"), *Content);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get 'content' field from message object"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No choices found in response"));
    }
}



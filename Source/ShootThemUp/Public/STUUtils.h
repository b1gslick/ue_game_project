#pragma once
#include "Player/STU_PlayerState.h"

class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(AActor* PlayerActor)
    {

        if (!PlayerActor) return nullptr;

        const auto Component = PlayerActor->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    bool static AreEnemies(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

        const auto PlayerState1 = Cast<ASTU_PlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<ASTU_PlayerState>(Controller2->PlayerState);

        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }
};

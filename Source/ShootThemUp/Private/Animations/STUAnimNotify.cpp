// Shoot Them Up game, All right resievied

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation, EventReference);
}
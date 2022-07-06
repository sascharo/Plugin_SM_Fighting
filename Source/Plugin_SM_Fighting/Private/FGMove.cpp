#include "FGMove.h"

FFGMoveLinkToFollow UFGMove::TryLinks(const AFGDefaultPawn* DefaultPawn,
									  const TArray<USM_InputAtom*>& InputAtoms,
									  int32 DataIndex/*=0*/,
									  int32 RemainingSteps/*=-1*/)
{
	//UE_LOG(LogTemp, Error, TEXT("TryLinks"));
	FFGMoveLinkToFollow MoveLinkToFollow;

	for (int32 i = 0; i < LinkedMoves.Num(); ++i)
	{
		UE_LOG(LogTemp, Error, TEXT("TryLinks, %i - %s"), i, *LinkedMoves[i]->Move->Name.ToString());
		check(LinkedMoves[i]);
		//if (!ensure(LinkedMoves[i])) {}

		MoveLinkToFollow.SMResult = LinkedMoves[i]->TryLink(DefaultPawn, InputAtoms, DataIndex, RemainingSteps);
		UE_LOG(LogTemp, Warning, TEXT("TryLinks, %i"), MoveLinkToFollow.SMResult.CompletionType);
		
		if (MoveLinkToFollow.SMResult.CompletionType == EStateMachineCompletionType::Accepted)
		{
			UE_LOG(LogTemp, Error, TEXT("...........................................................................\n...........................................................................\n..........................................................................."));
			MoveLinkToFollow.Link = LinkedMoves[i];

			return MoveLinkToFollow;
		}
	}
	
	return MoveLinkToFollow;
}

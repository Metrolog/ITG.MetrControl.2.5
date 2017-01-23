SETLOCAL EnableDelayedExpansion

SET VERSION_H_FILE=%~f1 

echo // version data file > %VERSION_H_FILE%

FOR %%V IN (
	Major,Minor,Patch,PreReleaseTag,PreReleaseTagWithDash,PreReleaseLabel,PreReleaseNumber,
	BuildMetaData,BuildMetaDataPadded,FullBuildMetaData,MajorMinorPatch,SemVer,LegacySemVer,LegacySemVerPadded,
	AssemblySemVer,FullSemVer,InformationalVersion,BranchName,Sha,
	NuGetVersionV2,NuGetVersion,
	CommitsSinceVersionSource,CommitsSinceVersionSourcePadded,CommitDate
) DO (
	echo #define GITVERSION_%%V !GitVersion_%%V! >> %VERSION_H_FILE%
)

ENDLOCAL

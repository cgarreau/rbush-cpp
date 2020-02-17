##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=rbush
ConfigurationName      :=Release
WorkspacePath          := "."
ProjectPath            := "."
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Christophe Garreau
Date                   :=17/02/20
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="rbush.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./lib $(IncludeSwitch)./lib/rapidjson 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/usr/lib $(LibraryPathSwitch)/usr/local/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/WhichPolygon.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeoJson.cpp$(ObjectSuffix) $(IntermediateDirectory)/RunTimeStatistic.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/WhichPolygon.cpp$(ObjectSuffix): WhichPolygon.cpp $(IntermediateDirectory)/WhichPolygon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./WhichPolygon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WhichPolygon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WhichPolygon.cpp$(DependSuffix): WhichPolygon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WhichPolygon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WhichPolygon.cpp$(DependSuffix) -MM "WhichPolygon.cpp"

$(IntermediateDirectory)/WhichPolygon.cpp$(PreprocessSuffix): WhichPolygon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WhichPolygon.cpp$(PreprocessSuffix) "WhichPolygon.cpp"

$(IntermediateDirectory)/GeoJson.cpp$(ObjectSuffix): GeoJson.cpp $(IntermediateDirectory)/GeoJson.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./GeoJson.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeoJson.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeoJson.cpp$(DependSuffix): GeoJson.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeoJson.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeoJson.cpp$(DependSuffix) -MM "GeoJson.cpp"

$(IntermediateDirectory)/GeoJson.cpp$(PreprocessSuffix): GeoJson.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeoJson.cpp$(PreprocessSuffix) "GeoJson.cpp"

$(IntermediateDirectory)/RunTimeStatistic.cpp$(ObjectSuffix): RunTimeStatistic.cpp $(IntermediateDirectory)/RunTimeStatistic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "./RunTimeStatistic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RunTimeStatistic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RunTimeStatistic.cpp$(DependSuffix): RunTimeStatistic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RunTimeStatistic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RunTimeStatistic.cpp$(DependSuffix) -MM "RunTimeStatistic.cpp"

$(IntermediateDirectory)/RunTimeStatistic.cpp$(PreprocessSuffix): RunTimeStatistic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RunTimeStatistic.cpp$(PreprocessSuffix) "RunTimeStatistic.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/



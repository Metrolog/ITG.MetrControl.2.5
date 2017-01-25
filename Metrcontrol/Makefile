ITG_MAKEUTILS_DIR ?= ../ITG.MakeUtils
include $(ITG_MAKEUTILS_DIR)/common.mk
include $(ITG_MAKEUTILS_DIR)/tests.mk
include $(ITG_MAKEUTILS_DIR)/appveyor.mk
include $(ITG_MAKEUTILS_DIR)/signing/sign.mk

# ProjectName       := GOST2.304-81
# export ProductCode:=0FE6159A-9D17-482A-9873-85D4ECBEDA9B
# $(eval $(call exportGlobalVariable,ProductCode))
# export UpgradeCode:=7C5D2996-34AE-40E7-B53D-643346A9165C
# $(eval $(call exportGlobalVariable,UpgradeCode))
# WIXEXTENSIONS     = WixUIExtension

export Platform := x86
$(eval $(call exportGlobalVariable,Platform))
export Configuration := Release
$(eval $(call exportGlobalVariable,Configuration))
export Solution := ../Metrocontrol.sln
$(eval $(call exportGlobalVariable,Solution))
OUTPUTDIR := ../bin
export msiTARGETS := ../bin/$(Configuration)/$(Platform)/ru-RU/Metrcontrol.msi
$(eval $(call exportGlobalVariable,msiTARGETS))

$(eval $(call pushArtifactTargets,msiTARGETS))

all: msi
msi: $(msiTARGETS)
	$(pushDeploymentArtifact)

$(msiTARGETS): $(Solution) $(CODE_SIGNING_CERTIFICATE_PFX)
	msbuild.exe $<
	$(SIGNTARGET)

$(eval $(call defineTest,check_sign,msi,\
  $$(call SIGNTEST,$$<), \
  $(msiTARGETS) \
))

$(eval $(call defineTest,install,msi,\
  cd $$(dir $$<);\
  msiexec.exe /norestart /qn /leumo! install.log /i $$(notdir $$<);\
  wait;\
  EXIT_CODE=$$$$?;\
  iconv -f UCS2 -c install.log;\
  exit $$$$EXIT_CODE;\
  , \
  $(msiTARGETS) \
))

$(eval $(call defineTest,uninstall,msi,\
  cd $$(dir $$<);\
  msiexec.exe /norestart /qn /leumo! uninstall.log /x $$(notdir $$<);\
  wait;\
  EXIT_CODE=$$$$?;\
  iconv -f UCS2 -c uninstall.log;\
  exit $$$$EXIT_CODE;\
  , \
  $(msiTARGETS) \
))

clean::
	rm -rf ../packages

ITG_MAKEUTILS_DIR ?= ITG.MakeUtils
include $(ITG_MAKEUTILS_DIR)/common.mk

# sub projects

$(eval $(call useSubProject,sign,sign,pfx pvk spc))
$(eval $(call useSubProject,metrcontrol,Metrcontrol))

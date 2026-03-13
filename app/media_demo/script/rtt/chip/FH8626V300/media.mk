#lib
MEDIA_LIBS = -lvmm_rpc_arm_rtt -ldsprpc_leader_rtt -lisprpc_leader_rtt -lvb_mpi_rpc_arm_rtt -lmcdb_api_rpc_arm_rtt

ARM_CORTEXA7_VFP = 0

CFLAGS += -DRPC_MEDIA

# DEMO
# AF
ifeq ($(FH_APP_OPEN_AF), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/af/src/sample_common_af_demo.c)
MEDIA_LIBS += -laf
CFLAGS += -DFH_AF_ENABLE
endif
# ISP Strategy
ifeq ($(FH_APP_OPEN_ISP_STRATEGY_DEMO), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/isp_strategy/src/sample_common_isp_demo.c)
CFLAGS += -DFH_ISP_DEMO_ENABLE
endif
# MD and CD
ifneq (, $(findstring y, $(FH_APP_OPEN_MOTION_DETECT) $(FH_APP_OPEN_RECT_MOTION_DETECT) $(FH_APP_OPEN_COVER_DETECT)))
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/motion_and_cover_detect/src/sample_common_md_cd.c)
CFLAGS += -DFH_MD_CD_ENABLE
endif
# NNA_APP
MEDIA_LIBS += -lnnprimary_det_post
NN_ENABLE = n
ifeq ($(NN_ENABLE_G0), y)
NN_ENABLE = y
else ifeq ($(NN_ENABLE_G1), y)
NN_ENABLE = y
endif
ifeq ($(NN_ENABLE), y)
CFLAGS += -DFH_NN_ENABLE
ifneq (, $(findstring y, $(FH_NN_PERSON_DETECT_ENABLE) $(FH_NN_C2D_ENABLE) $(FH_NN_FACE_DET_ENABLE) $(FH_APP_OPEN_PVF_DETECT)))
# NNA IVS
CFLAGS += -DFH_NN_DETECT_ENABLE
ifeq ($(FH_APP_OPEN_IVS), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/nna_app/src/sample_nna_ivs.c)
MEDIA_LIBS += -ltrip_area_arm_rtt -ltrack_arm_rtt -lm
CFLAGS += -DFH_NN_IVS_ENABLE
else
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/nna_app/src/sample_nna_obj_detect.c)
endif
endif
ifeq ($(FH_NN_GESTURE_REC_ENABLE), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/nna_app/src/sample_nna_gesture_rec.c)
MEDIA_LIBS += -lnngesturedet_post -lnngesturerec_post -lfhalg_imgcrop
CFLAGS += -DFH_GESTURE_REC_ENABLE
endif
endif
# OSD
ifeq ($(FH_APP_OPEN_OVERLAY), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/src/sample_common_overlay_v2.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/src/overlay_gbox.c)
CFLAGS += -DFH_OVERLAY_ENABLE

ifeq ($(FH_APP_OVERLAY_LOGO), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/src/overlay_logo.c)
endif

ifeq ($(FH_APP_OVERLAY_TOSD), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/src/overlay_tosd.c)

ifeq ($(FH_APP_OVERLAY_TOSD_MENU), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/mod/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/top/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/mod/menu/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/mod/callback/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/component/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/core/*.c)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/osd_menu/*.c)
endif
endif

ifeq ($(FH_APP_OVERLAY_MASK), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/overlay/src/overlay_mask.c)
endif

endif
# SmartIR
FH_APP_USING_IRCUT = n
ifeq ($(FH_APP_USING_IRCUT_G0), y)
FH_APP_USING_IRCUT = y
else ifeq ($(FH_APP_USING_IRCUT_G1), y)
FH_APP_USING_IRCUT = y
else ifeq ($(FH_APP_USING_IRCUT_G2), y)
FH_APP_USING_IRCUT = y
else ifeq ($(FH_APP_USING_IRCUT_G3), y)
FH_APP_USING_IRCUT = y
endif
ifeq ($(FH_APP_USING_IRCUT), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/smartIR/src/sample_common_smartIR.c)
CFLAGS += -DFH_SMART_IR_ENABLE
endif
# VENC
ifeq ($(FH_APP_OPEN_VENC), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/venc/src/sample_common_venc.c)
CFLAGS += -DFH_VENC_DEMO_ENABLE
endif
# Vise
ifeq ($(FH_APP_OPEN_VISE), y)
SRCS += $(wildcard $(SAMPLE_DEMO_DIR)/vise_demo/src/sample_common_vise_demo.c)
CFLAGS += -DFH_VISE_DEMO_ENABLE
endif

#sensor
ifeq ($(I2C_ON_ARM), y)
MEDIA_LIBS += -lsensor_rtt

OVOS02K_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS02K_MIPI_G0),y)
OVOS02K_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02K_MIPI_G1),y)
OVOS02K_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02K_MIPI_G2),y)
OVOS02K_MIPI_ENABLE = 1
endif
ifeq ($(OVOS02K_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS02K_MIPI
MEDIA_LIBS += -lovos02k_mipi_rtt
endif

OVOS02D_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS02D_MIPI_G0),y)
OVOS02D_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02D_MIPI_G1),y)
OVOS02D_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02D_MIPI_G2),y)
OVOS02D_MIPI_ENABLE = 1
endif
ifeq ($(OVOS02D_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS02D_MIPI
MEDIA_LIBS += -lovos02d_mipi_rtt
endif

OVOS02H_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS02H_MIPI_G0),y)
OVOS02H_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02H_MIPI_G1),y)
OVOS02H_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02H_MIPI_G2),y)
OVOS02H_MIPI_ENABLE = 1
endif
ifeq ($(OVOS02H_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS02H_MIPI
MEDIA_LIBS += -lovos02h_mipi_rtt
endif

OVOS02G10_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS02G10_MIPI_G0),y)
OVOS02G10_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02G10_MIPI_G1),y)
OVOS02G10_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS02G10_MIPI_G2),y)
OVOS02G10_MIPI_ENABLE = 1
endif
ifeq ($(OVOS02G10_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS02G10_MIPI
MEDIA_LIBS += -lovos02g10_mipi_rtt
endif

OVOS03A_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS03A_MIPI_G0),y)
OVOS03A_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS03A_MIPI_G1),y)
OVOS03A_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS03A_MIPI_G2),y)
OVOS03A_MIPI_ENABLE = 1
endif
ifeq ($(OVOS03A_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS03A_MIPI
MEDIA_LIBS += -lovos03a_mipi_rtt
endif

OVOS04C10_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS04C10_MIPI_G0),y)
OVOS04C10_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS04C10_MIPI_G1),y)
OVOS04C10_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS04C10_MIPI_G2),y)
OVOS04C10_MIPI_ENABLE = 1
endif
ifeq ($(OVOS04C10_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS04C10_MIPI
MEDIA_LIBS += -lovos04c10_mipi_rtt
endif

OVOS05_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS05_MIPI_G0),y)
OVOS05_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS05_MIPI_G1),y)
OVOS05_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS05_MIPI_G2),y)
OVOS05_MIPI_ENABLE = 1
endif
ifeq ($(OVOS05_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS05_MIPI
MEDIA_LIBS += -lovos05_mipi_rtt
endif

OVOS06A_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS06A_MIPI_G0),y)
OVOS06A_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS06A_MIPI_G1),y)
OVOS06A_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS06A_MIPI_G2),y)
OVOS06A_MIPI_ENABLE = 1
endif
ifeq ($(OVOS06A_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS06A_MIPI
MEDIA_LIBS += -lovos06a_mipi_rtt
endif

OVOS08_MIPI_ENABLE = 0
ifeq ($(FH_USING_OVOS08_MIPI_G0),y)
OVOS08_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OVOS08_MIPI_G1),y)
OVOS08_MIPI_ENABLE = 1
endif
ifeq ($(OVOS08_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OVOS08_MIPI
MEDIA_LIBS += -lovos08_mipi_rtt
endif

SC3335_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC3335_MIPI_G0),y)
SC3335_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC3335_MIPI_G1),y)
SC3335_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC3335_MIPI_G2),y)
SC3335_MIPI_ENABLE = 1
endif
ifeq ($(SC3335_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC3335_MIPI
MEDIA_LIBS += -lsc3335_mipi_rtt
endif

SC3336_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC3336_MIPI_G0),y)
SC3336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC3336_MIPI_G1),y)
SC3336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC3336_MIPI_G2),y)
SC3336_MIPI_ENABLE = 1
endif
ifeq ($(SC3336_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC3336_MIPI
MEDIA_LIBS += -lsc3336_mipi_rtt
endif

SC4336_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC4336_MIPI_G0),y)
SC4336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC4336_MIPI_G1),y)
SC4336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC4336_MIPI_G2),y)
SC4336_MIPI_ENABLE = 1
endif
ifeq ($(SC4336_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC4336_MIPI
MEDIA_LIBS += -lsc4336_mipi_rtt
endif

SC5336_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC5336_MIPI_G0),y)
SC5336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC5336_MIPI_G1),y)
SC5336_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC5336_MIPI_G2),y)
SC5336_MIPI_ENABLE = 1
endif
ifeq ($(SC5336_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC5336_MIPI
MEDIA_LIBS += -lsc5336_mipi_rtt
endif

SC200AI_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC200AI_MIPI_G0),y)
SC200AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC200AI_MIPI_G1),y)
SC200AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC200AI_MIPI_G2),y)
SC200AI_MIPI_ENABLE = 1
endif
ifeq ($(SC200AI_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC200AI_MIPI
MEDIA_LIBS += -lsc200ai_mipi_rtt
endif

SC450AI_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC450AI_MIPI_G0),y)
SC450AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC450AI_MIPI_G1),y)
SC450AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC450AI_MIPI_G2),y)
SC450AI_MIPI_ENABLE = 1
endif
ifeq ($(SC450AI_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC450AI_MIPI
MEDIA_LIBS += -lsc450ai_mipi_rtt
endif

SC530AI_MIPI_ENABLE = 0
ifeq ($(FH_USING_SC530AI_MIPI_G0),y)
SC530AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC530AI_MIPI_G1),y)
SC530AI_MIPI_ENABLE = 1
else ifeq ($(FH_USING_SC530AI_MIPI_G2),y)
SC530AI_MIPI_ENABLE = 1
endif
ifeq ($(SC530AI_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_SC530AI_MIPI
MEDIA_LIBS += -lsc530ai_mipi_rtt
endif

OV2735_MIPI_ENABLE = 0
ifeq ($(FH_USING_OV2735_MIPI_G0),y)
OV2735_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OV2735_MIPI_G1),y)
OV2735_MIPI_ENABLE = 1
else ifeq ($(FH_USING_OV2735_MIPI_G2),y)
OV2735_MIPI_ENABLE = 1
endif
ifeq ($(OV2735_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_OV2735_MIPI
MEDIA_LIBS += -lov2735_mipi_rtt
endif

GC2083_MIPI_ENABLE = 0
ifeq ($(FH_USING_GC2083_MIPI_G0),y)
GC2083_MIPI_ENABLE = 1
else ifeq ($(FH_USING_GC2083_MIPI_G1),y)
GC2083_MIPI_ENABLE = 1
else ifeq ($(FH_USING_GC2083_MIPI_G2),y)
GC2083_MIPI_ENABLE = 1
endif
ifeq ($(GC2083_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_GC2083_MIPI
MEDIA_LIBS += -lgc2083_mipi_rtt
endif

IMX415_MIPI_ENABLE = 0
ifeq ($(FH_USING_IMX415_MIPI_G0),y)
IMX415_MIPI_ENABLE = 1
else ifeq ($(FH_USING_IMX415_MIPI_G1),y)
IMX415_MIPI_ENABLE = 1
endif
ifeq ($(IMX415_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_IMX415_MIPI
MEDIA_LIBS += -limx415_mipi_rtt
endif

PS5270_MIPI_ENABLE = 0
ifeq ($(FH_USING_PS5270_MIPI_G0),y)
PS5270_MIPI_ENABLE = 1
else ifeq ($(FH_USING_PS5270_MIPI_G1),y)
PS5270_MIPI_ENABLE = 1
else ifeq ($(FH_USING_PS5270_MIPI_G2),y)
PS5270_MIPI_ENABLE = 1
endif
ifeq ($(PS5270_MIPI_ENABLE),1)
CFLAGS += -DFH_USING_PS5270_MIPI
MEDIA_LIBS += -lps5270_mipi_rtt
endif

DUMMY_SENSOR_ENABLE = 0
ifeq ($(FH_USING_DUMMY_SENSOR_G0),y)
DUMMY_SENSOR_ENABLE = 1
else ifeq ($(FH_USING_DUMMY_SENSOR_G1),y)
DUMMY_SENSOR_ENABLE = 1
else ifeq ($(FH_USING_DUMMY_SENSOR_G2),y)
DUMMY_SENSOR_ENABLE = 1
endif
ifeq ($(DUMMY_SENSOR_ENABLE),1)
CFLAGS += -DFH_USING_DUMMY_SENSOR
MEDIA_LIBS += -ldummy_sensor
endif
endif

LIBS += $(MEDIA_LIBS)

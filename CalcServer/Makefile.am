AM_CPPFLAGS=-I${top_srcdir}/include \
	-I${top_srcdir}/sevent/ \
	-I${top_srcdir}/CalcServer/ \
	-I${top_srcdir}/CalcServer/opencv/inc \
	-I${top_srcdir}/CalcServer/dip/a_image/include \
	-I${top_srcdir}/CalcServer/dip/autolabel_1_0_1/include \
	-I${top_srcdir}/CalcServer/dip/autolabel_2_0_1/include \
	-I${top_srcdir}/CalcServer/dip/cf/include \
	-I${top_srcdir}/CalcServer/dip/colorFeat_3_0_0_4/include \
	-I${top_srcdir}/CalcServer/dip/gf/Inc \
	-I${top_srcdir}/CalcServer/dip/image_splicing/include \
	-I${top_srcdir}/CalcServer/dip/iq/include \
	-I${top_srcdir}/CalcServer/dip/lf/include \
	-I${top_srcdir}/CalcServer/dip/npx/include \
	-I${top_srcdir}/CalcServer/dip/sf/Inc/ \
	-I${top_srcdir}/CalcServer/dip/tm_judge/include \
	-I${top_srcdir}/CalcServer/dip/tv/include

LDADD= $(top_srcdir)/lib/liblog4cxx.a \
	$(top_srcdir)/lib/libapr-1.a \
	$(top_srcdir)/lib/libaprutil-1.a \
	$(top_srcdir)/lib/libprotobuf.a \
	$(top_srcdir)/lib/libxml2.a \
	$(top_srcdir)/sevent/libsevent.a \
	$(top_srcdir)/CalcServer/dip/sf/Lib/libSF_1_0_0.a $(top_srcdir)/CalcServer/dip/sf/Input/lib/libCenterExtract.a $(top_srcdir)/CalcServer/dip/sf/Input/lib/libInvertedFileIndex.a $(top_srcdir)/CalcServer/dip/sf/Input/lib/libfftw3f.a $(top_srcdir)/CalcServer/dip/sf/Input/lib/libwatershed_1_0_0.a \
	$(top_srcdir)/CalcServer/dip/cf/lib/libcolorFeat_2_0_0.a\
	$(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/lib/libcolorFeat_3_0_0.a $(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/input/lib/libCenterExtract_1_2_0.a $(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/input/lib/libColorSeg_1_0_0.a $(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/input/lib/libInvertedFileIndex.a $(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/input/lib/libVector.a $(top_srcdir)/CalcServer/dip/colorFeat_3_0_0_4/input/lib/libmainBody_3_0_0.a \
	$(top_srcdir)/CalcServer/dip/gf/Lib/libGlobalFeature_4_0_0.a $(top_srcdir)/CalcServer/dip/gf/Input/lib/libAutoJudgeImage.a $(top_srcdir)/CalcServer/dip/gf/Input/lib/libfftw3f.a $(top_srcdir)/CalcServer/dip/gf/Input/lib/libCenterExtract.a $(top_srcdir)/CalcServer/dip/gf/Input/lib/libInvertedFileIndex.a\
	$(top_srcdir)/CalcServer/dip/lf/lib/libLocalRuku.a $(top_srcdir)/CalcServer/dip/lf/lib/libLocalExtracter.a $(top_srcdir)/CalcServer/dip/lf/lib/libOpencv243Patch.a \
	$(top_srcdir)/CalcServer/dip/tv/lib/libTVMergeV3_0_0_1.a $(top_srcdir)/CalcServer/dip/tv/lib/libwordlist_filter_TV_1.0.3.a\
	$(top_srcdir)/CalcServer/dip/autolabel_1_0_1/lib/libautoLabel_1_0_1.a\
	$(top_srcdir)/CalcServer/dip/autolabel_2_0_1/lib/libautoLabel_2_0_1.a\
	$(top_srcdir)/CalcServer/dip/npx/lib/libNPXFilterV1_1_0.a\
	$(top_srcdir)/CalcServer/dip/iq/lib/libImageQualityV1_1_1.a dip/iq/lib/libSalientMap.a\
	$(top_srcdir)/CalcServer/dip/image_splicing/lib/libImageSplicing_1_1_0.a dip/image_splicing/input/lib/libSalientMap.a\
	$(top_srcdir)/CalcServer/dip/a_image/lib/libImageTypeAJudge_2_0_0.a\
	$(top_srcdir)/CalcServer/dip/tm_judge/lib/libTMJudge_1_0_2.a

AM_LDFLAGS=-lpthread -lrt -lexpat -g -Wall -Lopencv/lib/ -lcv -lhighgui -lcxcore -lssl -lml -lcvaux

source_list= proto_message.pb.cc i_dont_known_opencv.cpp CalcServer.cpp
calcServer_list=main.cpp
client_list=client.cpp

bin_PROGRAMS = calcServer client

calcServer_SOURCES = ${source_list} ${calcServer_list}

client_SOURCES = ${source_list} ${client_list}


#非全局编译
if(NOT BUILD_ALL)
	cmake_minimum_required(VERSION 2.8)
	project(public_common_fastdepend)
	SET( CMAKE_VERBOSE_MAKEFILE on )
endif(NOT BUILD_ALL)



#包含的头文件目录，USER_INC_PATH,HEADERS,必须包含
INCLUDE($ENV{OB_SRC}/includehead.cmake)

#编译目标
SET(MY_TARGET cleanCycle${BUILDTYPE})

#自定义包含的头文件目录，可选
INCLUDE_DIRECTORIES( $ENV{OB_SRC}/public/common/log 
	$ENV{OB_REL}/include
	$ENV{OB_REL}/include/public
	$ENV{OB_REL}/include/public/xc
	$ENV{OB_REL}/include/public/common
	$ENV{OB_REL}/include/public/common/express
	$ENV{OB_REL}/include/public/common/log
	$ENV{OB_REL}/include/public/common/base	 
	$ENV{OB_REL}/include/3rd
	$ENV{OB_REL}/include/3rd/lua
	$ENV{OB_REL}/include/balance	
	$ENV{OB_REL}/include/rating_billing/rating/xdr_def
	$ENV{OB_REL}/include/rating_billing/rating/xc_param/common
	$ENV{OB_REL}/include/rating_billing/rating/xc_param/xqueryrating
	$ENV{OB_REL}/include/rating_billing/rating/xc_param/xloadrating)

#生成BIN包含该文件
INCLUDE($ENV{OB_SRC}/cmake.${PRJ_NAME}/ob_add_executable.cmake)

#出现在g++后面,必须包含
INCLUDE($ENV{OB_SRC}/makeoption.cmake)

#自定义-D，可选

SET(SRC_LIST itemAddUpForCleanCycle.cpp)
#增加依赖关系
#ADD_DEPENDENCIES(${MY_TARGET})
link_directories(/usr/lib/64  ${OB_REL}/lib)
ADD_LIBRARY(${MY_TARGET} SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES(${MY_TARGET} PROPERTIES VERSION ${VERSION_REV})
target_link_libraries(${MY_TARGET}  public_common_express${BUILDTYPE}
								public_common_base${BUILDTYPE}
								commlog${BUILDTYPE}
								Xdr_app${BUILDTYPE}
								ailog_interface${BUILDTYPE}
								xqueryrating${BUILDTYPE})

##############################################################################
#for makefile.prodBillCycle
ADD_LIBRARY(prodBillCycle${BUILDTYPE} SHARED ProdBillCycle.cpp )
SET_TARGET_PROPERTIES(prodBillCycle${BUILDTYPE} PROPERTIES VERSION ${VERSION_REV})
INSTALL(TARGETS prodBillCycle${BUILDTYPE} LIBRARY DESTINATION ${LIB_INSTALL_PATH})

target_link_libraries(prodBillCycle${BUILDTYPE} public_common_base${BUILDTYPE}
								cleanCycle${BUILDTYPE}
								Xdr_app${BUILDTYPE}							
								commlog${BUILDTYPE})
#end for makefile.prodBillCycle								
##############################################################################
##############################################################################
#for makefile.prorateDeductRule
ADD_LIBRARY(prorateDeductRule${BUILDTYPE} SHARED ProrateDeductRule.cpp ProrateDeductRuleFactory.cpp )
SET_TARGET_PROPERTIES(prorateDeductRule${BUILDTYPE} PROPERTIES VERSION ${VERSION_REV})
INSTALL(TARGETS prorateDeductRule${BUILDTYPE} LIBRARY DESTINATION ${LIB_INSTALL_PATH})
target_link_libraries(prorateDeductRule${BUILDTYPE} public_common_express${BUILDTYPE}
								public_common_base${BUILDTYPE}
								commlog${BUILDTYPE}
								ailog_interface${BUILDTYPE}
								xqueryrating${BUILDTYPE})
#end for makefile.prorateDeductRule								
##############################################################################



##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
##  * Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
##  * Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
##  * Neither the name of NVIDIA CORPORATION nor the names of its
##    contributors may be used to endorse or promote products derived
##    from this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
## EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
## PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
## CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
## OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
## Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.

#
# Build LowLevelAABB
#

SET(LOWLEVELAABB_PLATFORM_INCLUDES
	${PHYSX_SOURCE_DIR}/Common/src/windows
	${PHYSX_SOURCE_DIR}/LowLevelAABB/windows/include
	${PHYSX_SOURCE_DIR}/GpuBroadPhase/include
	${PHYSX_SOURCE_DIR}/GpuBroadPhase/src
)


SET(LOWLEVELAABB_COMPILE_DEFS

	# Common to all configurations
	${PHYSX_UWP_COMPILE_DEFS};${PHYSX_LIBTYPE_DEFS};

	$<$<CONFIG:debug>:${PHYSX_UWP_DEBUG_COMPILE_DEFS};>
	$<$<CONFIG:checked>:${PHYSX_UWP_CHECKED_COMPILE_DEFS};>
	$<$<CONFIG:profile>:${PHYSX_UWP_PROFILE_COMPILE_DEFS};>
	$<$<CONFIG:release>:${PHYSX_UWP_RELEASE_COMPILE_DEFS};>
)

IF(PX_GENERATE_STATIC_LIBRARIES)
	SET(LOWLEVELAABB_LIBTYPE OBJECT)
ELSE()
	SET(LOWLEVELAABB_LIBTYPE STATIC)
ENDIF()


IF(NV_USE_GAMEWORKS_OUTPUT_DIRS AND LOWLEVELAABB_LIBTYPE STREQUAL "STATIC")
	SET(LLAABB_COMPILE_PDB_NAME_DEBUG "LowLevelAABB_static${CMAKE_DEBUG_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_CHECKED "LowLevelAABB_static${CMAKE_CHECKED_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_PROFILE "LowLevelAABB_static${CMAKE_PROFILE_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_RELEASE "LowLevelAABB_static${CMAKE_RELEASE_POSTFIX}")
ELSE()
	SET(LLAABB_COMPILE_PDB_NAME_DEBUG "LowLevelAABB${CMAKE_DEBUG_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_CHECKED "LowLevelAABB${CMAKE_CHECKED_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_PROFILE "LowLevelAABB${CMAKE_PROFILE_POSTFIX}")
	SET(LLAABB_COMPILE_PDB_NAME_RELEASE "LowLevelAABB${CMAKE_RELEASE_POSTFIX}")
ENDIF()

IF(PX_EXPORT_LOWLEVEL_PDB)
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_DEBUG "${PHYSX_ROOT_DIR}/${PX_ROOT_LIB_DIR}/debug/${LLAABB_COMPILE_PDB_NAME_DEBUG}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_CHECKED "${PHYSX_ROOT_DIR}/${PX_ROOT_LIB_DIR}/checked/${LLAABB_COMPILE_PDB_NAME_CHECKED}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_PROFILE "${PHYSX_ROOT_DIR}/${PX_ROOT_LIB_DIR}/profile/${LLAABB_COMPILE_PDB_NAME_PROFILE}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_RELEASE "${PHYSX_ROOT_DIR}/${PX_ROOT_LIB_DIR}/release/${LLAABB_COMPILE_PDB_NAME_RELEASE}")

	INSTALL(FILES ${PHYSX_ROOT_DIR}/$<$<CONFIG:debug>:${PX_ROOT_LIB_DIR}/debug>$<$<CONFIG:release>:${PX_ROOT_LIB_DIR}/release>$<$<CONFIG:checked>:${PX_ROOT_LIB_DIR}/checked>$<$<CONFIG:profile>:${PX_ROOT_LIB_DIR}/profile>/$<$<CONFIG:debug>:${LLAABB_COMPILE_PDB_NAME_DEBUG}>$<$<CONFIG:checked>:${LLAABB_COMPILE_PDB_NAME_CHECKED}>$<$<CONFIG:profile>:${LLAABB_COMPILE_PDB_NAME_PROFILE}>$<$<CONFIG:release>:${LLAABB_COMPILE_PDB_NAME_RELEASE}>.pdb
		DESTINATION $<$<CONFIG:debug>:${PX_ROOT_LIB_DIR}/debug>$<$<CONFIG:release>:${PX_ROOT_LIB_DIR}/release>$<$<CONFIG:checked>:${PX_ROOT_LIB_DIR}/checked>$<$<CONFIG:profile>:${PX_ROOT_LIB_DIR}/profile> OPTIONAL)	
ELSE()
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_DEBUG "${LLAABB_COMPILE_PDB_NAME_DEBUG}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_CHECKED "${LLAABB_COMPILE_PDB_NAME_CHECKED}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_PROFILE "${LLAABB_COMPILE_PDB_NAME_PROFILE}")
	SET(LOWLEVELAABB_COMPILE_PDB_NAME_RELEASE "${LLAABB_COMPILE_PDB_NAME_RELEASE}")
ENDIF()

SET(LOWLEVELAABB_PLATFORM_LINK_FLAGS "/MAP")


CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(perigee)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0074 NEW)
ENDIF(COMMAND cmake_policy)

SET(perigee_VERSION_MAJOR 0)
SET(perigee_VERSION_MINOR 0)

IF( ${CMAKE_BUILD_TYPE} MATCHES "Release" )
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "set to release" FORCE)
ELSE( ${CMAKE_BUILD_TYPE} MATCHES "Release" )
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "default to debug" FORCE)
ENDIF( ${CMAKE_BUILD_TYPE} MATCHES "Release" )

OPTION(BUILD_SHARED_LIBS "build shard libraries" OFF)

# Libraries location
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# binary location
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

INCLUDE(../../conf/basic_variable_log.cmake)
INCLUDE(../../conf/system_lib_loading.cmake)

SET( perigee_source ${perigee_SOURCE_DIR}/../../src )
# 1. Preprocessor source cpp
SET( perigee_preprocess_lib_src 
  ${perigee_source}/System/Vector_3.cpp
  ${perigee_source}/System/Matrix_3x3.cpp
  ${perigee_source}/System/Matrix_double_3by3_Array.cpp
  ${perigee_source}/System/Math_Tools.cpp
  ${perigee_source}/System/HDF5_Writer.cpp
  ${perigee_source}/System/HDF5_Reader.cpp
  ${perigee_source}/Mesh/VTK_Tools.cpp
  ${perigee_source}/Mesh/Tet_Tools.cpp
  ${perigee_source}/Mesh/Gmsh_FileIO.cpp
  ${perigee_source}/Mesh/Mesh_Tet.cpp
  ${perigee_source}/Mesh/IEN_FEM.cpp
  ${perigee_source}/Mesh/INodalBC.cpp
  ${perigee_source}/Mesh/IPart.cpp
  ${perigee_source}/Mesh/Part_Tet.cpp
  ${perigee_source}/Mesh/ElemBC_3D_tet.cpp
  ${perigee_source}/Mesh/ElemBC_3D_tet_outflow.cpp
  ${perigee_source}/Mesh/NodalBC_3D_vtp.cpp
  ${perigee_source}/Mesh/NodalBC_3D_vtu.cpp
  ${perigee_source}/Mesh/NodalBC_3D_inflow.cpp
  ${perigee_source}/Mesh/QuadPts_Gauss_Triangle.cpp
  ${perigee_source}/Mesh/NBC_Partition.cpp
  ${perigee_source}/Mesh/NBC_Partition_inflow.cpp
  ${perigee_source}/Mesh/EBC_Partition.cpp
  ${perigee_source}/Mesh/EBC_Partition_outflow.cpp
  ${perigee_source}/Mesh/Map_Node_Index.cpp
  ${perigee_source}/Mesh/Global_Part_METIS.cpp
  ${perigee_source}/Mesh/Global_Part_Serial.cpp
  ${perigee_source}/Element/FEAElement_Triangle3_3D_der0.cpp
  ${perigee_source}/Element/FEAElement_Triangle6_3D_der0.cpp
  )

SET( perigee_analysis_lib_src
  ${perigee_source}/System/PETSc_Tools.cpp
  ${perigee_source}/System/Matrix_double_6by6_Array.cpp
  ${perigee_source}/System/Matrix_PETSc.cpp
  ${perigee_source}/System/Matrix_3x3.cpp
  ${perigee_source}/System/SymmMatrix_3x3.cpp
  ${perigee_source}/Mesh/QuadPts_Gauss_Tet.cpp
  ${perigee_source}/Analysis_Tool/AGlobal_Mesh_Info_FEM_3D.cpp
  ${perigee_source}/Analysis_Tool/APart_Basic_Info.cpp
  ${perigee_source}/Analysis_Tool/ALocal_Elem.cpp
  ${perigee_source}/Analysis_Tool/ALocal_IEN.cpp
  ${perigee_source}/Analysis_Tool/ALocal_NBC.cpp
  ${perigee_source}/Analysis_Tool/ALocal_InflowBC.cpp
  ${perigee_source}/Analysis_Tool/ALocal_EBC.cpp
  ${perigee_source}/Analysis_Tool/ALocal_EBC_outflow.cpp
  ${perigee_source}/Analysis_Tool/APart_Node.cpp
  ${perigee_source}/Element/FEANode.cpp
  ${perigee_source}/Element/FEAElement_Tet4.cpp
  ${perigee_source}/Element/FEAElement_Tet10_v2.cpp
  ${perigee_source}/Model/GenBC_RCR.cpp
  ${perigee_source}/Model/GenBC_Resistance.cpp
  ${perigee_source}/Model/GenBC_Inductance.cpp
  ${perigee_source}/Model/GenBC_Coronary.cpp
  ${perigee_source}/Model/GenBC_Tools.cpp
  ${perigee_source}/Model/GenBC_Pressure.cpp
  ${perigee_source}/Model/CVFlowRate_Unsteady.cpp
  ${perigee_source}/Model/CVFlowRate_Linear2Steady.cpp
  ${perigee_source}/Solver/PDNSolution.cpp
  ${perigee_source}/Solver/PDNTimeStep.cpp
  ${perigee_source}/Solver/TimeMethod_GenAlpha.cpp
  ${perigee_source}/Solver/PLinear_Solver_PETSc.cpp
  ${perigee_SOURCE_DIR}/src/PLocAssem_Tet_VMS_NS_GenAlpha.cpp
  ${perigee_SOURCE_DIR}/src/PGAssem_NS_FEM.cpp
  ${perigee_SOURCE_DIR}/src/PDNSolution_NS.cpp
  ${perigee_SOURCE_DIR}/src/PNonlinear_NS_Solver.cpp
  ${perigee_SOURCE_DIR}/src/PTime_NS_Solver.cpp
  )

SET( perigee_postprocess_lib_src
  ${perigee_source}/Mesh/QuadPts_vis_tri6.cpp
  ${perigee_source}/Mesh/QuadPts_vis_tet4.cpp
  ${perigee_source}/Mesh/QuadPts_vis_tet10_v2.cpp
  ${perigee_source}/Postproc_Tool/PostVectSolution.cpp
  ${perigee_source}/Postproc_Tool/Vis_Tools.cpp
  ${perigee_source}/Postproc_Tool/Interpolater.cpp
  ${perigee_SOURCE_DIR}/src/VisDataPrep_NS.cpp
  ${perigee_SOURCE_DIR}/src/VTK_Writer_NS.cpp
  )

INCLUDE_DIRECTORIES( ../../include )
INCLUDE_DIRECTORIES( ./include )

# -------------------------------------------------------------------
# MAKE MY OWN LIBRARIES
# 1. Preprocess libs
ADD_LIBRARY( perigee_preprocess ${perigee_preprocess_lib_src} )
TARGET_LINK_LIBRARIES( perigee_preprocess ${EXTRA_LINK_LIBS} )

# 2. Analysis libs
ADD_LIBRARY( perigee_analysis ${perigee_analysis_lib_src} )
TARGET_LINK_LIBRARIES( perigee_analysis ${EXTRA_LINK_LIBS} perigee_preprocess )

# 3. Postprocess lib
ADD_LIBRARY( perigee_postprocess ${perigee_postprocess_lib_src} )
TARGET_LINK_LIBRARIES( perigee_postprocess ${EXTRA_LINK_LIBS} perigee_analysis
  perigee_preprocess )
# -------------------------------------------------------------------

# Print compiler setup
INCLUDE(../../conf/message_compiler_setup.cmake)

# ===================================================================
# BUILD THE MAIN DRIVERS
# ===================================================================
ADD_EXECUTABLE( gmsh_p1_IO gmsh_p1_process.cpp )
ADD_EXECUTABLE( gmsh_p2_IO gmsh_p2_process.cpp )
ADD_EXECUTABLE( preprocess3d preprocess_ns.cpp)
ADD_EXECUTABLE( ns3d ns_driver.cpp)
ADD_EXECUTABLE( prepost3d prepost_ns.cpp)
ADD_EXECUTABLE( vis_ns vis_ns.cpp)
ADD_EXECUTABLE( vis_p1_wss vis_p1_wss.cpp)
ADD_EXECUTABLE( vis_p2_wss vis_p2_wss.cpp)

TARGET_LINK_LIBRARIES( gmsh_p1_IO perigee_preprocess )
TARGET_LINK_LIBRARIES( gmsh_p2_IO perigee_preprocess )
TARGET_LINK_LIBRARIES( preprocess3d perigee_preprocess )
TARGET_LINK_LIBRARIES( ns3d perigee_analysis )
TARGET_LINK_LIBRARIES( prepost3d perigee_preprocess )
TARGET_LINK_LIBRARIES( vis_ns perigee_postprocess )
TARGET_LINK_LIBRARIES( vis_p1_wss perigee_postprocess )
TARGET_LINK_LIBRARIES( vis_p2_wss perigee_postprocess )

# EOF

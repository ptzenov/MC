# Install script for directory: /home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/StdVector"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/PaStiXSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SparseCholesky"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SparseCore"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/UmfPackSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Core"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/QtAlignedMalloc"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Geometry"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/PardisoSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Cholesky"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Jacobi"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/CholmodSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Sparse"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/IterativeLinearSolvers"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Eigenvalues"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SVD"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SparseLU"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/LU"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/StdDeque"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SparseQR"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/MetisSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SPQRSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/StdList"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/SuperLUSupport"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Householder"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Dense"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/QR"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/Eigen"
    "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/OrderingMethods"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/home/kenny/Documents/workspaces/C++/MC/lib/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()


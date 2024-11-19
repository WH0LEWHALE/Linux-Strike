#//-----------------------------------------------------------------------------
#//	ENGINE.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------
# Possible Arguments used:
# -DUSE_VALVE_HRTF
cmake_minimum_required(VERSION 3.16)
project(engine_client)
set(EPROJ "engine")
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(ESRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")

#This is the non-dedicated engine
include(engine_inc.cmake)
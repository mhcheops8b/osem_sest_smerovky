@echo off
set OSM_VERSION=20040914
make clean
cd ..
rem Cleanup of old files
rm osm_src-%OSM_VERSION%.tar
rm osm_src-%OSM_VERSION%.tar.bz2

tar -cvf osm_src-%OSM_VERSION%.tar osm_src-%OSM_VERSION%/Makefile osm_src-%OSM_VERSION%/arch_src.bat osm_src-%OSM_VERSION%/arch_dat.bat osm_src-%OSM_VERSION%/doc osm_src-%OSM_VERSION%/src osm_src-%OSM_VERSION%/include osm_src-%OSM_VERSION%/work
bzip2 -9 osm_src-%OSM_VERSION%.tar
cd osm_src-%OSM_VERSION%
set OSM_VERSION=

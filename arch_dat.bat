@echo off
set OSM_VERSION=20040914
set DATA_VERSION=20040914
make clean
cd ..
rem Cleanup of old files
rm osm_data-%OSM_VERSION%.tar
rm osm_data-%OSM_VERSION%.tar.bz2

tar -cvf osm_data-%DATA_VERSION%.tar osm_src-%OSM_VERSION%/data
bzip2 -9 osm_data-%DATA_VERSION%.tar
cd osm_src-%OSM_VERSION%
set DATA_VERSION=
set OSM_VERSION=

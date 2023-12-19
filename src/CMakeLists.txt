include_directories(osm ../include)

add_library(bmp1dulm OBJECT)
target_sources(bmp1dulm PRIVATE bmp1dulm.c ../include/bmp1dulm.h)

add_library(bmp2dulm OBJECT)
target_sources(bmp2dulm PRIVATE bmp2dulm.c ../include/bmp2dulm.h)

add_library(cesty OBJECT)
target_sources(cesty PRIVATE cesty.c ../include/cesty.h)

add_library(charpath OBJECT)
target_sources(charpath PRIVATE charpath.c ../include/charpath.h)
target_compile_definitions(charpath PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(chrslovo OBJECT)
target_sources(chrslovo PRIVATE chrslovo.c ../include/chrslovo.h)
target_compile_definitions(chrslovo PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(find OBJECT)
target_sources(find PRIVATE find.c ../include/find.h)

add_library(findsyl OBJECT)
target_sources(findsyl PRIVATE findsyl.c ../include/findsyl.h)

add_library(flosm OBJECT)
target_sources(flosm PRIVATE flosm.c ../include/flosm.h)

add_library(flosmsyl OBJECT)
target_sources(flosmsyl PRIVATE flosmsyl.c ../include/flosmsyl.h)

add_library(lustikrz OBJECT)
target_sources(lustikrz PRIVATE lustikrz.c ../include/lustikrz.h)
target_compile_definitions(lustikrz PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(num_path OBJECT)
target_sources(num_path PRIVATE num_path.c ../include/num_path.h)
target_compile_definitions(num_path PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(obrfile OBJECT)
target_sources(obrfile PRIVATE obrfile.c ../include/obrfile.h)
target_compile_definitions(obrfile PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(osm_ries OBJECT)
target_sources(osm_ries PRIVATE osm_ries.c ../include/osm_ries.h)
target_compile_definitions(osm_ries PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(osm_size OBJECT)
target_sources(osm_size PRIVATE osm_size.c ../include/osm_size.h)

add_library(osmtop OBJECT)
target_sources(osmtop PRIVATE osmtop.c ../include/osmtop.h)

add_library(pismeno OBJECT)
target_sources(pismeno PRIVATE pismeno.c ../include/pismeno.h)

add_library(position OBJECT)
target_sources(position PRIVATE position.c ../include/position.h)

add_library(q_part OBJECT)
target_sources(q_part PRIVATE q_part.c ../include/q_part.h)

add_library(q_sect OBJECT)
target_sources(q_sect PRIVATE q_sect.c ../include/q_sect.h)

add_library(search OBJECT)
target_sources(search PRIVATE search.c ../include/search.h)
target_compile_definitions(search PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(slovo OBJECT)
target_sources(slovo PRIVATE slovo.c ../include/slovo.h)

add_library(smery OBJECT)
target_sources(smery PRIVATE smery.c ../include/smery.h)

add_library(specchar OBJECT)
target_sources(specchar PRIVATE specchar.c ../include/specchar.h)

add_library(str_path OBJECT)
target_sources(str_path PRIVATE str_path.c ../include/str_path.h)

add_library(syltop OBJECT)
target_sources(syltop PRIVATE syltop.c ../include/syltop.h)

add_library(table OBJECT)
target_sources(table PRIVATE table.c ../include/table.h)

add_library(wrdlist OBJECT)
target_sources(wrdlist PRIVATE wrdlist.c)

add_executable(osem_smerovky uu_n.c)
target_link_libraries(osem_smerovky PRIVATE bmp1dulm bmp2dulm cesty charpath 
	chrslovo find findsyl flosm flosmsyl lustikrz num_path
    obrfile osm_ries osm_size osmtop pismeno position q_part q_sect
	search slovo smery specchar str_path syltop table wrdlist)

add_executable(sest_smerovky ss_n.c)
target_compile_definitions(sest_smerovky PRIVATE _CRT_SECURE_NO_WARNINGS)
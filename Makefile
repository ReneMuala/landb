lib_lconv_make_dir = "./source/lanconv/"
lib_lconv_make_obj_file = "./source/lanconv/lanconv.o"
llmd_back_path = "../../"
lib_lconv_make_flags = "build_lib"

lib_lconv_obj_file = "./source/lanconv/lanconv.o"

lib_src_file = "./source/landb.cpp"
lib_obj_file = "landb.o"
lib_obj_standalone_file = "landb_sa.o"
lib_lconv_obj_file = "lanconv.o"

compiler_flags = "-std=c++17"

all: build_lib_lconv copy_lib_lconv build_landb merge_libs message

build_lib_lconv:
	@echo "- building dependece: lanconv"
	@cd $(lib_lconv_make_dir) && $(MAKE) $(lib_lconv_make_flags)

copy_lib_lconv:
	@echo "- moving $(lib_lconv_make_obj_file)"
	@mv $(lib_lconv_make_obj_file) ./
	@echo "done."

build_landb:
	@echo "- compiling landb.cpp"
	@g++ -c $(lib_src_file) -o $(lib_obj_file) $(compiler_flags)
	@echo "done."

merge_libs:
	@echo "- merging: standalone version"
	@ld -r $(lib_lconv_obj_file) $(lib_obj_file) -o $(lib_obj_standalone_file)
	@echo "done."
 
message:
	@echo "\nhint: You can statically link your program with landb by following this example:"
	@echo "\t $$ g++ yourfile.cpp $(lib_obj_file) $(lib_lconv_obj_file) #normal mode: recommended for muitliple file projects"
	@echo "\t\tor"
	@echo "\t $$ g++ yourfile.cpp $(lib_obj_standalone_file) #standalone mode: recommended for single file projects"

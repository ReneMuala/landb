example_src = "main.cpp"
example_exec = "example.out"
liblandb_obj = "../landb_sa.o"

all: warn build message

warn:
	@echo "Warn: To avoid errors compile \"landb\" first, by typing \"make\" in it's directory (\"../\")"

build: 
	@echo "- compiling $(example_src)"
	@g++ $(example_src) $(liblandb_obj) -o $(example_exec)
	@echo "done."

message:
	@echo "Hint: Test the example by typing \"./$(example_exec)\""

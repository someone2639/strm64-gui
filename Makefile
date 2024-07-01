default: all

build/:
	cmake -B build -S .

all: | build/
	$(MAKE) -C build

clean:
	rm -rf build

export:
	cp -R build/GDExtensionTemplate/* project/demo/bin/


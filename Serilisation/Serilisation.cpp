#include <iostream>
#include <string>

struct TestData {
	int i;
	char c;
	char s[20];
	
	std::string write() {
		return "Test Write " + s;
	}
};

template<typename T>
void write(T& data, const char* path) {
	
	// if T::write is implemented, call that, otherwise filedump
	
	static_assert(std::is_standard_layout<T>::value, "Data type is too complex");
	
	FILE* f;
	fopen_s(&f, path, "wb");
	if (f == NULL)
		return;

	fwrite(&data, sizeof(T), 1, f);

	fclose(f);
}

template<typename T>
void read(T& data, const char* path) {
	
	// if T::read is implemented, call that, otherwise filedump
	
	static_assert(std::is_standard_layout<T>::value, "Data type is too complex");
	
	FILE* f;
	fopen_s(&f, path, "rb");
	if (f == NULL)
		return;

	fread(&data, sizeof(T), 1, f);

	fclose(f);
}

int main()
{
	const char* path = "test.dat";

	TestData TDout{ 1, 'H', "This is a Test" };
	write(TDout, path);

	TestData TDin;
	read(TDin, path);
}
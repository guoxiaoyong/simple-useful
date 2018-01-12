
main: main.cc person.pb.cc person.pb.h
	g++ -std=c++11 -I$(HOME)/.local/include -L$(HOME)/.local/lib -lprotobuf -o $@ $^

person.pb.h person.pb.cc: person.proto
	protoc -I=. --cpp_out=. $^

clean:
	$(RM) main *.pb.*



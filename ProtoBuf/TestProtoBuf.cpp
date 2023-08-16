#include "TestProtoBuf.h"

#include <iostream>
#include <string>
#include "ProtoMessages.pb.h"

int TestProtoBuf(int argc, char* argv[])
{
	ProtoMessages::Person Person;
	ProtoMessages::Person DecodePerson;
	Person.set_age(38);
	Person.set_name("zhangwen");

	std::string buffer;

	Person.SerializeToString(&buffer);

	DecodePerson.ParseFromArray(buffer.c_str(), (int)buffer.size());

	std::cout << "name:" << DecodePerson.name() << "\tage:" << DecodePerson.age() << std::endl;

	return 0;
}
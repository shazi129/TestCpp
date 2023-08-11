#include <iostream>

using namespace std;

class FPaintArgs
{
public:
	FPaintArgs()
	{
		thisPointer = this;
	}

	FPaintArgs WithViewportGeometry(const string& name) const
	{
		FPaintArgs Args(*this);
		Args.name = name;
		return Args;
	}

	void SetName(const string& value)
	{
		name = value;
	}

	string name;

	FPaintArgs* thisPointer;
};

void TestFunction(const FPaintArgs& Args)
{
	//Args.WithViewportGeometry("zhangsan");

//	Args.name = "zhangsan";
	if (Args.thisPointer != nullptr)
	{
		Args.thisPointer->SetName("zhangsan");
	}
}

int Tmain(int argc, char** argv)
{
	FPaintArgs Args;
	Args.thisPointer = &Args;
	Args.name = "lisi";
	cout << Args.name << endl;

	TestFunction(Args);

	cout << Args.name << endl;

	return 0;
}
#include <iostream>
#include <BeastEngine/BeastEngine.h>

int main()
{	
	const auto engine = be::BeastEngine(be::EngineConfig());
	std::cout << engine.SayHello();
	std::getchar();

	return 0;
}

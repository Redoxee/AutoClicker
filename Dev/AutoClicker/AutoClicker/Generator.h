#pragma once
namespace AutoClicker
{
	class GeneratorDefinition
	{
	public:
		long BasePrice;
		long PriceRate;

		long BaseRate;
	};

	class Generator
	{
	public:
		const GeneratorDefinition* Definition = nullptr;
		long InstanceBought = 0;
		
		long Price();
	};
}
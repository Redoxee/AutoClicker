#pragma once

#include <iostream>
namespace AutoClicker
{
	struct Data
	{
	public:
		long Score;
		long PassiveSpeed;
		long TickCount;
		long TargetScore;
		long ClickValue;

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.TickCount = this->TickCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;
		}
	};

	class AutoClicker
	{
	public:
		AutoClicker();
		~AutoClicker();

		void Tick();
		void Click();
		bool IsOver();

		void GetData(Data& d) const
		{
			this->data.CopyTo(d);
		}

		friend std::ostream& operator<<(std::ostream&, const AutoClicker* c);
	private:
		Data data;
	};
}
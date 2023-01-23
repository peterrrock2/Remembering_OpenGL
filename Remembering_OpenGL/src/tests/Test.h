#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional> // get action to the function class

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test" << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

		template<typename T>
		void RegisterTest(const std::string& name, int W, int H) //allows you to pass the width and height of the window
		{
			std::cout << "Registering test" << name << std::endl;
		
			m_Tests.push_back(std::make_pair(name, [W,H]() {return new T(W, H); }));
		}
	private:
		Test*& m_CurrentTest; // refference to current test pointer since wemight want to change test
		// string is to give imgui button a label
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests; // function makes it so I make a new test every time I click the button
	};
}
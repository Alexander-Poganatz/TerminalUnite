
#ifndef OBSERVER_PATTERN_HPP
#define OBSERVER_PATTERN_HPP

/**
	@file ObserverPattern.hpp
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-15
	@brief Holds generic observer pattern stuff
	@note I'm not going to comment this file anymore since it is obvious to most programmers
*/

#include <vector>

namespace ca_poganatz 
{
	struct Observer 
	{
		virtual void update() = 0;
	};

	class Subject 
	{
	private:
		std::vector<Observer*> observerList;
	public:
		inline void attach(Observer* obs) { observerList.push_back(obs); }

		void detach(Observer* obs)
		{
			auto iter = observerList.begin();
			while (iter != observerList.end() && *iter != obs)
				++iter;
			observerList.erase(iter);
		}
		void notify() { for(auto o : observerList) { o->update(); } }
	};


	template <class T>
	class TSubject : public Subject
	{
	private:
		T state;
	public:
		TSubject(T defaultValue) : state(defaultValue) {}

		inline T getState() const { return state; }
		inline void setState(T && newState) { std::swap(this->state, newState); notify(); }
		inline void setState(T const& newState) { this->state = newState; notify(); }
	};
}

#endif
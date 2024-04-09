#pragma once
#include <unordered_map>

template<class... agruments>
class Action
{
public:
	template<typename callable>
	void addSubscriber(const std::string &tag, callable&& f)
	{
		if (subscribers_.find(tag) != subscribers_.end())
		{
			return;
		}
		subscribers_.insert(std::make_pair(tag, f));
	};

	void removeSubscriber(const std::string &tag)
	{
		if (subscribers_.find(tag) != subscribers_.end())
		{
			subscribers_.erase(tag);
		}
	}

	void operator() (agruments&&...args) const
	{
		invoke(std::forward<agruments...>(args)...);
	}

	void invoke(agruments&&...args) const
	{
		for (const auto &subscriber : subscribers_)
		{
			subscriber.second(std::forward<agruments...>(args)...);
		}
	}

private:
	std::unordered_map<std::string, std::function<void(agruments...)>> subscribers_;
};


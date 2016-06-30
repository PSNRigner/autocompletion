//
// autocompletion.cpp for  in /home/frasse_l/Projets/autocompletion
// 
// Made by loic frasse-mathon
// Login   <frasse_l@epitech.net>
// 
// Started on  Thu Jun 30 09:43:32 2016 loic frasse-mathon
// Last update Thu Jun 30 10:58:15 2016 loic frasse-mathon
//

#include "autocompletion.hh"

ac::AutoCompletion::AutoCompletion()
{
}

ac::AutoCompletion::~AutoCompletion()
{
}

void	ac::AutoCompletion::addAddress(const std::string &name, const std::string &address)
{
  std::string				lower = name;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
  std::vector<ac::City *>::iterator	it = this->cities.begin();
  std::vector<ac::City *>::iterator	it_end = this->cities.end();
  ac::City				*city = NULL;

  while (it != it_end)
    {
      std::string tmp = (*it)->getName();
      std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
      if (tmp == lower)
	city = (*it);
      it++;
    }
  if (city == NULL)
    {
      city = new City(name);
      this->cities.push_back(city);
    }
  city->addAddress(address);
}

const std::vector<ac::City *>	&ac::AutoCompletion::getCities() const
{
  return this->cities;
}

ac::City::City(const std::string &name)
  : name(name)
{
}

ac::City::~City()
{
}

void	ac::City::addAddress(const std::string &address)
{
  this->addresses.push_back(address);
}

const std::string	&ac::City::getName() const
{
  return this->name;
}

const std::vector<std::string>	&ac::City::getAddresses() const
{
  return this->addresses;
}

//
// main.cpp for  in /home/frasse_l/Projets/autocompletion
// 
// Made by loic frasse-mathon
// Login   <frasse_l@epitech.net>
// 
// Started on  Thu Jun 30 09:16:46 2016 loic frasse-mathon
// Last update Fri Jul  1 12:08:17 2016 loic frasse-mathon
//

#include "autocompletion.hh"

static bool	isNumber(const std::string &string)
{
  size_t	i;

  i = 0;
  while (i < string.length())
    {
      if (string[i] < '0' || string[i] > '9')
	return false;
      i++;
    }
  return true;
}

static bool	isStreetType(const std::string &arg)
{
  std::string	string = arg;
  AC_TO_LOWER(string);
  return string == "allée" || string == "avenue" || string == "boulevard" || string == "chemin"
    || string == "impasse" || string == "place" || string == "quai" || string == "rue" || string == "square";
}

static void	usage()
{
  std::cout << "USAGE" << std::endl;
  std::cout << "\t./autocompletion dictionnary" << std::endl << std::endl;
  std::cout << "DESCRIPTION" << std::endl;
  std::cout << "\tdictionnary file, containing one address per line, serving as knowledge base" << std::endl;
  exit(0);
}

static void	bad_arg()
{
  std::cerr << "Invalid argument" << std::endl;
  exit(84);
}

static void	no_address()
{
  std::cout << "Unknown address" << std::endl;
  exit(84);
}

static void	add_dictionary(ac::AutoCompletion &autoCompletion, char *path)
{
  std::ifstream	stream;
  std::string	line;

  stream.open(path);
  if (!stream)
    bad_arg();

  while (std::getline(stream, line))
    {
      bool		before = true;
      bool		ok = true;
      size_t		i = 0;
      std::string	name;
      std::string       address;
      while (i < line.length() && ok)
	{
	  if (line[i] == ',' && before)
	    before = false;
	  else if (line[i] == ',')
	    ok = false;
	  else if (before)
	    name.push_back(line[i]);
	  else
	    address.push_back(line[i]);
	  i++;
	}
      std::vector<std::string>	split;
      std::istringstream	iss(address);
      std::string		tmp;
      while (std::getline(iss, tmp, ' '))
	split.push_back(tmp);
      if (!ok || name.empty() || address.empty() || address[0] != ' ' || split.size() < 4 || !isNumber(split[1])
	  || !isStreetType(split[2]))
	std::cerr << line << std::endl;
      else
	autoCompletion.addAddress(name, address.substr(1, address.length() - 1));
    }
}

template<typename T>
static void	removeAt(std::vector<T> &vector, size_t index)
{
  typename std::vector<T>::iterator	it = vector.begin();
  typename std::vector<T>::iterator	it_end = vector.end();
  while (it != it_end && index > 0)
    {
      it++;
      index--;
    }
  vector.erase(it);
}

static void	putOrIncrement(std::map<std::string, int> &map, const std::string &key, int count)
{
  std::map<std::string, int>::iterator	it = map.begin();
  std::map<std::string, int>::iterator	it_end = map.end();
  while (it != it_end)
    {
      if (it->first == key)
	{
	  it->second = count + it->second;
	  return ;
	}
      it++;
    }
  map.insert(std::pair<std::string, int>(key, count));
}

static bool	_sort(std::pair<std::string, int> pair1, std::pair<std::string, int> pair2)
{
  if (pair1.second == pair2.second)
    return pair1.first < pair2.first;
  return pair1.second > pair2.second;
}

static std::vector<std::string>			sort(const std::map<std::string, int> map)
{
  std::vector<std::pair<std::string, int> >	list;
  std::vector<std::string>			sorted;
  std::map<std::string, int>::const_iterator	it = map.begin();
  std::map<std::string, int>::const_iterator	it_end = map.end();
  while (it != it_end)
    {
      list.push_back(std::pair<std::string, int>(it->first, it->second));
      it++;
    }
  std::sort(list.begin(), list.end(), _sort);
  std::vector<std::pair<std::string, int> >::const_iterator	it2 = list.begin();
  std::vector<std::pair<std::string, int> >::const_iterator	it2_end = list.end();
  while (it2 != it2_end)
    {
      sorted.push_back(it2->first);
      it2++;
    }
  return sorted;
}

static std::string	format(const std::string &string, const std::string &name)
{
  std::string		tmp = string;
  std::string		tmp2 = name;
  size_t		index;
  AC_TO_LOWER(tmp);
  AC_TO_LOWER(tmp2);
  while ((index = tmp.find(tmp2)) != std::string::npos)
    tmp.replace(index, name.length(), name);
  return tmp;
}

static void	complete(std::vector<ac::City *> &choices, std::string &name, std::string &address, bool &first,
			 int selection)
{
  static	std::vector<std::string> selections;

  if (choices.size() > 1)
    {
      size_t				i = 0;
      std::map<std::string, int>	map;
      while (i < choices.size())
	{
	  std::string		tmp = choices[i]->getName();
	  AC_TO_UPPER(tmp);
	  bool			ok = false;
	  std::istringstream	iss(tmp);
	  std::string		line;
	  while (std::getline(iss, line, ' '))
	    {
	      if (line.find(name) == 0)
		{
		  ok = true;
		  putOrIncrement(map, name.length() == line.length() ? name :
				 name + (char)std::tolower(line[name.length()]), choices[i]->getAddresses().size());
		}
	    }
	  if (!ok)
	    {
	      removeAt(choices, i);
	      i--;
	    }
	  i++;
	}
      if (selections.empty())
	{
	  std::vector<std::string> sorted = sort(map);
	  if (sorted.size() == 1)
	    {
	      if (name == sorted[0])
		{
		  i = 0;
		  map.clear();
		  while (i < choices.size())
		    {
		      std::string tmp = choices[i]->getName();
		      AC_TO_LOWER(tmp);
		      putOrIncrement(map, tmp, choices[i]->getAddresses().size());
		      i++;
		    }
		  std::vector<std::string> sorted2 = sort(map);
		  i = 0;
		  while (i < sorted2.size())
		    {
		      std::string tmp = format(sorted2[i], name);
		      selections.push_back(tmp);
		      if (i != 0)
			std::cout << " ";
		      std::cout << "{" << i + 1 << " : " << tmp << "}";
		      i++;
		    }
		  std::cout << std::endl;
		}
	      else
		{
		  name = sorted[0];
		  AC_TO_UPPER(name);
		  complete(choices, name, address, first, selection);
		  return ;
		}
	    }
	  else if (sorted.size() > 1)
	    {
	      std::vector<std::string>::iterator	it = sorted.begin();
	      std::vector<std::string>::iterator	it_end = sorted.end();
	      int					j = 0;
	      while (it != it_end && j < 5)
		{
		  if (j != 0)
		    std::cout << " ";
		  std::cout << "{" << *it << "}";
		  j++;
		  it++;
		}
	      std::cout << std::endl;
	    }
	}
      else
	{
	  if (selection < 0 || (size_t)selection >= selections.size())
	    no_address();
	  std::string tmp = selections[selection];
	  selections.clear();
	  AC_TO_UPPER(tmp);
	  name = tmp;
	  while (choices.size() != 1)
	    {
	      std::string tmp = choices[0]->getName();
	      AC_TO_UPPER(tmp);
	      if (tmp != name)
		removeAt(choices, 0);
	      else
		{
		  tmp = choices[1]->getName();
		  AC_TO_UPPER(tmp);
		  if (tmp != name)
		    removeAt(choices, 1);
		}
	    }
	}
    }
  if (choices.size() == 0)
    no_address();
  if (choices.size() == 1)
    {
      first = false;
      name = choices[0]->getName();
      AC_TO_UPPER(name);
      if (choices[0]->getAddresses().size() > 1)
	{
	  size_t			i = 0;
	  std::map<std::string, int>	map;
	  while (i < choices[0]->getAddresses().size())
	    {
	      std::string		tmp = choices[0]->getAddresses()[i];
	      AC_TO_UPPER(tmp);
	      bool			ok = false;
	      std::istringstream	iss(tmp);
	      std::string		line;
	      int			j = 0;
	      while (std::getline(iss, line, ' '))
		{
		  if (j > 1 && line.find(address) == 0)
		    {
		      ok = true;
		      putOrIncrement(map, address.length() == line.length() ? address :
				     address + (char)std::tolower(line[address.length()]), 1);
		    }
		  j++;
		}
	      if (!ok)
		{
		  removeAt(choices[0]->getAddresses(), i);
		  i--;
		}
	      i++;
	    }
	  if (selections.empty())
	    {
	      std::vector<std::string> sorted = sort(map);
	      if (sorted.size() == 1)
		{
		  if (address == sorted[0])
		    {
		      i = 0;
		      map.clear();
		      while (i < choices[0]->getAddresses().size())
			{
			  std::string tmp = choices[0]->getAddresses()[i];
			  AC_TO_LOWER(tmp);
			  putOrIncrement(map, tmp, 1);
			  i++;
			}
		      std::vector<std::string> sorted2 = sort(map);
		      i = 0;
		      while (i < sorted2.size())
			{
			  std::string tmp = format(sorted2[i], address);
			  selections.push_back(tmp);
			  if (i != 0)
			    std::cout << " ";
			  std::cout << "{" << i + 1 << " : " << name << ", " << tmp << "}";
			  i++;
			}
		      std::cout << std::endl;
		    }
		  else
		    {
		      address = sorted[0];
		      AC_TO_UPPER(address);
		      complete(choices, name, address, first, selection);
		      return ;
		    }
		}
	      else if (sorted.size() > 1)
		{
		  std::vector<std::string>::iterator	it = sorted.begin();
		  std::vector<std::string>::iterator	it_end = sorted.end();
		  int					j = 0;
		  while (it != it_end && j < 5)
		    {
		      if (j != 0)
			std::cout << " ";
		      std::cout << "{" << name << ", " << *it << "}";
		      j++;
		      it++;
		    }
		  std::cout << std::endl;
		}
	    }
	  else
	    {
	      if (selection < 0 || (size_t)selection >= selections.size())
		no_address();
	      std::string tmp = selections[selection];
	      selections.clear();
	      AC_TO_UPPER(tmp);
	      address = tmp;
	      while (choices[0]->getAddresses().size() != 1)
		{
		  std::string tmp = choices[0]->getAddresses()[0];
		  AC_TO_UPPER(tmp);
		  if (tmp != address)
		    removeAt(choices[0]->getAddresses(), 0);
		  else
		    {
		      tmp = choices[0]->getAddresses()[1];
		      AC_TO_UPPER(tmp);
		      if (tmp != address)
			removeAt(choices[0]->getAddresses(), 1);
		    }
		}
	    }
	}
      if (choices[0]->getAddresses().size() == 0)
	no_address();
      if (choices[0]->getAddresses().size() == 1)
	{
	  std::cout << "=> " << choices[0]->getName() << ", " << choices[0]->getAddresses()[0] << std::endl;
	  exit(0);
	}
    }
}

static void	run(ac::AutoCompletion &autoCompletion)
{
  std::vector<ac::City *>	choices = autoCompletion.getCities();
  std::string			name;
  std::string			address;
  std::string			line;
  bool				first = true;
  int				selection = -1;
  while (true)
    {
      complete(choices, name, address, first, selection);
      if (!std::getline(std::cin, line))
	return ;
      AC_TO_UPPER(line);
      if (line == "ABORT")
	exit(0);
      selection = -1;
      if (!line.empty() && line[0] > '0' && line[0] < '6')
	selection = line[0] - 49;
      else
	(first ? name : address).append(line);
    }
}

int			main(int ac, char **av)
{
  ac::AutoCompletion	autoCompletion;
  int			i;

  i = 1;
  if (ac == 1)
    bad_arg();
  while (i < ac)
    {
      if (std::string("-h") == av[i])
	usage();
      if (i != 1)
	bad_arg();
      add_dictionary(autoCompletion, av[i]);
      i++;
    }
  run(autoCompletion);
  return (0);
}

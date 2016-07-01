//
// main.cpp for  in /home/frasse_l/Projets/autocompletion
// 
// Made by loic frasse-mathon
// Login   <frasse_l@epitech.net>
// 
// Started on  Thu Jun 30 09:16:46 2016 loic frasse-mathon
// Last update Fri Jul  1 17:25:37 2016 loic frasse-mathon
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
  std::cerr << "Unknown address" << std::endl;
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

static bool	equals(const std::string &str1, const std::string &str2)
{
  size_t	i = 0;
  size_t	j = 0;
  while (i < str1.length() && j < str2.length())
    {
      while (i < str1.length() && (str1[i] == '-' || str1[i] == '\''))
	i++;
      while (j < str2.length() && (str2[j] == '-' || str2[j] == '\''))
	j++;
      if (i >= str1.length() || j >= str2.length() || str1[i] != str2[j])
	return false;
      i++;
      j++;
    }
  return i == str1.length() && j == str2.length();
}

static void	putOrIncrement(std::map<std::string, int> &map, const std::string &key, int count)
{
  std::map<std::string, int>::iterator	it = map.begin();
  std::map<std::string, int>::iterator	it_end = map.end();
  while (it != it_end)
    {
      if (equals(it->first, key))
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

static bool	_sort_addresses(std::pair<std::string, int> pair1, std::pair<std::string, int> pair2)
{
  if (pair1.second == pair2.second)
    {
      std::istringstream	iss1(pair1.first);
      std::istringstream	iss2(pair2.first);
      std::string		word;
      std::vector<std::string>	vec1;
      std::vector<std::string>	vec2;
      while (std::getline(iss1, word, ' '))
	vec1.push_back(word);
      while (std::getline(iss2, word, ' '))
	vec2.push_back(word);
      size_t			i = 2;
      while (i < vec1.size() && i < vec2.size())
	{
	    if (vec1[i] < vec2[i])
	      return true;
	    i++;
	}
      return false;
    }
  return pair1.second > pair2.second;
}

static std::vector<std::string>			sort(const std::map<std::string, int> map, bool first)
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
  std::sort(list.begin(), list.end(), first ? _sort : _sort_addresses);
  std::vector<std::pair<std::string, int> >::const_iterator	it2 = list.begin();
  std::vector<std::pair<std::string, int> >::const_iterator	it2_end = list.end();
  while (it2 != it2_end)
    {
      sorted.push_back(it2->first);
      it2++;
    }
  return sorted;
}

static bool		my_find(const std::string &string, const std::string &substring, size_t &start, size_t &end)
{
  size_t		i;
  size_t		j;

  i = 0;
  j = 0;
  start = -1;
  end = -1;
  while (i < string.length() && j < substring.length())
    {
      if (substring[j] == '-' || substring[j] == '\'')
	{
	  j++;
	  i--;
	}
      else if (substring[j] == string[i])
	{
	  if (start == std::string::npos)
	    start = i;
	  j++;
	}
      else if (string[i] != '\'' && string[i] != '-')
	{
	  start = std::string::npos;
	  j = 0;
	}
      i++;
    }
  if (j == substring.length())
    end = i;
  if (j == 0 && substring.length() == j)
    start = 0;
  return j == substring.length();
}

static std::string	format(const std::string &string, const std::string &name)
{
  std::string		tmp = string;
  std::string		tmp2 = name;
  size_t		index;
  size_t		length;
  AC_TO_LOWER(tmp);
  AC_TO_LOWER(tmp2);
  while (my_find(tmp, tmp2, index, length))
    {
      while (index < length)
	{
	  tmp[index] = (char)std::toupper(tmp[index]);
	  index++;
	}
    }
  return tmp;
}

static std::string	next(const std::string &name, const std::string &line)
{
  size_t		i = 0;
  std::string		sub = line;
  AC_TO_UPPER(sub);
  size_t		j = 0;
  size_t		n = 0;
  while (j < line.length() && (line[j] == '-' || line[j] == '\'' || line[j] == name[i] || name[i] == '-' || name[i] == '\''))
    {
      i++;
      j++;
      while (i < name.length() && (name[i] == '-' || name[i] == '\''))
	{
	  i++;
	  n--;
	}
      while (j < line.length() && (line[j] == '-' || line[j] == '\''))
	{
	  j++;
	  n++;
	}
    }
  while (j < line.length() && (line[j] == '-' || line[j] == '\''))
    {
      j++;
      n++;
    }
  sub = sub.substr(0, j);
  i += n;
  while (i < line.length())
    {
      if (line[i] != '-' && line[i] != '\'')
	return sub + (char)std::tolower(line[i]);
      i++;
    }
  return sub;
}

static void	complete(std::vector<ac::City *> &choices, std::string &name, std::string &address, bool &first,
			 int selection)
{
  static	std::vector<std::string> selections;
  size_t	index;
  size_t	end;

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
	      if (my_find(line, name, index, end) && index == 0)
		{
		  ok = true;
		  putOrIncrement(map, next(name, line), choices[i]->getAddresses().size());
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
	  std::vector<std::string> sorted = sort(map, true);
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
		  std::vector<std::string> sorted2 = sort(map, true);
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
		  if (j > 1 && my_find(line, address, index, end) && index == 0)
		    {
		      ok = true;
		      putOrIncrement(map, next(address, line), 1);
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
	      std::vector<std::string> sorted = sort(map, true);
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
		      std::vector<std::string> sorted2 = sort(map, false);
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

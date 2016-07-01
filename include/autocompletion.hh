/*
** autocompletion.h for  in /home/frasse_l/Projets/autocompletion
** 
** Made by loic frasse-mathon
** Login   <frasse_l@epitech.net>
** 
** Started on  Thu Jun 30 09:16:21 2016 loic frasse-mathon
// Last update Fri Jul  1 09:47:49 2016 loic frasse-mathon
*/

#ifndef AUTOCOMPLETION_HH_
# define AUTOCOMPLETION_HH_

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <algorithm>

# define	AC_TO_UPPER(str)	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
# define	AC_TO_LOWER(str)	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

namespace	ac
{
  class		City
  {
  public:
    City(const std::string &);
    ~City();
    void				addAddress(const std::string &);
    const std::string			&getName() const;
    std::vector<std::string>		&getAddresses();
  private:
    std::string				name;
    std::vector<std::string>		addresses;
  };
  
  class		AutoCompletion
  {
  public:
    AutoCompletion();
    ~AutoCompletion();
    void				addAddress(const std::string &, const std::string &);
    const std::vector<ac::City *>	&getCities() const;
  private:
    std::vector<ac::City *>		cities;
  };
}
#endif /* !AUTOCOMPLETION_HH_ */

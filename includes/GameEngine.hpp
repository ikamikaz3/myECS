//
// GameEngine.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Fri Nov 25 15:08:25 2016 Thomas Billot
// Last update Fri Jan  6 13:59:50 2017 Thomas Billot
//

#ifndef			_GAMEENGINE_HPP_
# define		_GAMEENGINE_HPP_

# include		<map>
# include		<vector>
# include		"EntityManager.hpp"
# include		"SystemManager.hpp"
# include		"ASystem.hpp"
# include		"MessageBus.hpp"
# include		"IDynamicLoader.hpp"

class					GameEngine
{

public:

  explicit				GameEngine(const std::string &libsDir);
  ~GameEngine();

  void					loadLib(const std::string &libPath);
  int					run(void);
  void					update(void);
  void					postImmediateMessage(int messageId, void *data);

  explicit				GameEngine(const GameEngine &other) = delete;
  GameEngine				operator=(const GameEngine &other) = delete;

private:

  MessageBus				_messageBus;  
  EntityManager				_entityManager;
  SystemManager				_systemManager;
  std::unique_ptr<IDynamicLoader>	_libLoader;  
};

#endif			/* _GAMEENGINE_HPP_ */

//
// SystemManager.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Tue Dec 13 13:15:36 2016 Thomas Billot
// Last update Tue Dec 27 10:31:06 2016 Thomas Billot
//

#pragma once

# include		<memory>
# include		<map>
# include		"ASystem.hpp"
# include		"EntityManager.hpp"

class			EntityManager;

class			SystemManager
{

  EntityManager						&_entityManager;
  MessageBus						&_messageBus;
  std::map<std::string, std::shared_ptr<ASystem> >	_systems;
  
public:

  SystemManager(EntityManager &entityManager, MessageBus &messageBus);
  ~SystemManager();

  bool			addSystem(const std::shared_ptr<ASystem> &systemToAdd,
				  const std::string &name,
				  const std::vector<std::string> &affectedComponents,
				  const std::vector<int> &suscribedToMessages);
  bool			removeSystem(const std::string &systemToRemove);
  void		        debugSystem();
  void		        updateSystems();

  SystemManager(const SystemManager &) = delete;
  SystemManager		&operator=(const SystemManager &) = delete;
  
};

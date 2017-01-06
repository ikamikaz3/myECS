//
// SystemManager.cpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Tue Dec 13 13:18:17 2016 Thomas Billot
// Last update Thu Dec 29 16:30:07 2016 Thomas Billot
//

#include	<iostream>
#include	<memory>
#include	"SystemManager.hpp"

SystemManager::SystemManager(EntityManager &entityManager, MessageBus &messageBus)
  : _entityManager(entityManager),
    _messageBus(messageBus),
    _systems()
{}

SystemManager::~SystemManager() {}

bool		SystemManager::addSystem(const std::shared_ptr<ASystem> &systemToAdd,
					 const std::string &name,
					 const std::vector<std::string> &affectedComponents,
					 const std::vector<int> &subscribedToMessages)
{
  if (systemToAdd && !name.empty() && !affectedComponents.empty())
    {
      systemToAdd->addName(name);
      if (!affectedComponents.empty())
	systemToAdd->addAffectedComponents(affectedComponents);
      _messageBus.registerSystem(systemToAdd);
      _messageBus.subscribeToMessage(name, MessageBus::ENTITY_CREATED);
      _messageBus.subscribeToMessage(name, MessageBus::ENTITY_DESTROYED);
      _messageBus.subscribeToMessage(name, MessageBus::ENTITY_COMPOSITION_CHANGED);
      for (int messageId: subscribedToMessages)
      	{
      	  _messageBus.subscribeToMessage(name, messageId);
      	}
      _systems.emplace(systemToAdd->name(), std::move(systemToAdd));
      return true;
    }
  throw std::logic_error("addSystem failed !");
}

bool		SystemManager::removeSystem(const std::string &systemToRemove)
{
  auto found = _systems.find(systemToRemove);

  if (found != _systems.end())
    {
      _systems.erase(found);
      return true;
    }
  throw std::logic_error("removeSystem failed");
}

void		SystemManager::updateSystems()
{
  for (std::map<std::string, std::shared_ptr<ASystem> >::const_iterator it = _systems.begin();
       it != _systems.end();
       ++it)
    {
      // std::cout << "SystemManager::updating " << it->second->name() << std::endl;
      it->second->update();
      _messageBus.broadcast();
    }
}

void		SystemManager::debugSystem()
{
  for (std::map<std::string, std::shared_ptr<ASystem> >::const_iterator it = _systems.begin();
       it != _systems.end();
       ++it)
    {
      //std::cout << it->second->name() << std::endl;
    }
}

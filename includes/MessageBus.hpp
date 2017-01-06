//
// MessageBus.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/cpp_rtype/ECS
//
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
//
// Started on  Wed Dec 21 18:24:17 2016 Thomas Billot
// Last update Tue Dec 27 11:00:06 2016 Thomas Billot
//

#pragma once

#include		<vector>
#include	        <deque>
#include		<memory>
#include		<map>
#include		"ASystem.hpp"

class ASystem;

class					MessageBus
{
  class					Message
  {
    int					_typeId;
    void				*_data;

  public:
    Message(int typeId, void *data);
    ~Message();
    int					getTypeId();
    void				*getData();
  };

  std::vector<int>			_validMessages;
  std::multimap<int, std::string>	_subscribedList;
  std::vector<Message>			_messages;
  std::vector<std::shared_ptr<ASystem>>	_systems;

public:

  enum					standardMessages : int
    {
      ENTITY_CREATED			= 0,
      ENTITY_DESTROYED			= 1,
      ENTITY_COMPOSITION_CHANGED	= 2,
      END_OF_STANDARD_MESSAGES		= 10
    };

  explicit MessageBus();
  ~MessageBus();

  void					registerSystem(std::shared_ptr<ASystem> systemToManage);
  void					registerValidMessageId(int messageId);
  void					removeSystem(const std::string &name);
  void					post(const int messageTypeId,
					     void *messageData);
  void					broadcast();
  void					subscribeToMessage(const std::string &system,
								   const int message);
  bool					isSubscribed(const std::string &name,
						     const int message);

private:

  MessageBus(const MessageBus &lhs);
  MessageBus				operator=(const MessageBus &lhs);
};

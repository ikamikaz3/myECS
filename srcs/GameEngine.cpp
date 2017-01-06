//
// GameEngine.cpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
//
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
//
// Started on  Fri Nov 25 17:20:05 2016 Thomas Billot
// Last update Fri Jan  6 14:01:52 2017 Thomas Billot
//

#include <iostream>
#include <chrono>
#if defined(__GNUC__)
#include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
#endif
#include "GameEngine.hpp"

GameEngine::GameEngine(const std::string &libsDir)
  : _messageBus(),
    _entityManager(_messageBus),
    _systemManager(_entityManager, _messageBus)
{
# if		defined(_WIN32) || defined(WIN32)
  std::unique_ptr<IDynamicLoader> libLoader(new windowsDynamicLoader);
  _libLoader = std::move(libLoader);

	  WIN32_FIND_DATA	ffd;
	  LARGE_INTEGER		filesize;
	  TCHAR				szDir[MAX_PATH];
	  size_t			length_of_arg;
	  HANDLE			hFind = INVALID_HANDLE_VALUE;
	  DWORD				dwError = 0;

	  StringCchLength(libsDir.c_str(), MAX_PATH, &length_of_arg);
	  if (length_of_arg > (MAX_PATH - 3))
	  {
		  _tprintf(TEXT("\nDirectory path is too long.\n"));
		  std::exit(42);
	  }
	  _tprintf(TEXT("\nTarget directory is %s\n\n"), libsDir);
	  StringCchCopy(szDir, MAX_PATH, libsDir.c_str());
	  StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	  hFind = FindFirstFile(szDir, &ffd);
	  while (FindNextFile(hFind, &ffd))
	  {
		  if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		  {
			  _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		  }
		  else
		  {
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			if (filesize.QuadPart > 1) {
				_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				std::string s(libsDir.c_str());
				s += ffd.cFileName;
				loadLib(s);
			  }
		  }
	  }
	  FindClose(hFind);

#elif           defined(__GNUC__)
  std::unique_ptr<IDynamicLoader> libLoader(new unixDynamicLoader);

  _libLoader = std::move(libLoader);
  if (!libsDir.empty())
    {
      DIR               *dir;
      struct dirent     *ent;

      if ((dir = opendir(libsDir.c_str())) != NULL)
	{
	  while ((ent = readdir (dir)) != NULL)
	    {
		  std::string s(libsDir.c_str());

		  s += ent->d_name;
		  loadLib(s);
	  }
	  closedir (dir);
	}
    }
#endif
}

void			GameEngine::loadLib(const std::string &libPath)
{
  std::size_t found = libPath.find_last_of(".");
# if		defined(_WIN32) || defined(WIN32)
  if (libPath.substr(found + 1) == "dll")
  {
	  std::cout << "trying to load lib" << std::endl;
	  void(*fPtr)(EntityManager &,
		  SystemManager &,
		  MessageBus &) = _libLoader->load(libPath, "returnLoader");

	  fPtr(_entityManager, _systemManager, _messageBus);
	  std::cout << "succeded in loading lib" << std::endl;
  }
#elif           defined(__GNUC__)
  if (libPath.substr(found + 1) == "so")
  {
	  std::cout << "trying to load lib" << std::endl;
	  void(*fPtr)(EntityManager &,
		  SystemManager &,
		  MessageBus &) = _libLoader->load(libPath, "returnLoader");

	  fPtr(_entityManager, _systemManager, _messageBus);
	  std::cout << "succeded in loading lib" << std::endl;
  }
#endif
}

GameEngine::~GameEngine()
{}

void					GameEngine::postImmediateMessage(int messageId, void *data)
{
  _messageBus.post(messageId, data);
  _messageBus.broadcast();
}


int			GameEngine::run(void)
{
	try {
		bool			run = true;
		std::chrono::time_point<std::chrono::system_clock> now, then;
		then = std::chrono::system_clock::now();

		while (run)
		{
			now = std::chrono::system_clock::now();
			int elapsed =
				std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
			if (elapsed >= 10)
			{
				_systemManager.updateSystems();
				then = now;
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

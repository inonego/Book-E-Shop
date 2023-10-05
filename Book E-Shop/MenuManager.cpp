#include "MenuManager.h"

void MenuManager::CleanUp()
{
}

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
}

void MenuManager::AppendMenu(string menu_name, Menu* menu)
{
}

void MenuManager::RemoveMenu(string menu_name)
{
}

void MenuManager::AppendCommandFunc(char command, function<void()> command_func)
{
}

void MenuManager::RemoveCommandFunc(char command)
{
}

void MenuManager::SetCurrentMenu(string menu_name)
{
}

void MenuManager::ProcessInput(string input)
{
}

void MenuManager::ProcessCommand(char command)
{
}

void MenuManager::RefreshScreen()
{
}

void MenuManager::Quit()
{
}

/*
 * Copyright (c) 2009 David Marques.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "Win32PowerManager.h"

bool Win32PowerManager::relized = false;
bool Win32PowerManager::isWinNT = false;

void Win32PowerManager::realizeWin32Version()
{
	WCHAR buffer[100];

	GetEnvironmentVariable(L"OS", buffer, 100);
	if (wcscmp(L"Windows_NT", buffer) == 0)
		isWinNT = true;
	else
		isWinNT = false;
	relized = true;
}

void Win32PowerManager::EnableShutdownPrivileges(void)
{
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	// Get the current process token handle...
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES
			| TOKEN_QUERY, &token))
		return;

	// Get the LUID for shutdown privilege...
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);

	// Set parameters for AdjustTokenPrivileges...
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Enable shutdown privilege...
	AdjustTokenPrivileges(token, FALSE, &privileges, 0,
			(PTOKEN_PRIVILEGES) NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
		return;

}


void Win32PowerManager::DisableShutdownPrivileges(void)
{

	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	// Get the current process token handle...
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES
			| TOKEN_QUERY, &token))
		return;

	// Get the LUID for shutdown privilege...
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);

	// Set parameters for AdjustTokenPrivileges...
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = 0;

	// Disable shutdown privilege...
	AdjustTokenPrivileges(token, FALSE, &privileges, 0,
			(PTOKEN_PRIVILEGES) NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
		return;

}

void Win32PowerManager::Logoff(void)
{
	if (!relized) realizeWin32Version();
	ExitWindowsEx(EWX_LOGOFF, 0);
}

void Win32PowerManager::Poweroff(void)
{
	if (!relized) realizeWin32Version();
	if (isWinNT)
		EnableShutdownPrivileges();
	ExitWindowsEx(EWX_POWEROFF, 0);
	if (isWinNT)
		DisableShutdownPrivileges();
}

void Win32PowerManager::Restart(void)
{
	if (!relized) realizeWin32Version();
	if (isWinNT)
		EnableShutdownPrivileges();
	ExitWindowsEx(EWX_REBOOT, 0);
	if (isWinNT)
		DisableShutdownPrivileges();
}

void Win32PowerManager::Shutdown(void)
{
	if (!relized) realizeWin32Version();
	if (isWinNT)
		EnableShutdownPrivileges();
	ExitWindowsEx(EWX_SHUTDOWN, 0);
	if (isWinNT)
		DisableShutdownPrivileges();
}

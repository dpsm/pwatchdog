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
#ifndef CLOGOFF_H_
#define CLOGOFF_H_

#include <windows.h>

class Win32PowerManager
{

public:

	static void Logoff(void);
	static void Poweroff(void);
	static void Restart(void);
	static void Shutdown(void);

private:

	static bool relized;
	static bool isWinNT;

	static void EnableShutdownPrivileges(void);
	static void DisableShutdownPrivileges(void);
	static void realizeWin32Version();

};
#endif /* CLOGOFF_H_ */

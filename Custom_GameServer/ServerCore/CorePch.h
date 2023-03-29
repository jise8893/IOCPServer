#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"


#include <windows.h>
#include <iostream>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "Lock.h"

#include "Memory.h"

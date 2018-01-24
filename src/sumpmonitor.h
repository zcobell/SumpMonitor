//------------------------------GPLv3------------------------------------//
//
//  This file is part of SumpMonitor.
//
//  SumpMonitor is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SumpMonitor is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SumpMonitor. If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------GPLv3------------------------------------//
#ifndef SUMPMONITOR
#define SUMPMONITOR

namespace Priority{
    const int Lowest = -2;
    const int Low = -1;
    const int Standard = 0;
    const int High = 1;
    const int Emergency = 2;
};

namespace Emergency{
    const int Retry = 600;
    const int Expire = 86400;
};

#endif //SUMPMONITOR

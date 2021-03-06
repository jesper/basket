/***************************************************************************
 *   Copyright (C) 2006 by Petri Damsten                                   *
 *   damu@iki.fi                                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BASKET_PLUGIN_H
#define BASKET_PLUGIN_H

#include <klocale.h>
#include <kparts/part.h>
#include <kontact/plugin.h>
#include "uniqueapphandler.h"
#include "basketdcopiface_stub.h"

class KAboutData;

class BasketUniqueAppHandler : public Kontact::UniqueAppHandler
{
	public:
		BasketUniqueAppHandler( Kontact::Plugin* plugin ) : Kontact::UniqueAppHandler( plugin ) {}
		virtual void loadCommandLineOptions();
		virtual int newInstance();
};

class BasketPlugin : public Kontact::Plugin
{
  Q_OBJECT

  public:
    BasketPlugin( Kontact::Core *core, const char *name,
                       const QStringList & );
    ~BasketPlugin();

    int weight() const { return 700; }
	virtual bool isRunningStandalone();
	//virtual bool createDCOPInterface( const QString& serviceType );

  public slots:
	void newBasket();
	void showPart();

  protected:
    KParts::ReadOnlyPart *createPart();
	BasketDcopInterface_stub *m_stub;
	Kontact::UniqueAppWatcher *m_uniqueAppWatcher;
};

#endif

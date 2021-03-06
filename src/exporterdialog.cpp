/***************************************************************************
 *   Copyright (C) 2003 by Sébastien Laoût                                 *
 *   slaout@linux62.org                                                    *
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

#include <kurlrequester.h>
#include <klineedit.h>
#include <kfiledialog.h>
#include <qcheckbox.h>
#include <qdir.h>
#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <klocale.h>
#include <kconfig.h>

#include <KVBox>

#include "exporterdialog.h"
#include "basket.h"
#include "global.h"

ExporterDialog::ExporterDialog(Basket *basket, QWidget *parent, const char *name)
     : KDialog(parent)
     , m_basket(basket)
{
	// Dialog options
	setObjectName(name);
	setModal(true);
	setCaption(i18n("Export Basket to HTML"));
	setButtons(Ok | Cancel);
	setDefaultButton(Ok);
	showButtonSeparator(true);
	connect(this, SIGNAL(okClicked()), SLOT(save()));

	KVBox *page  = new KVBox(this);
	QWidget     *wid  = new QWidget(page);
    setMainWidget(wid);
	QHBoxLayout *hLay = new QHBoxLayout(wid, /*margin=*/0, spacingHint());
	m_url = new KUrlRequester(KUrl(""), wid);
	m_url->setCaption(i18n("HTML Page Filename"));
	m_url->setFilter("text/html");
	m_url->fileDialog()->setOperationMode(KFileDialog::Saving);
	hLay->addWidget( new QLabel(m_url, i18n("&Filename:"), wid) );
	hLay->addWidget( m_url );

	m_embedLinkedFiles    = new QCheckBox(i18n("&Embed linked local files"),              page);
	m_embedLinkedFolders  = new QCheckBox(i18n("Embed &linked local folders"),            page);
	m_erasePreviousFiles  = new QCheckBox(i18n("Erase &previous files in target folder"), page);
	m_formatForImpression = new QCheckBox(i18n("For&mat for impression"),                 page);
	m_formatForImpression->hide();

	load();
	m_url->lineEdit()->setFocus();

	//showTile(true);
	// Add a stretch at the bottom:
	// Duplicated code from AddBasketWizard::addStretch(QWidget *parent):
	(new QWidget(page))->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Double the width, because the filename should be visible
	QSize size(sizeHint());
	resize(QSize(size.width() * 2, size.height()));
/*
==========================
+ [00000000000          ] Progress bar!
+ newBasket -> name folder as the basket
*/
}

ExporterDialog::~ExporterDialog()
{
}

void ExporterDialog::show()
{
	KDialog::show();

	QString lineEditText = m_url->lineEdit()->text();
	int selectionStart = lineEditText.findRev("/") + 1;
	m_url->lineEdit()->setSelection(selectionStart, lineEditText.length() - selectionStart - QString(".html").length());
}

void ExporterDialog::load()
{
    KConfigGroup config = Global::config()->group("HTML Export");

	QString folder = config.readEntry("lastFolder", QDir::homePath()) + "/";
	QString url = folder + QString(m_basket->basketName()).replace("/", "_") + ".html";
	m_url->setUrl(KUrl(url));

	m_embedLinkedFiles->setChecked(    config.readEntry("embedLinkedFiles",    true)                      );
	m_embedLinkedFolders->setChecked(  config.readEntry("embedLinkedFolders",  false)                     );
	m_erasePreviousFiles->setChecked(  config.readEntry("erasePreviousFiles",  true)                      );
	m_formatForImpression->setChecked( config.readEntry("formatForImpression", false)                     );
}

void ExporterDialog::save()
{
    KConfigGroup config = Global::config()->group("HTML Export");

	QString folder = KUrl(m_url->url()).directory();
	config.writeEntry( "lastFolder",          folder                             );
	config.writeEntry( "embedLinkedFiles",    m_embedLinkedFiles->isChecked()    );
	config.writeEntry( "embedLinkedFolders",  m_embedLinkedFolders->isChecked()  );
	config.writeEntry( "erasePreviousFiles",  m_erasePreviousFiles->isChecked()  );
	config.writeEntry( "formatForImpression", m_formatForImpression->isChecked() );
}

void ExporterDialog::accept()
{
	save();
}

QString ExporterDialog::filePath()
{
	return m_url->url().url();
}

bool ExporterDialog::embedLinkedFiles()
{
	return m_embedLinkedFiles->isChecked();
}

bool ExporterDialog::embedLinkedFolders()
{
	return m_embedLinkedFolders->isChecked();
}

bool ExporterDialog::erasePreviousFiles()
{
	return m_erasePreviousFiles->isChecked();
}

bool ExporterDialog::formatForImpression()
{
	return m_formatForImpression->isChecked();
}


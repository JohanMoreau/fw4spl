/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2013.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>

#include <fwTools/UUID.hpp>
#include <fwTools/fwID.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/AppConfig.hpp>
#include <fwServices/IEditionService.hpp>

#include <fwData/Vector.hpp>
#include <fwData/String.hpp>
#include <fwData/Boolean.hpp>

#include <fwComEd/VectorMsg.hpp>

#include <fwGui/dialog/SelectorDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwActivities/IBuilder.hpp>
#include "activities/action/SActivityLauncher.hpp"

Q_DECLARE_METATYPE(::fwActivities::registry::ActivityInfo)

namespace activities
{
namespace action
{

//------------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::activities::action::SActivityLauncher, ::fwData::Vector );

//------------------------------------------------------------------------------

SActivityLauncher::SActivityLauncher() throw()
{}

//------------------------------------------------------------------------------

SActivityLauncher::~SActivityLauncher() throw()
{}

//------------------------------------------------------------------------------

void SActivityLauncher::starting() throw(::fwTools::Failed)
{
    this->actionServiceStarting();
    this->updateState();
}

//------------------------------------------------------------------------------

void SActivityLauncher::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->actionServiceStopping();
}

//------------------------------------------------------------------------------

void SActivityLauncher::configuring() throw(fwTools::Failed)
{
    this->::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

::fwActivities::registry::ActivityInfo SActivityLauncher::show( const ActivityInfoContainer & infos )
{
    QWidget *parent = qApp->activeWindow();

    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(QString::fromStdString("Choose an activity"));


    QStandardItemModel *model = new QStandardItemModel(dialog);

    BOOST_FOREACH( ::fwActivities::registry::ActivityInfo info, infos)
    {
        QStandardItem* item = new QStandardItem(QIcon(info.icon.c_str()), QString::fromStdString(info.id));
        item->setData(QVariant::fromValue(info));
        model->appendRow(item);
    }


    QListView * selectionList = new QListView();
    selectionList->setIconSize(QSize(100,100));
    selectionList->setUniformItemSizes(true);
    selectionList->setModel(model);

    QModelIndex index = model->index( 0, 0 );
    if ( index.isValid() )
    {
        selectionList->selectionModel()->select( index, QItemSelectionModel::Select );
    }

    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(selectionList);
    vLayout->addLayout(hLayout);

    dialog->setLayout(vLayout);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
    QObject::connect(selectionList, SIGNAL(doubleClicked( const QModelIndex &  )), dialog, SLOT(accept()));

    ::fwActivities::registry::ActivityInfo info;
    if(dialog->exec())
    {
        QModelIndex currentIndex = selectionList->selectionModel()->currentIndex();
        QStandardItem *item = model->itemFromIndex( currentIndex );
        QVariant var = item->data();
        info = var.value< ::fwActivities::registry::ActivityInfo >();
    }

    return info;
}

//------------------------------------------------------------------------------

void SActivityLauncher::updating() throw(::fwTools::Failed)
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();
    ActivityInfoContainer infos = ::fwActivities::registry::Activities::getDefault()->getInfos(selection);

    if ( ! infos.empty() )
    {
        ::fwActivities::registry::ActivityInfo info = this->show( infos );
        if( !info.id.empty() )
        {
            this->sendConfig( info );
        }
    }
    else
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog("Activity launcher",
                                                          "Not available activity for the current selection.",
                                                          ::fwGui::dialog::MessageDialog::WARNING);
    }
}

//------------------------------------------------------------------------------

void SActivityLauncher::receiving( ::fwServices::ObjectMsg::csptr msg ) throw(::fwTools::Failed)
{
    this->updateState();
}

//------------------------------------------------------------------------------

void SActivityLauncher::updateState()
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();
    ActivityInfoContainer infos = ::fwActivities::registry::Activities::getDefault()->getInfos(selection);
    this->setIsExecutable(!infos.empty());
}

//------------------------------------------------------------------------------

void SActivityLauncher::info( std::ostream &_sstream )
{}

//------------------------------------------------------------------------------

void SActivityLauncher::sendConfig( const ::fwActivities::registry::ActivityInfo & info )
{
    ::fwData::Vector::sptr selection = this->getObject< ::fwData::Vector >();

    ::fwActivities::IBuilder::sptr builder;
    builder = ::fwActivities::builder::factory::New(info.builderImpl);
    OSLM_ASSERT(info.builderImpl << " instantiation failed", builder);

    ::fwActivities::ActivitySeries::sptr actSeries;
    actSeries = builder->buildData(info, selection);
    SLM_ASSERT("ActivitySeries instantiation failed", actSeries);

    std::string fieldID         = "::fwServices::registry::AppConfig";
    std::string viewConfigID    = "viewConfigID";
    std::string closableFieldID = "closable";
    std::string iconFieldID     = "icon";
    std::string tooltipFieldID  = "tooltip";
    std::string tabIDFieldID    = "tabID";
    std::string tabID = "TABID_" + ::fwTools::UUID::generateUUID();

    ::fwServices::ObjectMsg::sptr  msg = ::fwServices::ObjectMsg::New();
    ::fwData::String::NewSptr title;

    title->value() = info.title;
    msg->addEvent( "NEW_CONFIGURATION_HELPER", title );
    title->setField( viewConfigID, ::fwData::String::New(info.appConfigId) );
    title->setField( closableFieldID, ::fwData::Boolean::New(true));
    title->setField( tabIDFieldID, ::fwData::String::New(tabID));
    title->setField( iconFieldID, ::fwData::String::New(info.icon) );

    ::fwServices::IEditionService::notify(this->getSptr(), selection, msg);
}

//------------------------------------------------------------------------------

}
}

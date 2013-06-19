/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _CTRLSELECTION_MANAGER_PATIENTDBWRAPPERSRV_HPP_
#define _CTRLSELECTION_MANAGER_PATIENTDBWRAPPERSRV_HPP_

#include <fwServices/IService.hpp>

#include "ctrlSelection/config.hpp"
#include "ctrlSelection/IWrapperSrv.hpp"

namespace ctrlSelection
{

namespace wrapper
{

/**
 * @class  PatientDBWrapperSrv.
 * @brief  Convert generic message by specific message from PatientDB
 * @author IRCAD (Research and Development Team).

 * @date   2007-2009.
 */
class CTRLSELECTION_CLASS_API PatientDBWrapperSrv : public ::ctrlSelection::IWrapperSrv
{

public :

    fwCoreServiceClassDefinitionsMacro ( (PatientDBWrapperSrv)(::ctrlSelection::IWrapperSrv) ) ;

//    /// Constructor.  Do nothing.
    CTRLSELECTION_API PatientDBWrapperSrv() throw() ;

    /// Destructor. Do nothing.
    CTRLSELECTION_API virtual ~PatientDBWrapperSrv() throw();

protected:

    /// Implements starting method derived from IService. Do nothing.
   virtual void starting()  throw ( ::fwTools::Failed ) {};

    /// Implements stopping method derived from IService. Do nothing.
    virtual void stopping()  throw ( ::fwTools::Failed ) {};

    /// Implements configuring method derived from IService. Do nothing.
    virtual void configuring()  throw ( ::fwTools::Failed ) {};

    /// Implements reconfiguring method derived from IService. Do nothing.
    virtual void reconfiguring()  throw ( ::fwTools::Failed ) {};

    /// Implements updating method derived from IService. Do nothing.
    virtual void updating() throw ( ::fwTools::Failed ) {};

    /// Implements info method derived from IService. Print classname.
    virtual void info( std::ostream &_sstream ) {};

    /**
     * @brief Convert the Object message with "UPDATED_OBJECT" event sent on PatientDB by PatientDBMsg with "NEW_PATIENT" event
     */
    CTRLSELECTION_API virtual void receiving( ::fwServices::ObjectMsg::csptr _msg ) throw ( ::fwTools::Failed );


};

} // wrapper
} // ctrlSelection

#endif // _CTRLSELECTION_MANAGER_PATIENTDBWRAPPERSRV_HPP_
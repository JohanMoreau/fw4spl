/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _FWSERVICES_CONFIGTEMPLATEMANAGER_HPP_
#define _FWSERVICES_CONFIGTEMPLATEMANAGER_HPP_

#include <vector>
#include <string>

#include <boost/tuple/tuple.hpp>

#include <fwTools/Object.hpp>

#include <fwRuntime/ConfigurationElement.hpp>

#include "fwServices/config.hpp"
#include "fwServices/IXMLParser.hpp"
#include "fwServices/IService.hpp"
#include "fwServices/ComChannelService.hpp"

namespace fwServices
{
/**
 * @class   AppConfigManager.
 * @brief   This class provides an API to manage config template.
 *
 * @author IRCAD (Research and Development Team).
 * @date    2007-2009.
 */
class FWSERVICES_CLASS_API AppConfigManager : public ::fwTools::Object
{
protected:
    enum ConfigState
    {
        STATE_CREATED,
        STATE_STARTED,
        STATE_STOPPED,
        STATE_DESTROYED,
    };
    typedef std::vector< ::fwServices::IService::wptr > ServiceContainer;
    typedef ::boost::tuple< std::string, bool > ConfigAttribute;

public:

    fwCoreClassDefinitionsWithFactoryMacro((AppConfigManager)(::fwTools::Object),
                                           (()),
                                           ::boost::make_shared< AppConfigManager >);

    /// Constructor. Do nothing.
    FWSERVICES_API AppConfigManager();

    /// Destructor. Do nothing.
    FWSERVICES_API virtual ~AppConfigManager();

    /// Return state
    bool isCreated() { return m_state == STATE_CREATED; }
    bool isStarted() { return m_state == STATE_STARTED; }
    bool isStopped() { return m_state == STATE_STOPPED; }
    bool isDestroyed() { return m_state == STATE_DESTROYED; }

    /// Set config param
    void setConfig(::fwRuntime::ConfigurationElement::csptr cfgElem)
    {
        m_cfgElem = cfgElem;
    }

    /// Get config root
    ::fwData::Object::sptr getConfigRoot() const
    {
        return m_configuredObject;
    }

    /// Get config root with autocast
    template < class ClassName >
    ::boost::shared_ptr< ClassName > getConfigRoot() const;

    /// Creates objects and services from config
    FWSERVICES_API virtual void create();

    /// Starts services specified in config
    FWSERVICES_API virtual void start();

    /// Updates services specified in config
    FWSERVICES_API virtual void update();

    /// Stops services specified in config
    FWSERVICES_API virtual void stop();

    /// Destroys services specified in config
    FWSERVICES_API virtual void destroy();

    /// Calls methods : create, start then update.
    FWSERVICES_API void launch();

    /// Stops and destroys services specified in config, then resets the configRoot sptr.
    FWSERVICES_API void stopAndDestroy();

protected:

    ::fwData::Object::sptr m_configuredObject;
    ::fwServices::IXMLParser::sptr m_objectParser;
    ::fwRuntime::ConfigurationElement::csptr m_cfgElem;

    ConfigState m_state;

    ServiceContainer m_createdSrv;
    ServiceContainer m_startedSrv;
    ServiceContainer m_startedComChannels;

    FWSERVICES_API ::fwData::Object::sptr getNewObject(
            ConfigAttribute type,
            const std::string& uid,
            ConfigAttribute id=ConfigAttribute("", false));

    FWSERVICES_API ::fwData::Object::sptr getNewObject(
            ConfigAttribute type,
            ConfigAttribute uid=ConfigAttribute("", false),
            ConfigAttribute id=ConfigAttribute("", false));

    FWSERVICES_API ::fwData::Object::sptr getRefObject(
            ConfigAttribute type,
            const std::string& uid,
            ConfigAttribute id=ConfigAttribute("", false));

    FWSERVICES_API ::fwServices::IService::sptr getNewService(
            ConfigAttribute type,
            ConfigAttribute uid,
            ConfigAttribute implType);

    FWSERVICES_API ::fwServices::ComChannelService::sptr connectComChannel(
            ::fwData::Object::sptr obj,
            ::fwServices::IService::sptr srv,
            ConfigAttribute priority);

    FWSERVICES_API void startComChannels();
    FWSERVICES_API void stopComChannels();

    FWSERVICES_API void stopStartedServices();
    FWSERVICES_API void destroyCreatedServices();

    FWSERVICES_API void processStartItems();
    FWSERVICES_API void processUpdateItems();

    FWSERVICES_API virtual ::fwData::Object::sptr createObject();
    FWSERVICES_API virtual void createServices();
    FWSERVICES_API virtual void createServices(::fwRuntime::ConfigurationElement::csptr cfgElem);

    FWSERVICES_API virtual void bindService(::fwRuntime::ConfigurationElement::csptr srvElem);
};

} // namespace fwServices

#include "fwServices/AppConfigManager.hxx"

#endif // _FWSERVICES_CONFIGTEMPLATEMANAGER_HPP_

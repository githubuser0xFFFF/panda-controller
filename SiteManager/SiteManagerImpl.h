//============================================================================
/// \file    SiteManagerImpl.h
/// \authors Florian Bauer <florian.bauer.dev@gmail.com>
/// \date    2021-01-10
/// \brief   Declaration of the CSiteManagerImpl class
/// \note    Code generated by sila2codegenerator 0.3.3-dev
//============================================================================
#ifndef SITEMANAGERIMPL_H
#define SITEMANAGERIMPL_H

#include <map>
#include <nlohmann/json.hpp>
#include <sila_cpp/server/SiLAFeature.h>
#include <sila_cpp/data_types.h>
#include <sila_cpp/server/command/UnobservableCommand.h>
#include <sila_cpp/server/property/UnobservableProperty.h>
#include "MoveDirection.h"
#include "Pose.h"
#include "SiteManager.grpc.pb.h"

/**
 * @brief The CSiteManagerImpl class implements the SiteManager feature
 *
 * @details Manager to set and remove Sites which define the pick-up and drop locations
 * for the robot gripper.
 */
class CSiteManagerImpl final : public SiLA2::CSiLAFeature<sila2::de::fau::robot::sitemanager::v1::SiteManager> {
    // Using declarations for the Feature's Commands and Properties
    using SetSiteCommand = SiLA2::CUnobservableCommandManager<&CSiteManagerImpl::RequestSetSite>;
    using SetSiteWrapper = SiLA2::CUnobservableCommandWrapper<
            sila2::de::fau::robot::sitemanager::v1::SetSite_Parameters,
            sila2::de::fau::robot::sitemanager::v1::SetSite_Responses>;
    using DeleteSiteCommand = SiLA2::CUnobservableCommandManager<&CSiteManagerImpl::RequestDeleteSite>;
    using DeleteSiteWrapper = SiLA2::CUnobservableCommandWrapper<
            sila2::de::fau::robot::sitemanager::v1::DeleteSite_Parameters,
            sila2::de::fau::robot::sitemanager::v1::DeleteSite_Responses>;
    using SitesProperty = SiLA2::CUnobservablePropertyWrapper<
            std::vector<SiLA2::CString>, &CSiteManagerImpl::RequestGet_Sites>;

public:
    /**
     * @brief C'tor
     *
     * @param parent The SiLA server instance that contains this Feature
     */
    explicit CSiteManagerImpl(SiLA2::CSiLAServer* parent);

    /**
     * @brief SetSite Command
     *
     * @details Adds or overwrites a Site. The orientation is defined a Quaternion (X, Y, Z,
     * W).
     *
     * @param Command The current SetSite Command Execution Wrapper
     * It contains the following Parameters:
     * @li SiteId The string identifier of the Site.
     * @li Pose The Pose to reach the Site.
     *
     * @return SetSite_Responses The Command Response
     * It contains the following fields:
     * None
     *
     * @throw Validation Error if the given Parameter(s) are invalid
     */
    sila2::de::fau::robot::sitemanager::v1::SetSite_Responses SetSite(SetSiteWrapper* command);

    /**
     * @brief DeleteSite Command
     *
     * @details Removes a Site.
     *
     * @param Command The current DeleteSite Command Execution Wrapper
     * It contains the following Parameters:
     * @li SiteId The string identifier of the Site.
     *
     * @return DeleteSite_Responses The Command Response
     * It contains the following fields:
     * None
     *
     * @throw Validation Error if the given Parameter(s) are invalid
     */
    sila2::de::fau::robot::sitemanager::v1::DeleteSite_Responses DeleteSite(DeleteSiteWrapper* command);

private:
    SetSiteCommand m_SetSiteCommand;
    DeleteSiteCommand m_DeleteSiteCommand;
    SitesProperty m_SitesProperty;
    std::map<std::string, nlohmann::json> m_JsonSites;

    static std::map<std::string, nlohmann::json> loadJsonFilesToMap();
};

#endif  // SITEMANAGERIMPL_H

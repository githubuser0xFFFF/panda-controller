//============================================================================
/// \file    SiteManagerImpl.cpp
/// \authors Florian Bauer <florian.bauer.dev@gmail.com>
/// \date    2021-01-25
/// \brief   Definition of the CSiteManagerImpl class
/// \note    Code generated by sila2codegenerator 0.3.3-dev
//============================================================================

#include "SiteManagerImpl.h"

#include <iostream>
#include <fstream>
#include <sila_cpp/common/logging.h>
#include "FileManager.h"
#include "SiteManager.pb.h"

namespace fs = std::filesystem;
using namespace sila2::de::fau::robot::sitemanager::v1;
using json = nlohmann::json;

constexpr double CM_TO_M = 0.01;

std::map<std::string, json> CSiteManagerImpl::loadJsonFilesToMap() {
    const fs::path appDir = FileManager::getAppDir();
    const fs::path sitesDir = appDir / SITES_DIR;
    FileManager::checkAndCreateDir(sitesDir);

    std::map<std::string, json> sites;
    const std::vector<fs::path> sitesFiles = FileManager::collectJsonFilesFromDir(sitesDir);
    for (const auto& file : sitesFiles) {
        // read a JSON file
        std::ifstream jsonStream(file);
        json jsonStruct;
        try {
            jsonStream >> jsonStruct;
        } catch (const std::exception& ex) {
            std::cerr << "Could not load: " << file << "\n" << ex.what();
            jsonStream.close();
            continue;
        }
        jsonStream.close();

        const std::string& id = jsonStruct[Site::SITE_ID].get<std::string>();
        sites[id] = jsonStruct;
    }
    return sites;
}

CSiteManagerImpl::CSiteManagerImpl(SiLA2::CSiLAServer* parent)
: CSiLAFeature{parent}
, m_SetSiteCommand{this, "SetSite"}
, m_DeleteSiteCommand{this, "DeleteSite"}
, m_SitesProperty{this, "Sites"}
, m_JsonSites(loadJsonFilesToMap())
, m_SitesDir(FileManager::getAppDir() / SITES_DIR) {

    std::vector<SiLA2::CString> siteIds;
    for (auto const& elem : m_JsonSites) {
        // collect IDs
        siteIds.push_back(SiLA2::CString(elem.first));
    }
    m_SitesProperty.setValue(siteIds);
    m_SetSiteCommand.setExecutor(this, &CSiteManagerImpl::SetSite);
    m_DeleteSiteCommand.setExecutor(this, &CSiteManagerImpl::DeleteSite);
}

SetSite_Responses CSiteManagerImpl::SetSite(SetSiteWrapper* command) {
    const auto request = command->parameters();
    const std::string& idToSet = request.siteid().siteid().value();
    const auto iter = m_JsonSites.find(idToSet);
    json jsonStruct;
    const bool isSiteIdInList = (iter != m_JsonSites.end());
    if (!isSiteIdInList) {
        // Add a new ID to property list.
        m_SitesProperty.append(request.siteid().siteid());
        jsonStruct[Site::SITE_ID] = idToSet;
    } else {
        jsonStruct = iter->second;
    }

    const auto& pose = request.pose().pose();
    jsonStruct[Site::POSE] = {
        {Site::POS_X, pose.x().value()},
        {Site::POS_Y, pose.y().value()},
        {Site::POS_Z, pose.z().value()},
        {Site::ORI_X, pose.orix().value()},
        {Site::ORI_Y, pose.oriy().value()},
        {Site::ORI_Z, pose.oriz().value()},
        {Site::ORI_W, pose.oriw().value()},
    };

    const auto& approachMove = request.approach().relativemove();
    jsonStruct[Site::APPROACH] = {
        {Site::DESIRED_DIST, approachMove.desireddist().value() * CM_TO_M},
        {Site::MIN_DIST, approachMove.mindist().value() * CM_TO_M},
        {Site::DIR_X, approachMove.x().value() * CM_TO_M},
        {Site::DIR_Y, approachMove.y().value() * CM_TO_M},
        {Site::DIR_Z, approachMove.z().value() * CM_TO_M},
    };

    if (!isSiteIdInList) {
        // Add entry to map.
        m_JsonSites[idToSet] = jsonStruct;

    }

    // Write JSON data to file.
    const fs::path jsonFile = m_SitesDir / (idToSet + JSON_FILE_EXT);
    FileManager::saveJsonToFile(jsonStruct, jsonFile);

    return SetSite_Responses{};
}

DeleteSite_Responses CSiteManagerImpl::DeleteSite(DeleteSiteWrapper* command) {
    const auto request = command->parameters();
    const std::string& idToDelete = request.siteid().siteid().value();
    const auto iter = m_JsonSites.find(idToDelete);
    const bool isSiteIdInList = (iter != m_JsonSites.cend());
    if (isSiteIdInList) {
        m_JsonSites.erase(iter);
        std::vector<SiLA2::CString> siteIds;
        for (auto const& elem : m_JsonSites) {
            // collect IDs
            siteIds.push_back(SiLA2::CString(elem.first));
        }
        m_SitesProperty.setValue(siteIds);
        const fs::path jsonFile = m_SitesDir / (idToDelete + JSON_FILE_EXT);
        fs::remove(jsonFile);
    } else {
        throw SiLA2::CDefinedExecutionError{
            "SiteIdNotFound",
            "The given Site ID does not exist or could not be found."};
    }

    return DeleteSite_Responses{};
}

/**
 * Checks if the given site ID is available.
 * 
 * @param siteId The site ID to check.
 * @return true if the site ID is known, otherwise false.
 */
bool CSiteManagerImpl::hasSiteId(const std::string& siteId) const {
    return (m_JsonSites.find(siteId) != m_JsonSites.cend());
}

/**
 * Get the site object with the site ID. No validation of the ID is made.
 * 
 * @param siteId The site ID for the site to get.
 * @return The site object for further usage.
 */
Site CSiteManagerImpl::getSite(const std::string& siteId) const {
    const std::map<std::string, nlohmann::json>::const_iterator iter = m_JsonSites.find(siteId);
    return Site{iter->second};
}

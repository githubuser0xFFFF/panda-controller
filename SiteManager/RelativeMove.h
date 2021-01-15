//============================================================================
/// \file    RelativeMove.h
/// \authors Florian Bauer <florian.bauer.dev@gmail.com>
/// \date    2021-01-15
/// \brief   Definition of the RelativeMove Data Type for the SiteManager Feature
/// \note    Code generated by sila2codegenerator 0.3.3-dev
//============================================================================
#ifndef RELATIVEMOVE_H
#define RELATIVEMOVE_H

#include "SiteManager.grpc.pb.h"

/**
 * @brief Convenience type for the DataType_RelativeMove Protobuf Message
 */
struct RelativeMove {
    SiLA2::CReal X;
    SiLA2::CReal Y;
    SiLA2::CReal Z;
    SiLA2::CReal Distance;

    [[nodiscard]] sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove::RelativeMove_Struct*
    makeNestedMessage() const {
        using sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove;
        auto result = new DataType_RelativeMove::RelativeMove_Struct{};
        result->set_allocated_x(X.toProtoMessagePtr());
        result->set_allocated_y(Y.toProtoMessagePtr());
        result->set_allocated_z(Z.toProtoMessagePtr());
        result->set_allocated_distance(Distance.toProtoMessagePtr());
        return result;
    }

    /**
     * @brief Convert this convenience type to a SiLAFramework type, i.e. the
     * Protobuf Message
     *
     * @return The SiLAFramework equivalent of this type as a Protobuf Message
     */
    [[nodiscard]] sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove
    toProtoMessage() const {
        using sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove;
        auto result = DataType_RelativeMove{};
        result.set_allocated_relativemove(makeNestedMessage());
        return result;
    }

    /**
     * @brief Convert this convenience type to a SiLAFramework type, i.e. the
     * Protobuf Message
     *
     * @return The SiLAFramework equivalent of this type as a Protobuf Message
     * pointer
     */
    [[nodiscard]] sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove*
    toProtoMessagePtr() const {
        using sila2::de::fau::robot::sitemanager::v1::DataType_RelativeMove;
        auto result = new DataType_RelativeMove{};
        result->set_allocated_relativemove(makeNestedMessage());
        return result;
    }
};

#endif  // RELATIVEMOVE_H

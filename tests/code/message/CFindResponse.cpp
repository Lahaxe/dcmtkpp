#define BOOST_TEST_MODULE CFindResponse
#include <boost/test/unit_test.hpp>

#include "dcmtkpp/message/CFindResponse.h"
#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/message/Message.h"
#include "dcmtkpp/registry.h"

#include "../../MessageFixtureBase.h"

struct Fixture: public MessageFixtureBase<dcmtkpp::message::CFindResponse>
{
    dcmtkpp::DataSet command_set;
    dcmtkpp::DataSet data_set;

    Fixture()
    {
        command_set.add(
            "CommandField", {dcmtkpp::message::Message::Command::C_FIND_RSP});
        command_set.add("MessageIDBeingRespondedTo", {1234});
        command_set.add("Status", {dcmtkpp::message::Response::Success});

        command_set.add("MessageID", {5678});
        command_set.add("AffectedSOPClassUID",
            {dcmtkpp::registry::StudyRootQueryRetrieveInformationModelFIND});

        data_set.add("PatientName", {"Doe^John"});
        data_set.add("PatientID", {"DJ123"});
        data_set.add("StudyDescription", {"Brain"});
        data_set.add("StudyInstanceUID", {"1.2.3"});
    }

    virtual void check(dcmtkpp::message::CFindResponse const & message)
    {
        BOOST_CHECK_EQUAL(
            message.get_command_field(),
            dcmtkpp::message::Message::Command::C_FIND_RSP);
        BOOST_CHECK_EQUAL(message.get_message_id_being_responded_to(), 1234);
        BOOST_CHECK_EQUAL(
            message.get_status(), dcmtkpp::message::Response::Success);

        BOOST_CHECK(message.has_message_id());
        BOOST_CHECK_EQUAL(message.get_message_id(), 5678);

        BOOST_CHECK(message.has_affected_sop_class_uid());
        BOOST_CHECK_EQUAL(
            message.get_affected_sop_class_uid(),
            dcmtkpp::registry::StudyRootQueryRetrieveInformationModelFIND);

        BOOST_CHECK(message.has_data_set());
        BOOST_CHECK(message.get_data_set() == this->data_set);
    }
};

BOOST_FIXTURE_TEST_CASE(Constructor, Fixture)
{
    dcmtkpp::message::CFindResponse message(
        1234, dcmtkpp::message::Response::Success, this->data_set);
    message.set_message_id(5678);
    message.set_affected_sop_class_uid(
        dcmtkpp::registry::StudyRootQueryRetrieveInformationModelFIND);
    this->check(message);
}

BOOST_FIXTURE_TEST_CASE(MessageConstructor, Fixture)
{
    this->check_message_constructor(this->command_set, this->data_set);
}

BOOST_FIXTURE_TEST_CASE(MessageConstructorWrongCommandField, Fixture)
{
    this->command_set.as_int("CommandField") = {
        dcmtkpp::message::Message::Command::C_ECHO_RQ};
    this->check_message_constructor_throw(this->command_set, this->data_set);
}

BOOST_AUTO_TEST_CASE(StatusPending)
{
    std::vector<dcmtkpp::Value::Integer> const statuses = {
        dcmtkpp::message::CFindResponse::PendingWarningOptionalKeysNotSupported,
    };

    for(auto const status:statuses)
    {
        dcmtkpp::message::CFindResponse response(1234, status);
        BOOST_REQUIRE(response.is_pending());
        BOOST_REQUIRE(!response.is_warning());
        BOOST_REQUIRE(!response.is_failure());
    }
}

BOOST_AUTO_TEST_CASE(StatusFailure)
{
    std::vector<dcmtkpp::Value::Integer> const statuses = {
        dcmtkpp::message::CFindResponse::RefusedOutOfResources,
        dcmtkpp::message::CFindResponse::IdentifierDoesNotMatchSOPClass,
        dcmtkpp::message::CFindResponse::UnableToProcess
    };

    for(auto const status:statuses)
    {
        dcmtkpp::message::CFindResponse response(1234, status);
        BOOST_REQUIRE(!response.is_pending());
        BOOST_REQUIRE(!response.is_warning());
        BOOST_REQUIRE(response.is_failure());
    }
}

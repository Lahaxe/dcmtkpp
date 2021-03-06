#define BOOST_TEST_MODULE CGetResponse
#include <boost/test/unit_test.hpp>

#include "dcmtkpp/message/CGetResponse.h"
#include "dcmtkpp/DataSet.h"
#include "dcmtkpp/message/Message.h"
#include "dcmtkpp/registry.h"

#include "../../MessageFixtureBase.h"

struct Fixture: public MessageFixtureBase<dcmtkpp::message::CGetResponse>
{
    dcmtkpp::DataSet command_set;
    dcmtkpp::DataSet data_set;

    Fixture()
    {
        this->command_set.add(
            "CommandField", {dcmtkpp::message::Message::Command::C_GET_RSP});
        this->command_set.add("MessageIDBeingRespondedTo", {1234});
        this->command_set.add("Status", {dcmtkpp::message::Response::Success});

        this->command_set.add("MessageID", {5678});
        this->command_set.add("AffectedSOPClassUID",
            {dcmtkpp::registry::StudyRootQueryRetrieveInformationModelGET});
        this->command_set.add(dcmtkpp::registry::NumberOfRemainingSuboperations, {1});
        this->command_set.add(dcmtkpp::registry::NumberOfCompletedSuboperations, {2});
        this->command_set.add(dcmtkpp::registry::NumberOfFailedSuboperations, {3});
        this->command_set.add(dcmtkpp::registry::NumberOfWarningSuboperations, {4});

        this->data_set.add("PatientName", {"Doe^John"});
        this->data_set.add("PatientID", {"DJ123"});
        this->data_set.add("StudyDescription", {"Brain"});
        this->data_set.add("StudyInstanceUID", {"1.2.3"});
    }

    virtual void check(dcmtkpp::message::CGetResponse const & message)
    {
        BOOST_CHECK_EQUAL(
            message.get_command_field(),
            dcmtkpp::message::Message::Command::C_GET_RSP);
        BOOST_CHECK_EQUAL(message.get_message_id_being_responded_to(), 1234);
        BOOST_CHECK_EQUAL(
            message.get_status(), dcmtkpp::message::Response::Success);

        BOOST_CHECK(message.has_message_id());
        BOOST_CHECK_EQUAL(message.get_message_id(), 5678);

        BOOST_CHECK(message.has_affected_sop_class_uid());
        BOOST_CHECK_EQUAL(
            message.get_affected_sop_class_uid(),
            dcmtkpp::registry::StudyRootQueryRetrieveInformationModelGET);

        BOOST_CHECK(message.has_number_of_remaining_sub_operations());
        BOOST_CHECK_EQUAL(message.get_number_of_remaining_sub_operations(), 1);

        BOOST_CHECK(message.has_number_of_completed_sub_operations());
        BOOST_CHECK_EQUAL(message.get_number_of_completed_sub_operations(), 2);

        BOOST_CHECK(message.has_number_of_failed_sub_operations());
        BOOST_CHECK_EQUAL(message.get_number_of_failed_sub_operations(), 3);

        BOOST_CHECK(message.has_number_of_warning_sub_operations());
        BOOST_CHECK_EQUAL(message.get_number_of_warning_sub_operations(), 4);

        BOOST_CHECK(message.has_data_set());
        BOOST_CHECK(message.get_data_set() == this->data_set);
    }
};

BOOST_FIXTURE_TEST_CASE(Constructor, Fixture)
{
    dcmtkpp::message::CGetResponse message(
        1234, dcmtkpp::message::Response::Success, this->data_set);
    message.set_message_id(5678);
    message.set_affected_sop_class_uid(
        dcmtkpp::registry::StudyRootQueryRetrieveInformationModelGET);
    message.set_number_of_remaining_sub_operations(1);
    message.set_number_of_completed_sub_operations(2);
    message.set_number_of_failed_sub_operations(3);
    message.set_number_of_warning_sub_operations(4);

    this->check(message);
}

BOOST_FIXTURE_TEST_CASE(MessageConstructor, Fixture)
{
    this->check_message_constructor(this->command_set, this->data_set);
}

BOOST_FIXTURE_TEST_CASE(MessageConstructorWrongCommandField, Fixture)
{
    this->command_set.as_int(dcmtkpp::registry::CommandField) = {
        dcmtkpp::message::Message::Command::C_ECHO_RQ };
    this->check_message_constructor_throw(this->command_set, this->data_set);
}

BOOST_AUTO_TEST_CASE(StatusWarning)
{
    std::vector<dcmtkpp::Value::Integer> const statuses = {
        dcmtkpp::message::CGetResponse::SubOperationsCompleteOneOrMoreFailuresOrWarnings
    };

    for(auto const status:statuses)
    {
        dcmtkpp::message::CGetResponse response(1234, status);
        BOOST_REQUIRE(!response.is_pending());
        BOOST_REQUIRE(response.is_warning());
        BOOST_REQUIRE(!response.is_failure());
    }
}

BOOST_AUTO_TEST_CASE(StatusFailure)
{
    std::vector<dcmtkpp::Value::Integer> const statuses = {
        dcmtkpp::message::CGetResponse::RefusedOutOfResourcesUnableToCalculateNumberOfMatches,
        dcmtkpp::message::CGetResponse::RefusedOutOfResourcesUnableToPerformSubOperations,
        dcmtkpp::message::CGetResponse::IdentifierDoesNotMatchSOPClass,
        dcmtkpp::message::CGetResponse::UnableToProcess
    };

    for(auto const status:statuses)
    {
        dcmtkpp::message::CGetResponse response(1234, status);
        BOOST_REQUIRE(!response.is_pending());
        BOOST_REQUIRE(!response.is_warning());
        BOOST_REQUIRE(response.is_failure());
    }
}

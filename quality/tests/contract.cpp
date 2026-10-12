#include "one-msg-sdk/model/SendMessageRequest.h"
#include "one-msg-sdk/model/SendFlow_request_header.h"
#include "one-msg-sdk/model/SetWebhook_request_webhookUrl.h"
#include "one-msg-sdk/ApiClient.h"
#include <iostream>
#include <stdexcept>

/** Verify generated JSON field names and optional omission without contacting a service. */
int main() {
    using onemsg::sdk::model::SendMessageRequest;
    SendMessageRequest request;
    request.setBody(utility::conversions::to_string_t("hello"));
    request.setChatId(utility::conversions::to_string_t("12020721369@c.us"));
    const auto json = request.toJson();
    if (json.at(utility::conversions::to_string_t("body")).as_string() != utility::conversions::to_string_t("hello") || json.at(utility::conversions::to_string_t("chatId")).as_string() != utility::conversions::to_string_t("12020721369@c.us")) {
        throw std::runtime_error("request wire names changed");
    }
    if (json.has_field(utility::conversions::to_string_t("phone")) || json.has_field(utility::conversions::to_string_t("quotedMsgId"))) {
        throw std::runtime_error("unset optional fields must be omitted");
    }
    SendMessageRequest decoded;
    if (!decoded.fromJson(json) || decoded.getBody() != request.getBody() || decoded.getChatId() != request.getChatId()) {
        throw std::runtime_error("request JSON does not round-trip");
    }
    onemsg::sdk::model::SendFlow_request_header header;
    const auto headerText = web::json::value::string(utility::conversions::to_string_t("header text"));
    if (!header.fromJson(headerText) || header.toJson() != headerText) throw std::runtime_error("string union alternative lost");
    auto headerObject = web::json::value::object();
    headerObject[utility::conversions::to_string_t("text")] = headerText;
    if (!header.fromJson(headerObject) || header.toJson() != headerObject) throw std::runtime_error("object union alternative lost");
    if (header.fromJson(web::json::value::null())) throw std::runtime_error("non-nullable union accepted null");
    onemsg::sdk::model::SetWebhook_request_webhookUrl webhook;
    auto urls = web::json::value::array(2);
    urls[0] = web::json::value::string(utility::conversions::to_string_t("https://example.test/one"));
    urls[1] = web::json::value::string(utility::conversions::to_string_t("https://example.test/two"));
    if (!webhook.fromJson(urls) || webhook.toJson() != urls) throw std::runtime_error("webhook array union lost");
    if (webhook.fromJson(web::json::value::number(42))) throw std::runtime_error("webhook union accepted invalid type");
    const std::map<utility::string_t, utility::string_t> names{{utility::conversions::to_string_t("formatted_name"), utility::conversions::to_string_t("Contact")}};
    const auto parameter = onemsg::sdk::api::ApiClient::parameterToString(names);
    if (!web::json::value::parse(parameter).is_object()) throw std::runtime_error("map form field must remain JSON");
    std::cout << "offline SDK contract smoke passed\n";
}

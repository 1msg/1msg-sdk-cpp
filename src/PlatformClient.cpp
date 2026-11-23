#include "one-msg-sdk/PlatformClient.h"
#include <cpprest/http_client.h>
#include <iomanip>
#include <sstream>
#include <vector>
namespace onemsg { namespace sdk {
namespace {
struct Op { std::string method; std::string path; std::vector<std::string> path_params; };
const std::map<std::string, Op> kOperations = {
    {"listInboxChannels", {"GET", "/api/v1/inbox/channels", {}}},
    {"listInboxChannelAssignees", {"GET", "/api/v1/inbox/channels/{channelId}/assignees", {"channelId"}}},
    {"listInboxConversations", {"GET", "/api/v1/inbox/conversations", {}}},
    {"searchInboxConversations", {"GET", "/api/v1/inbox/conversations/search", {}}},
    {"countInboxConversations", {"GET", "/api/v1/inbox/conversations/counts", {}}},
    {"getInboxConversation", {"GET", "/api/v1/inbox/conversations/{conversationId}", {"conversationId"}}},
    {"updateInboxConversationStatus", {"PATCH", "/api/v1/inbox/conversations/{conversationId}/status", {"conversationId"}}},
    {"updateInboxConversationAssignee", {"PATCH", "/api/v1/inbox/conversations/{conversationId}/assignee", {"conversationId"}}},
    {"listInboxConversationMessages", {"GET", "/api/v1/inbox/conversations/{conversationId}/messages", {"conversationId"}}},
    {"blockInboxContact", {"POST", "/api/v1/inbox/conversations/{conversationId}/block", {"conversationId"}}},
    {"unblockInboxContact", {"POST", "/api/v1/inbox/conversations/{conversationId}/unblock", {"conversationId"}}},
    {"renameInboxConversation", {"PATCH", "/api/v1/inbox/conversations/{conversationId}/title", {"conversationId"}}},
    {"listInboxContacts", {"GET", "/api/v1/inbox/contacts", {}}},
    {"getInboxWebhook", {"GET", "/api/v1/inbox/webhook", {}}},
    {"setInboxWebhook", {"PUT", "/api/v1/inbox/webhook", {}}},
    {"deleteInboxWebhook", {"DELETE", "/api/v1/inbox/webhook", {}}},
    {"rotateInboxWebhookSecret", {"POST", "/api/v1/inbox/webhook/secret", {}}},
    {"listCampaignSegments", {"GET", "/api/v1/campaigns/segments", {}}},
    {"createCampaignSegment", {"POST", "/api/v1/campaigns/segments", {}}},
    {"getCampaignSegment", {"GET", "/api/v1/campaigns/segments/{segmentId}", {"segmentId"}}},
    {"updateCampaignSegment", {"PATCH", "/api/v1/campaigns/segments/{segmentId}", {"segmentId"}}},
    {"deleteCampaignSegment", {"DELETE", "/api/v1/campaigns/segments/{segmentId}", {"segmentId"}}},
    {"listCampaignSegmentMembers", {"GET", "/api/v1/campaigns/segments/{segmentId}/members", {"segmentId"}}},
    {"addCampaignSegmentMembers", {"POST", "/api/v1/campaigns/segments/{segmentId}/members", {"segmentId"}}},
    {"replaceCampaignSegmentMembers", {"PUT", "/api/v1/campaigns/segments/{segmentId}/members", {"segmentId"}}},
    {"removeCampaignSegmentMembers", {"DELETE", "/api/v1/campaigns/segments/{segmentId}/members", {"segmentId"}}},
    {"updateCampaignSegmentMember", {"PATCH", "/api/v1/campaigns/segments/{segmentId}/members/{memberId}", {"segmentId", "memberId"}}},
    {"listCampaigns", {"GET", "/api/v1/campaigns", {}}},
    {"createCampaign", {"POST", "/api/v1/campaigns", {}}},
    {"getCampaign", {"GET", "/api/v1/campaigns/{campaignId}", {"campaignId"}}},
    {"updateCampaign", {"PATCH", "/api/v1/campaigns/{campaignId}", {"campaignId"}}},
    {"previewCampaign", {"GET", "/api/v1/campaigns/{campaignId}/preview", {"campaignId"}}},
    {"sendCampaign", {"POST", "/api/v1/campaigns/{campaignId}/send", {"campaignId"}}},
    {"scheduleCampaign", {"PATCH", "/api/v1/campaigns/{campaignId}/schedule", {"campaignId"}}},
    {"unscheduleCampaign", {"POST", "/api/v1/campaigns/{campaignId}/unschedule", {"campaignId"}}},
    {"pauseCampaign", {"POST", "/api/v1/campaigns/{campaignId}/pause", {"campaignId"}}},
    {"resumeCampaign", {"POST", "/api/v1/campaigns/{campaignId}/resume", {"campaignId"}}},
    {"completeCampaign", {"POST", "/api/v1/campaigns/{campaignId}/complete", {"campaignId"}}},
    {"testCampaign", {"POST", "/api/v1/campaigns/{campaignId}/test", {"campaignId"}}},
    {"getCampaignAnalytics", {"GET", "/api/v1/campaigns/{campaignId}/analytics", {"campaignId"}}},
    {"listCampaignAnalyticsRecipients", {"GET", "/api/v1/campaigns/{campaignId}/analytics/recipients", {"campaignId"}}},
    {"listCampaignAnalyticsReplies", {"GET", "/api/v1/campaigns/{campaignId}/analytics/replies", {"campaignId"}}},
    {"listCampaignAnalyticsErrors", {"GET", "/api/v1/campaigns/{campaignId}/analytics/errors", {"campaignId"}}},
};
std::string encode(const std::string& value) {
  std::ostringstream out;
  for (unsigned char byte : value) {
    if ((byte >= 'A' && byte <= 'Z') || (byte >= 'a' && byte <= 'z') || (byte >= '0' && byte <= '9') || byte == '-' || byte == '_' || byte == '.' || byte == '~') out << byte;
    else out << '%' << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << int(byte) << std::dec << std::nouppercase;
  }
  return out.str();
}
}
PlatformClient::PlatformClient(std::string api_key, std::string base_url) : api_key_(std::move(api_key)), base_url_(std::move(base_url)) {
  if (api_key_.empty()) throw std::invalid_argument("apiKey is required");
  while (!base_url_.empty() && base_url_.back() == '/') base_url_.pop_back();
  if (base_url_.empty()) base_url_ = "https://api.1msg.io";
}
std::string PlatformClient::call(const std::string& operation_id, const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) {
  auto found = kOperations.find(operation_id);
  if (found == kOperations.end()) throw std::invalid_argument("unknown platform operation " + operation_id);
  const Op& spec = found->second;
  std::string url_path = spec.path;
  for (const auto& name : spec.path_params) {
    auto value = path.find(name);
    if (value == path.end()) throw std::invalid_argument("missing path param " + name);
    auto token = "{" + name + "}";
    auto at = url_path.find(token);
    if (at != std::string::npos) url_path.replace(at, token.size(), encode(value->second));
  }
  web::http::uri_builder builder(url_path);
  for (const auto& entry : query) builder.append_query(entry.first, entry.second);
  web::http::client::http_client client(base_url_);
  web::http::http_request request;
  request.set_method(spec.method);
  request.set_request_uri(builder.to_uri());
  request.headers().add("Authorization", "Bearer " + api_key_);
  request.headers().add("Accept", "application/json");
  if (!body.empty()) request.set_body(body, "application/json");
  auto response = client.request(request).get();
  auto text = response.extract_string().get();
  if (response.status_code() < 200 || response.status_code() >= 300) throw PlatformError(response.status_code(), text);
  return text;
}
std::string PlatformClient::listInboxChannels(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listInboxChannels", path, query, body); }
std::string PlatformClient::listInboxChannelAssignees(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listInboxChannelAssignees", path, query, body); }
std::string PlatformClient::listInboxConversations(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listInboxConversations", path, query, body); }
std::string PlatformClient::searchInboxConversations(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("searchInboxConversations", path, query, body); }
std::string PlatformClient::countInboxConversations(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("countInboxConversations", path, query, body); }
std::string PlatformClient::getInboxConversation(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("getInboxConversation", path, query, body); }
std::string PlatformClient::updateInboxConversationStatus(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("updateInboxConversationStatus", path, query, body); }
std::string PlatformClient::updateInboxConversationAssignee(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("updateInboxConversationAssignee", path, query, body); }
std::string PlatformClient::listInboxConversationMessages(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listInboxConversationMessages", path, query, body); }
std::string PlatformClient::blockInboxContact(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("blockInboxContact", path, query, body); }
std::string PlatformClient::unblockInboxContact(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("unblockInboxContact", path, query, body); }
std::string PlatformClient::renameInboxConversation(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("renameInboxConversation", path, query, body); }
std::string PlatformClient::listInboxContacts(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listInboxContacts", path, query, body); }
std::string PlatformClient::getInboxWebhook(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("getInboxWebhook", path, query, body); }
std::string PlatformClient::setInboxWebhook(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("setInboxWebhook", path, query, body); }
std::string PlatformClient::deleteInboxWebhook(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("deleteInboxWebhook", path, query, body); }
std::string PlatformClient::rotateInboxWebhookSecret(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("rotateInboxWebhookSecret", path, query, body); }
std::string PlatformClient::listCampaignSegments(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaignSegments", path, query, body); }
std::string PlatformClient::createCampaignSegment(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("createCampaignSegment", path, query, body); }
std::string PlatformClient::getCampaignSegment(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("getCampaignSegment", path, query, body); }
std::string PlatformClient::updateCampaignSegment(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("updateCampaignSegment", path, query, body); }
std::string PlatformClient::deleteCampaignSegment(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("deleteCampaignSegment", path, query, body); }
std::string PlatformClient::listCampaignSegmentMembers(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaignSegmentMembers", path, query, body); }
std::string PlatformClient::addCampaignSegmentMembers(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("addCampaignSegmentMembers", path, query, body); }
std::string PlatformClient::replaceCampaignSegmentMembers(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("replaceCampaignSegmentMembers", path, query, body); }
std::string PlatformClient::removeCampaignSegmentMembers(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("removeCampaignSegmentMembers", path, query, body); }
std::string PlatformClient::updateCampaignSegmentMember(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("updateCampaignSegmentMember", path, query, body); }
std::string PlatformClient::listCampaigns(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaigns", path, query, body); }
std::string PlatformClient::createCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("createCampaign", path, query, body); }
std::string PlatformClient::getCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("getCampaign", path, query, body); }
std::string PlatformClient::updateCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("updateCampaign", path, query, body); }
std::string PlatformClient::previewCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("previewCampaign", path, query, body); }
std::string PlatformClient::sendCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("sendCampaign", path, query, body); }
std::string PlatformClient::scheduleCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("scheduleCampaign", path, query, body); }
std::string PlatformClient::unscheduleCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("unscheduleCampaign", path, query, body); }
std::string PlatformClient::pauseCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("pauseCampaign", path, query, body); }
std::string PlatformClient::resumeCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("resumeCampaign", path, query, body); }
std::string PlatformClient::completeCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("completeCampaign", path, query, body); }
std::string PlatformClient::testCampaign(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("testCampaign", path, query, body); }
std::string PlatformClient::getCampaignAnalytics(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("getCampaignAnalytics", path, query, body); }
std::string PlatformClient::listCampaignAnalyticsRecipients(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaignAnalyticsRecipients", path, query, body); }
std::string PlatformClient::listCampaignAnalyticsReplies(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaignAnalyticsReplies", path, query, body); }
std::string PlatformClient::listCampaignAnalyticsErrors(const std::map<std::string, std::string>& path, const std::map<std::string, std::string>& query, const std::string& body) { return call("listCampaignAnalyticsErrors", path, query, body); }
}}

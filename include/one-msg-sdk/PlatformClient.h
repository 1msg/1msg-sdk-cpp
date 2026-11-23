#pragma once
#include <map>
#include <stdexcept>
#include <string>
namespace onemsg { namespace sdk {
class PlatformError : public std::runtime_error {
 public:
  int status;
  std::string response_body;
  PlatformError(int status, std::string body) : std::runtime_error("Platform API HTTP " + std::to_string(status)), status(status), response_body(std::move(body)) {}
};
/** Platform API client. Bearer API key, not the WhatsApp channel token. */
class PlatformClient {
 public:
  PlatformClient(std::string api_key, std::string base_url = "https://api.1msg.io");
  std::string call(const std::string& operation_id, const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listInboxChannels(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listInboxChannelAssignees(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listInboxConversations(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string searchInboxConversations(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string countInboxConversations(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string getInboxConversation(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string updateInboxConversationStatus(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string updateInboxConversationAssignee(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listInboxConversationMessages(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string blockInboxContact(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string unblockInboxContact(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string renameInboxConversation(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listInboxContacts(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string getInboxWebhook(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string setInboxWebhook(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string deleteInboxWebhook(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string rotateInboxWebhookSecret(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaignSegments(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string createCampaignSegment(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string getCampaignSegment(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string updateCampaignSegment(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string deleteCampaignSegment(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaignSegmentMembers(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string addCampaignSegmentMembers(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string replaceCampaignSegmentMembers(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string removeCampaignSegmentMembers(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string updateCampaignSegmentMember(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaigns(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string createCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string getCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string updateCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string previewCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string sendCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string scheduleCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string unscheduleCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string pauseCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string resumeCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string completeCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string testCampaign(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string getCampaignAnalytics(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaignAnalyticsRecipients(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaignAnalyticsReplies(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
    std::string listCampaignAnalyticsErrors(const std::map<std::string, std::string>& path = {}, const std::map<std::string, std::string>& query = {}, const std::string& body = {});
 private:
  std::string api_key_;
  std::string base_url_;
};
}}

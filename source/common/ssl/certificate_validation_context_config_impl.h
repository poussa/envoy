#pragma once

#include <string>

#include "envoy/api/api.h"
#include "envoy/extensions/transport_sockets/tls/v3/cert.pb.h"
#include "envoy/extensions/transport_sockets/tls/v3/common.pb.h"
#include "envoy/server/transport_socket_config.h"
#include "envoy/ssl/certificate_validation_context_config.h"
#include "envoy/type/matcher/v3/string.pb.h"

namespace Envoy {
namespace Ssl {

class CertificateValidationContextConfigImpl : public CertificateValidationContextConfig {
public:
  CertificateValidationContextConfigImpl(
      const envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext& config,
      Api::Api& api, Server::Configuration::TransportSocketFactoryContext& factory_context);

  const std::string& caCert() const override { return ca_cert_; }
  const std::string& caCertPath() const override { return ca_cert_path_; }
  const std::string& certificateRevocationList() const override {
    return certificate_revocation_list_;
  }
  const std::string& certificateRevocationListPath() const final {
    return certificate_revocation_list_path_;
  }
  const std::vector<envoy::extensions::transport_sockets::tls::v3::SubjectAltNameMatcher>&
  subjectAltNameMatchers() const override {
    return subject_alt_name_matchers_;
  }
  const std::vector<std::string>& verifyCertificateHashList() const override {
    return verify_certificate_hash_list_;
  }
  const std::vector<std::string>& verifyCertificateSpkiList() const override {
    return verify_certificate_spki_list_;
  }
  bool allowExpiredCertificate() const override { return allow_expired_certificate_; }
  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext::
      TrustChainVerification
      trustChainVerification() const override {
    return trust_chain_verification_;
  }

  const absl::optional<envoy::config::core::v3::TypedExtensionConfig>&
  customValidatorConfig() const override {
    return custom_validator_config_;
  }

  Api::Api& api() const override { return api_; }

  bool onlyVerifyLeafCertificateCrl() const override { return only_verify_leaf_cert_crl_; }

  absl::optional<uint32_t> maxVerifyDepth() const override { return max_verify_depth_; }

  Envoy::CertificateProvider::CertificateProviderSharedPtr caProvider() const override {
    return ca_provider_instance_;
  };

  void setCAUpdateCallback(std::function<void()> callback) override;

private:
  static std::vector<envoy::extensions::transport_sockets::tls::v3::SubjectAltNameMatcher>
  getSubjectAltNameMatchers(
      const envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext& config);
  std::string ca_cert_;
  const std::string ca_cert_path_;
  const std::string certificate_revocation_list_;
  const std::string certificate_revocation_list_path_;
  const std::vector<envoy::extensions::transport_sockets::tls::v3::SubjectAltNameMatcher>
      subject_alt_name_matchers_;
  const std::vector<std::string> verify_certificate_hash_list_;
  const std::vector<std::string> verify_certificate_spki_list_;
  const bool allow_expired_certificate_;
  const envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext::
      TrustChainVerification trust_chain_verification_;
  const absl::optional<envoy::config::core::v3::TypedExtensionConfig> custom_validator_config_;
  Api::Api& api_;
  const bool only_verify_leaf_cert_crl_;
  absl::optional<uint32_t> max_verify_depth_;
  Envoy::CertificateProvider::CertificateProviderSharedPtr ca_provider_instance_;
  std::string ca_provider_cert_name_;
  Envoy::Common::CallbackHandlePtr ca_update_callback_handle_;
};

} // namespace Ssl
} // namespace Envoy
